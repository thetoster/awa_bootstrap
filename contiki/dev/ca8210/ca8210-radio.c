/*
 * Copyright (c) 2016, Cascoda Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*---------------------------------------------------------------------------*/
// NETSTACK_RADIO Driver for the CA8210
// As the Radio contains a full 802.15.4 MAC according to IEEE802.15.4 2006,
// it requires the following NETSTACK Configuration:
// NETSTACK_RADIO:  ca8210_driver
// NETSTACK_FRAMER: framer_hardmac
// NETSTACK_RDC:    nullrdc_noframer_driver
// NETSTACK_MAC:    nullmac_driver
/*---------------------------------------------------------------------------*/

#include <contiki.h>
#include <sys/etimer.h>

#include "net/packetbuf.h"
#include "net/netstack.h"
#include "net/rime/rimestats.h"

#include "ca8210-radio.h"
#include "ca8210-conf.h"
#include "dev/ca8210/cascoda/include/cascoda_api.h"
#include "dev/ca8210/cascoda/include/cascoda_evbme.h"


#define DEBUGP 1
#if DEBUGP
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif
#define DEBUG_PRINTPAYLOAD 1


/*---------------------------------------------------------------------------*/
// RADIO Process Thread Declaration
/*---------------------------------------------------------------------------*/
PROCESS(ca8210_process, "CA8210 driver");
PROCESS_NAME(ca8210_test_process);


/*---------------------------------------------------------------------------*/
// MAC MCPS and PIB Parameters
/*---------------------------------------------------------------------------*/
u8_t         MAC_SrcAddrMode;
u8_t         MAC_DstAddrMode;
u16_t        MAC_DstPANId;
MacAddr_t    MAC_DstAddr;
u8_t         MAC_MsduHandle;
u8_t         MAC_TxOptions;
SecSpec_t    MAC_Security;
FullAddr_t   MAC_SrcAddress;
FullAddr_t   MAC_DstAddress;
u8_t         MAC_Lqi;
u8_t         MAC_Dsn;
u8_t         MAC_Rssi;
u8_t         MAC_MaxFrameRetries;
u8_t         Tx_Initialised = 0;
u8_t         Rx_Initialised = 0;
u8_t         Device_On      = 1;
u32_t        t_s_tx = 0;

extern u8_t                 PHY_TESTMODE;


/*---------------------------------------------------------------------------*/
// Initialisation
// (NETSTACK_RADIO.init)
/*---------------------------------------------------------------------------*/
int ca8210_init(void)
{

  PRINTF("ca8210: init\n");
  if(EVBMEInitialise((u8_t *)CA8210_VERSION))
   {
   Device_On = 0;
   return 0;
   }

  CA8210_IRQN_INIT();

  if(MAC_Workarounds)
   TDME_SETSFR_request_sync(0, 0xB2, 0x08);

  Device_On = 1;

  process_start(&ca8210_process, NULL);

  #if defined __USE_UART_PORT3_FOR_15_4_DEBUG__ || __USE_UART_PORT3_FOR_15_4_GUI__
  process_start(&ca8210_test_process, NULL);
  #endif

  return 1;
}


/*---------------------------------------------------------------------------*/
// Prepare Transmit Packet
// (NETSTACK_RADIO.prepare)
/*---------------------------------------------------------------------------*/
static int ca8210_prepare(const void *payload, unsigned short payload_len)
{
  u8_t broadcast;

  // protect packetbuffer from being overwritten by received packets
  CA8210_IRQN_DISABLE();

  RIMESTATS_ADD(lltx);

  broadcast = packetbuf_holds_broadcast();

  // assemble address information for data request
  MAC_DstPANId = IEEE802154_PANID;

  if(LINKADDR_SIZE == 2)
   MAC_SrcAddrMode = MAC_MODE_SHORT_ADDR;
  else
   MAC_SrcAddrMode = MAC_MODE_LONG_ADDR;

  if(broadcast)
   MAC_DstAddrMode = MAC_MODE_SHORT_ADDR;
  else if(LINKADDR_SIZE == 2)
   MAC_DstAddrMode = MAC_MODE_SHORT_ADDR;
  else
   MAC_DstAddrMode = MAC_MODE_LONG_ADDR;

  if(broadcast)
   MAC_DstAddr.ShortAddress = CA8210_BROADCAST_ADDRESS;
  else
   {
   linkaddr_copy((linkaddr_t *)&MAC_DstAddr, packetbuf_addr(PACKETBUF_ADDR_RECEIVER));
   if(LINKADDR_SIZE == 2)
    ca8210_reverse_bytes((uint8_t *)&MAC_DstAddr.ShortAddress, 2);
   else
    ca8210_reverse_bytes((uint8_t *)&MAC_DstAddr.IEEEAddress, 8);
   }

  // update packetbuf attributes
  packetbuf_set_addr(PACKETBUF_ADDR_SENDER, &linkaddr_node_addr);

  if(!Tx_Initialised)
   ca8210_get_random(&MAC_MsduHandle, 1);
  else
   ++MAC_MsduHandle;
  packetbuf_set_attr(PACKETBUF_ATTR_MAC_SEQNO, MAC_MsduHandle);

  // why not use the original attribute set in radio.h?
  if(RADIO_RX_MODE_AUTOACK)
   packetbuf_set_attr(PACKETBUF_ATTR_MAC_ACK, 1);

  // ack requested ?
  if(broadcast)
   MAC_TxOptions = 0x00;
  else if(packetbuf_attr(PACKETBUF_ATTR_MAC_ACK))
   MAC_TxOptions = TXOPT_ACKREQ;
  else
   MAC_TxOptions = 0x00;

  #if LLSEC802154_SECURITY_LEVEL
  MAC_Security.SecurityLevel = packetbuf_attr(PACKETBUF_ATTR_SECURITY_LEVEL);
  #if LLSEC802154_USES_EXPLICIT_KEYS
  MAC_Security.KeyIdMode = packetbuf_attr(PACKETBUF_ATTR_KEY_ID_MODE);
  MAC_Security.KeyIndex  = packetbuf_attr(PACKETBUF_ATTR_KEY_INDEX);
  // Note: Contiki implementation of key source with 2 Bytes doesn't make sense,
  // as per spec it's either 0, 4 bytes (panid + shortaddress) or 8 bytes (longaddress)
  MAC_Security.KeySource[0]  = (packetbuf_attr(PACKETBUF_ATTR_KEY_SOURCE_BYTES_0_1) >> 0) & 0xFF;
  MAC_Security.KeySource[1]  = (packetbuf_attr(PACKETBUF_ATTR_KEY_SOURCE_BYTES_0_1) >> 1) & 0xFF;
  memset(MAC_Security.KeySource+2, 0x00, 6);
  #else  // LLSEC802154_USES_EXPLICIT_KEYS
  MAC_Security.KeyIdMode = 0;
  #endif // LLSEC802154_USES_EXPLICIT_KEYS
  #else  // LLSEC802154_SECURITY_LEVEL
  MAC_Security.SecurityLevel = 0;
  #endif // LLSEC802154_SECURITY_LEVEL

  Tx_Initialised = 1;

  return 1;
}


/*---------------------------------------------------------------------------*/
// Transmit prepared Packet
// (NETSTACK_RADIO.transmit)
/*---------------------------------------------------------------------------*/
static int ca8210_transmit(unsigned short transmit_len)
{
  u8_t  status;
  u8_t  cnfhandle;
  u32_t dummy32;

  if(!Device_On)
   NETSTACK_RADIO.on();

  if(MAC_Workarounds)
   TDME_SETSFR_request_sync(0, 0xB2, 0x00);

  // set MaxFrameRetries in MAC PIB
  if((!Tx_Initialised) || (MAC_MaxFrameRetries != packetbuf_attr(PACKETBUF_ATTR_MAX_MAC_TRANSMISSIONS)))
   {
   MAC_MaxFrameRetries = packetbuf_attr(PACKETBUF_ATTR_MAX_MAC_TRANSMISSIONS);
   MLME_SET_request_sync(macMaxFrameRetries, 0, 1, &MAC_MaxFrameRetries);
   }

  // re-enable interrupts
  CA8210_IRQN_ENABLE();

  // MCPS-Data Request
  MCPS_DATA_request(
   MAC_SrcAddrMode,             // SrcAddrMode
   MAC_DstAddrMode,             // DstAddrMode
   MAC_DstPANId,                // DstPANId
   &MAC_DstAddr,                // *pDstAddr
   transmit_len,                // MsduLength
   packetbuf_dataptr(),         // *pMsdu
   MAC_MsduHandle,              // MsduHandle
   MAC_TxOptions,               // TxOptions
   &MAC_Security);              // *pSecurity

  ENERGEST_OFF(ENERGEST_TYPE_LISTEN);
  ENERGEST_ON(ENERGEST_TYPE_TRANSMIT);

  // wait for confirm
  if(EVBME_WaitforResponse(SPI_MCPS_DATA_CONFIRM, 10000))
   {
   PRINTF("ca8210_transmit: timeout\n");
   return RADIO_TX_ERR;
   }

  // MCPS-DATA Confirm
  status = MCPS_DATA_confirm(&cnfhandle, &dummy32);

  if(status)
   {
   PRINTF("ca8210_transmit: confirm status: %02X\n", status);
   if((status == MAC_SYSTEM_ERROR) || (status == MAC_TRANSACTION_OVERFLOW))
    {
    MLME_RESET_request_sync(0);
    return RADIO_TX_ERR;
    }
   }

  ENERGEST_OFF(ENERGEST_TYPE_TRANSMIT);
  ENERGEST_ON(ENERGEST_TYPE_LISTEN);

  if(MAC_Workarounds)
   TDME_SETSFR_request_sync(0, 0xB2, 0x08);

  if(cnfhandle != MAC_MsduHandle)
   {
   PRINTF("ca8210_transmit: MSDU handle mismatch %d - %d\n", cnfhandle, MAC_MsduHandle);
   return RADIO_TX_ERR;
   }

  if(status == MAC_CHANNEL_ACCESS_FAILURE)
   {
   RIMESTATS_ADD(contentiondrop);
   return(RADIO_TX_COLLISION);
   }
  else if(status == MAC_NO_ACK)
   {
   RIMESTATS_ADD(badackrx);
   return(RADIO_TX_NOACK);
   }

  return RADIO_TX_OK;
}


/*---------------------------------------------------------------------------*/
// Prepare and Transmit Packet
// (NETSTACK_RADIO.send)
/*---------------------------------------------------------------------------*/
static int ca8210_send(const void *payload, unsigned short payload_len)
{
int length;

  if(PHY_TESTMODE)
   return 0;

  #if DEBUG_PRINTPAYLOAD
  PRINTF("ca8210: send (%d) bytes", payload_len);
  PRINTF(": ");
  unsigned short i;
  for(i=0;i<payload_len;++i)
   PRINTF(" %02X", ((uint8_t *) payload)[i]);
  PRINTF("\n");
  #endif

  ca8210_prepare(payload, payload_len);
  length = ca8210_transmit(payload_len);

  // wait if last send was called less than CA8210_T_MIN_TX ago
  // done at end of send, as rx is on and packetbuffer could be overwritten if at start
  if(((clock_time() - t_s_tx) < CA8210_T_MIN_TX) && (CA8210_T_MIN_TX != 0))
   clock_delay_usec((clock_time() - t_s_tx)*1000);
  t_s_tx = clock_time();

  return length;
}


/*---------------------------------------------------------------------------*/
// Read received Packet
// (NETSTACK_RADIO.read)
/*---------------------------------------------------------------------------*/
static int ca8210_read(void *buf, unsigned short buf_len)
{
  u8_t  status;
  u8_t  length;
  u8_t  dsn_new;
  u32_t dummy32;
  u8_t  dummmy8;

  // MCPS-DATA Indication
  status = MCPS_DATA_indication(
   &MAC_SrcAddress,  // *Src
   &MAC_DstAddress,  // *Dst
   &length,          // *MsduLength
   &MAC_Lqi,         // *MpduLinkQuality
   &dsn_new,         // *DSN
   &dummy32,         // *pTimeStamp
   buf,              // *pMsdu
   &MAC_Security);   // *pSecurity

  if(status)
   {
   PRINTF("ca8210_read failed\n");
   return(0);
   }

  // if DSN is the same than previous this is a re-send (ack got lost)
  if((Rx_Initialised) && (dsn_new == MAC_Dsn))
   {
   PRINTF("ca8210_read discarding duplicate packet\n");
   return(0);
   }

  Rx_Initialised = 1;
  MAC_Dsn = dsn_new;

  // get ED/RSSI value
  HWME_GET_request_sync(HWME_EDVALLP,  &dummmy8, &MAC_Rssi);

  #if DEBUG_PRINTPAYLOAD
  PRINTF("ca8210: read (%d) bytes", length);
  PRINTF(": ");
  unsigned short i;
  for(i=0;i<length;++i)
   PRINTF(" %02X", ((uint8_t *) buf)[i]);
  PRINTF("\n");
  #endif

  // update packetbuf attributes
  ca8210_reverse_bytes((uint8_t *)&MAC_SrcAddress.Address, LINKADDR_SIZE);
  packetbuf_set_addr(PACKETBUF_ADDR_SENDER, (linkaddr_t *)&MAC_SrcAddress.Address);

  if((MAC_DstAddress.AddressMode != MAC_MODE_SHORT_ADDR) || ((MAC_DstAddress.Address[0] + (MAC_DstAddress.Address[1] << 8)) != CA8210_BROADCAST_ADDRESS))
   {
   ca8210_reverse_bytes((uint8_t *)&MAC_DstAddress.Address, LINKADDR_SIZE);
   packetbuf_set_addr(PACKETBUF_ADDR_RECEIVER, (linkaddr_t *)&MAC_DstAddress.Address);
   }

  packetbuf_set_attr(PACKETBUF_ATTR_FRAME_TYPE, MAC_FC_FT_DATA);
  packetbuf_set_attr(PACKETBUF_ATTR_MAC_SEQNO, MAC_Dsn);
  #if NETSTACK_CONF_WITH_RIME
  packetbuf_set_attr(PACKETBUF_ATTR_PACKET_ID, MAC_Dsn);
  #endif
  packetbuf_set_attr(PACKETBUF_ATTR_RSSI, MAC_Rssi);
  packetbuf_set_attr(PACKETBUF_ATTR_LINK_QUALITY, MAC_Lqi);

  #if LLSEC802154_SECURITY_LEVEL
  MAC_Security.SecurityLevel = packetbuf_attr(PACKETBUF_ATTR_SECURITY_LEVEL);
  #if LLSEC802154_USES_EXPLICIT_KEYS
  MAC_Security.KeyIdMode = packetbuf_attr(PACKETBUF_ATTR_KEY_ID_MODE);
  MAC_Security.KeyIndex  = packetbuf_attr(PACKETBUF_ATTR_KEY_INDEX);
  // Note: Contiki implementation of key source with 2 Bytes doesn't make sense,
  // as per spec it's either 0, 4 bytes (panid + shortaddress) or 8 bytes (longaddress)
  MAC_Security.KeySource[0]  = (packetbuf_attr(PACKETBUF_ATTR_KEY_SOURCE_BYTES_0_1) >> 0) & 0xFF;
  MAC_Security.KeySource[1]  = (packetbuf_attr(PACKETBUF_ATTR_KEY_SOURCE_BYTES_0_1) >> 8) & 0xFF;
  memset(MAC_Security.KeySource+2, 0x00, 6);
  #else  // LLSEC802154_USES_EXPLICIT_KEYS
  MAC_Security.KeyIdMode = 0;
  #endif // LLSEC802154_USES_EXPLICIT_KEYS
  #else  // LLSEC802154_SECURITY_LEVEL
  MAC_Security.SecurityLevel = 0;
  #endif // LLSEC802154_SECURITY_LEVEL

  #if LLSEC802154_SECURITY_LEVEL
  packetbuf_set_attr(PACKETBUF_ATTR_SECURITY_LEVEL, MAC_Security.SecurityLevel);
  #if LLSEC802154_USES_EXPLICIT_KEYS
  if(MAC_Security.SecurityLevel)
   {
   packetbuf_set_attr(PACKETBUF_ATTR_KEY_ID_MODE, MAC_Security.KeyIdMode);
   packetbuf_set_attr(PACKETBUF_ATTR_KEY_INDEX, MAC_Security.KeyIndex);
   // Note: Contiki implementation of key source with 2 Bytes doesn't make sense,
   // as per spec it's either 0, 4 bytes (panid + shortaddress) or 8 bytes (longaddress)
   packetbuf_set_attr(PACKETBUF_ATTR_KEY_SOURCE_BYTES_0_1, (MAC_Security.KeySource[0] + (MAC_Security.KeySource[1]<<8)));
   }
  #endif // LLSEC802154_USES_EXPLICIT_KEYS
  #endif // LLSEC802154_SECURITY_LEVEL

  RIMESTATS_ADD(llrx);

  return (int)length;
}


/*---------------------------------------------------------------------------*/
// Perform Channel Clear Assesment (CCA)
// (NETSTACK_RADIO.channel_clear)
/*---------------------------------------------------------------------------*/
static int ca8210_channel_clear(void)
{

  PRINTF("ca8210: channel_clear\n");

  // the MAC is below this layer, so has always to be assumed to be clear
  return 1;
}


/*---------------------------------------------------------------------------*/
// Check if Receiving a Packet
// (NETSTACK_RADIO.receiving_packet)
/*---------------------------------------------------------------------------*/
static int ca8210_receiving_packet(void)
{

  PRINTF("ca8210: receiving_packet\n");

  // the MAC is below this layer, so has always to be assumed to be false
  return 0;
}


/*---------------------------------------------------------------------------*/
// Check if a Packet has been received but has not yet been processed
// (NETSTACK_RADIO.pending_packet)
/*---------------------------------------------------------------------------*/
static int ca8210_pending_packet(void)
{

  PRINTF("ca8210: pending_packet\n");

  // MAC layers is queueing and immediately flagging MCPS-DATA Indication via interrupt
  return 0;
}


/*---------------------------------------------------------------------------*/
// Turn Transceiver on (with Rx enabled)
// (NETSTACK_RADIO.on)
/*---------------------------------------------------------------------------*/
static int ca8210_on(void)
{
  u8_t status = MAC_SUCCESS;
  u8_t rxidle = 1;

  PRINTF("ca8210: on\n");

  if(!Device_On)
   EVBME_Wakeup(CA8210_LOWPOWER_MODE);

  CA8210_IRQN_ENABLE();

  if((status = MLME_SET_request_sync(macRxOnWhenIdle,    0, 1, &rxidle)))
   PRINTF("MLME_SET macRxOnWhenIdle failed: %02X\n", status);

  Device_On = 1;

  ENERGEST_ON(ENERGEST_TYPE_LISTEN);

  return 0;
}


/*---------------------------------------------------------------------------*/
// Turn Transceiver off
// (NETSTACK_RADIO.off)
/*---------------------------------------------------------------------------*/
static int ca8210_off(void)
{
  u8_t status = MAC_SUCCESS;
  u8_t rxidle = 0;

  PRINTF("ca8210: off\n");

  if((status = MLME_SET_request_sync(macRxOnWhenIdle,    0, 1, &rxidle)))
   PRINTF("MLME_SET macRxOnWhenIdle failed: %02X\n", status);

  if(Device_On)
   EVBME_PowerDown(CA8210_LOWPOWER_MODE, 0);

  CA8210_IRQN_DISABLE();

  Device_On = 0;

  ENERGEST_OFF(ENERGEST_TYPE_LISTEN);

  return 0;
}


/*---------------------------------------------------------------------------*/
// Get a RADIO Parameter
// (NETSTACK_RADIO.get_value)
/*---------------------------------------------------------------------------*/
radio_result_t ca8210_get_value(radio_param_t param, radio_value_t *value)
{
u8_t length;
uint8_t buf[4];

  PRINTF("ca8210: get_value\n");

  if(!value)
   return RADIO_RESULT_INVALID_VALUE;
  *value = 0;

  // getting parameters other than RADIO_PARAM_POWER_MODE requires device to be on
  if((!Device_On) && (param != RADIO_PARAM_POWER_MODE))
   return RADIO_RESULT_ERROR;

  // fill in parameter
  switch(param)
   {
   case RADIO_PARAM_POWER_MODE:
    *value = Device_On ? RADIO_POWER_MODE_ON : RADIO_POWER_MODE_OFF;
    return RADIO_RESULT_OK;
   case RADIO_PARAM_CHANNEL:
    MLME_GET_request_sync(phyCurrentChannel, 0, &length, (u8_t *)value);
    return RADIO_RESULT_OK;
   case RADIO_PARAM_PAN_ID:
    MLME_GET_request_sync(macPANId, 0, &length, (u8_t *)value);
    return RADIO_RESULT_OK;
   case RADIO_PARAM_16BIT_ADDR:
    MLME_GET_request_sync(macShortAddress, 0, &length, buf);
    ca8210_reverse_bytes(buf, 2);
    memcpy(value, buf, 2);
    return RADIO_RESULT_OK;
   case RADIO_PARAM_RX_MODE:
    // static, has to made dynamic if radio MAC functions are to be disabled in future
    *value = RADIO_RX_MODE_ADDRESS_FILTER|RADIO_RX_MODE_AUTOACK;
    return RADIO_RESULT_OK;
   case RADIO_PARAM_TX_MODE:
    // static, has to made dynamic if radio MAC functions are to be disabled in future
    *value = RADIO_TX_MODE_SEND_ON_CCA;
    return RADIO_RESULT_OK;
   case RADIO_PARAM_TXPOWER:
    MLME_GET_request_sync(phyTransmitPower, 0, &length, (u8_t *)value);
    return RADIO_RESULT_OK;
   case RADIO_PARAM_CCA_THRESHOLD:
    // according to contiki description rssi (ed) only
    HWME_GET_request_sync(HWME_EDTHRESHOLD, &length, (u8_t *)value);
    return RADIO_RESULT_OK;
   case RADIO_PARAM_RSSI:
    *value = MAC_Rssi;
    return RADIO_RESULT_OK;
   case RADIO_PARAM_64BIT_ADDR:
    // too large, requires get_object
    return RADIO_RESULT_NOT_SUPPORTED;
   case RADIO_CONST_CHANNEL_MIN:
    *value = CA8210_MIN_CHANNEL;
    return RADIO_RESULT_OK;
   case RADIO_CONST_CHANNEL_MAX:
    *value = CA8210_MAX_CHANNEL;
    return RADIO_RESULT_OK;
   case RADIO_CONST_TXPOWER_MIN:
    *value = CA8210_TXPOWER_MIN;
    return RADIO_RESULT_OK;
   case RADIO_CONST_TXPOWER_MAX:
    *value = CA8210_TXPOWER_MAX;
    return RADIO_RESULT_OK;
   default:
    return RADIO_RESULT_NOT_SUPPORTED;
   }

  return RADIO_RESULT_NOT_SUPPORTED;
}


/*---------------------------------------------------------------------------*/
// Set a RADIO Parameter
// (NETSTACK_RADIO.set_value)
/*---------------------------------------------------------------------------*/
static radio_result_t ca8210_set_value(radio_param_t param, radio_value_t value)
{
  uint8_t buf[4];

  PRINTF("ca8210: set_value\n");

  // setting parameters other than RADIO_PARAM_POWER_MODE requires device to be on
  if((!Device_On) && (param != RADIO_PARAM_POWER_MODE))
   return RADIO_RESULT_ERROR;

  switch(param)
   {
   case RADIO_PARAM_POWER_MODE:
    if(value == RADIO_POWER_MODE_ON)
     {
     NETSTACK_RADIO.on();
     return RADIO_RESULT_OK;
     }
    else if(value == RADIO_POWER_MODE_OFF)
     {
	 ca8210_off();
     return RADIO_RESULT_OK;
     }
    else    {
     return RADIO_RESULT_INVALID_VALUE;
	 }
   case RADIO_PARAM_CHANNEL:
    if((value >= CA8210_MIN_CHANNEL) && (value <= CA8210_MAX_CHANNEL))
     {
     MLME_SET_request_sync(phyCurrentChannel, 0, 1, &value);
     return RADIO_RESULT_OK;
	 }
    else
     {
     return RADIO_RESULT_INVALID_VALUE;
     }
   case RADIO_PARAM_PAN_ID:
    MLME_SET_request_sync(macPANId, 0, 2, &value);
    return RADIO_RESULT_OK;
   case RADIO_PARAM_16BIT_ADDR:
    memcpy(buf, &value, 2);
    ca8210_reverse_bytes(buf, 2);
    MLME_SET_request_sync(macShortAddress, 0, 2, buf);
    return RADIO_RESULT_OK;
   case RADIO_PARAM_RX_MODE:
    // static, has to made dynamic if radio MAC functions are to be disabled in future
    return RADIO_RESULT_NOT_SUPPORTED;
   case RADIO_PARAM_TX_MODE:
    // static, has to made dynamic if radio MAC functions are to be disabled in future
    return RADIO_RESULT_NOT_SUPPORTED;
   case RADIO_PARAM_TXPOWER:
    if(value < CA8210_TXPOWER_MIN)
     MLME_SET_request_sync(phyTransmitPower, 0, 1, (u8_t *)CA8210_TXPOWER_MIN);
    else if (value > CA8210_TXPOWER_MAX)
     MLME_SET_request_sync(phyTransmitPower, 0, 1, (u8_t *)CA8210_TXPOWER_MAX);
    else
     MLME_SET_request_sync(phyTransmitPower, 0, 1, &value);
    return RADIO_RESULT_OK;
   case RADIO_PARAM_CCA_THRESHOLD:
    // according to contiki description rssi (ed) only
    HWME_SET_request_sync(HWME_EDTHRESHOLD, 1, (u8_t *)&value);
    return RADIO_RESULT_OK;
   case RADIO_PARAM_RSSI:
    // read-only
    return RADIO_RESULT_NOT_SUPPORTED;
   case RADIO_PARAM_64BIT_ADDR:
    // too large, requires set_object
    return RADIO_RESULT_NOT_SUPPORTED;
   case RADIO_CONST_CHANNEL_MIN:
    // read-only
    return RADIO_RESULT_NOT_SUPPORTED;
   case RADIO_CONST_CHANNEL_MAX:
    // read-only
    return RADIO_RESULT_NOT_SUPPORTED;
   case RADIO_CONST_TXPOWER_MIN:
    // read-only
    return RADIO_RESULT_NOT_SUPPORTED;
   case RADIO_CONST_TXPOWER_MAX:
    // read-only
    return RADIO_RESULT_NOT_SUPPORTED;
   default:
    return RADIO_RESULT_NOT_SUPPORTED;
   }

  return RADIO_RESULT_NOT_SUPPORTED;
}


/*---------------------------------------------------------------------------*/
// Get a RADIO Parameter Object
// (NETSTACK_RADIO.get_object)
/*---------------------------------------------------------------------------*/
static radio_result_t ca8210_get_object(radio_param_t param, void *dest, size_t size)
{
  u8_t length;
  uint8_t buf[8];

  PRINTF("ca8210: get_object\n");

  switch(param)
   {
   case RADIO_PARAM_64BIT_ADDR:
    if(size == 8)
     {
     MLME_GET_request_sync(macShortAddress, 0, &length, buf);
     ca8210_reverse_bytes(buf, 8);
     memcpy(dest, buf, 8);
     return RADIO_RESULT_OK;
	 }
	else
	 {
     return RADIO_RESULT_INVALID_VALUE;
	 }
   default:
    return RADIO_RESULT_NOT_SUPPORTED;
   }

  return RADIO_RESULT_NOT_SUPPORTED;
}


/*---------------------------------------------------------------------------*/
// Set a RADIO Parameter Object
// (NETSTACK_RADIO.set_object)
/*---------------------------------------------------------------------------*/
static radio_result_t ca8210_set_object(radio_param_t param, const void *src, size_t size)
{
  uint8_t buf[8];

  PRINTF("ca8210: set_object\n");

  switch(param)
   {
   case RADIO_PARAM_64BIT_ADDR:
    if(size == 8)
     {
     memcpy(buf, src, 8);
     ca8210_reverse_bytes(buf, 8);
     MLME_SET_request_sync(nsIEEEAddress, 0, 8, buf);
     return RADIO_RESULT_OK;
	 }
	else
	 {
     return RADIO_RESULT_INVALID_VALUE;
	 }
   default:
    return RADIO_RESULT_NOT_SUPPORTED;
   }

  return RADIO_RESULT_NOT_SUPPORTED;
}


/*---------------------------------------------------------------------------*/
// Main RADIO Process Thread
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ca8210_process, ev, data)
{
  int len;

  PROCESS_BEGIN();

  PRINTF("ca8210_process: started\n");

  while(1) {
    PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);

    // PRINTF("ca8210_process: calling callback\n");

    if(EVBMEHandler())            // DownStream Communications (SPI)
     {

     if(PHY_TESTMODE)
      {
      // Insert Application-Specific DownStream Dispatches (from SPI) here
      ca8210_test_downstreamdispatch();
      }
     else
      {
      // dispatch on interrupt
      // MCPS-DATA Indication -> read
      if(EVBME_CompareCommand(SPI_MCPS_DATA_INDICATION))
       {

       packetbuf_clear();
       len = ca8210_read(packetbuf_dataptr(), PACKETBUF_SIZE);
       if(len)
        {
        packetbuf_set_datalen(len);
        NETSTACK_RDC.input();
        }

       }
      else if(!EVBME_CompareCommand(SPI_MCPS_DATA_CONFIRM))
       {
       // all others
       #if defined __USE_UART_PORT3_FOR_15_4_DEBUG__ || __USE_UART_PORT3_FOR_15_4_GUI__
       EVBMESendUpStream();
       #endif
 	   }
      }
     }

  }

  PROCESS_END();
}


/*---------------------------------------------------------------------------*/
// Initialise MAC PIB
/*---------------------------------------------------------------------------*/
int ca8210_init_pib(int channel, unsigned panid, unsigned addr, const uint8_t *ieee_addr)
{
  u8_t status = MAC_SUCCESS;
  uint8_t buf[8];

  // set phyCurrentChannel
  if((channel >= CA8210_MIN_CHANNEL) && (channel <= CA8210_MAX_CHANNEL))
   {
   status |= MLME_SET_request_sync(phyCurrentChannel, 0, 1, &channel);
   }

  // set macPANId
  status |= MLME_SET_request_sync(macPANId,          0, 2, &panid);

  // set IEEEAddress
  memcpy(buf, ieee_addr, 8);
  ca8210_reverse_bytes(buf, 8);
  status |= MLME_SET_request_sync(nsIEEEAddress,     0, 8, buf);

  // set macShortAddress
  memcpy(buf, &addr, 2);
  ca8210_reverse_bytes(buf, 2);
  status |= MLME_SET_request_sync(macShortAddress,   0, 2, buf);

  if(status)
   PRINTF("ca8210_init_pib failed: %02X\n", status);

  return(status);
}


/*---------------------------------------------------------------------------*/
// Get Random Number
/*---------------------------------------------------------------------------*/
void ca8210_get_random(uint8_t *num, int count)
{
  u8_t status = MAC_SUCCESS;
  u8_t len;
  u8_t rand[2];
  u8_t i;

  for(i=0;i<count;++i)
   {
   if((i % 2) == 0)
    {
    status |= HWME_GET_request_sync(HWME_RANDOMNUM, &len, rand);
    num[i] = rand[0];
    }
   else
    {
    num[i] = rand[1];
	}
   }

  if(status)
   PRINTF("ca8210_get_random failed: %02X\n", status);
}


/*---------------------------------------------------------------------------*/
// get Tx Power in [dBm]
/*---------------------------------------------------------------------------*/
int ca8210_get_txpower(void)
{
  u8_t length;
  i8_t txpower;

  MLME_GET_request_sync(phyTransmitPower, 0, &length, &txpower);
  txpower = 0x3F & txpower;
  if(txpower & 0x20)
   txpower += 0xC0;

  return((int)txpower);
}


/*---------------------------------------------------------------------------*/
// Reverse Byte Order (used for Address Endian-nes)
/*---------------------------------------------------------------------------*/
void ca8210_reverse_bytes(uint8_t *u8buf, uint8_t n)
{

  #if CA8210_REVERSE_ADDR_BYTEORDER
  uint8_t tmp, i, j;

  i = 0;
  j = n - 1;

  while(i < j)
   {
   tmp      = u8buf[i];
   u8buf[i] = u8buf[j];
   u8buf[j] = tmp;
   j--;
   i++;
   }
  #endif

  return;
}


/*---------------------------------------------------------------------------*/
// CA8210 NIRQ (INT1) Interrupt Handler
/*---------------------------------------------------------------------------*/
int ca8210_interrupt(void)
{
  CA8210_IRQN_CLEAR();

  PRINTF("ca8210: interrupt received\n");
  process_poll(&ca8210_process);

  return 1;
}


/*---------------------------------------------------------------------------*/
// RADIO Device Driver Declaration
/*---------------------------------------------------------------------------*/
const struct radio_driver ca8210_driver =
  {
    ca8210_init,
    ca8210_prepare,
    ca8210_transmit,
    ca8210_send,
    ca8210_read,
    ca8210_channel_clear,
    ca8210_receiving_packet,
    ca8210_pending_packet,
    ca8210_on,
    ca8210_off,
    ca8210_get_value,
    ca8210_set_value,
    ca8210_get_object,
    ca8210_set_object
  };


/*---------------------------------------------------------------------------*/
