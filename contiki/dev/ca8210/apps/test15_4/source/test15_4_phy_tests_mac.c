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

/******************************************************************************/
/******************************************************************************/
/****** Cascoda Ltd. 2014, CA-821X EVB Driver Code                       ******/
/******************************************************************************/
/******************************************************************************/
/****** PHY Test Functions using MAC Functions for Data Reliablity       ******/
/******************************************************************************/
/******************************************************************************/
/****** Revision           Notes                                         ******/
/******************************************************************************/
/****** 1.0  19/07/14  WB  Release Baseline                              ******/
/******************************************************************************/
/******************************************************************************/
#include <string.h>
#include "dev/ca8210/cascoda/include/cascoda_types.h"
#include "dev/ca8210/cascoda/include/mac_messages.h"
#include "dev/ca8210/cascoda/include/cascoda_api.h"
#include "dev/ca8210/cascoda/include/cascoda_spi.h"
#include "dev/ca8210/apps/test15_4/include/test15_4_phy_tests.h"


/******************************************************************************/
/****** Address/PID Definitions                                          ******/
/******************************************************************************/
#define  PHY_PANID         0xCA5C
#define  PHY_TX_SHORTADD   0xCA51
#define  PHY_RX_SHORTADD   0xCA52
#define  PHY_TX_LONGADD    {0x01, 0x00, 0x00, 0x00, 0xA0, 0x0D, 0x5C, 0xCA}
#define  PHY_RX_LONGADD    {0x02, 0x00, 0x00, 0x00, 0xA0, 0x0D, 0x5C, 0xCA}


/******************************************************************************/
/****** Global Variables from PHY Tests                                  ******/
/******************************************************************************/
extern u8_t PHYBuffer[256];
extern u8_t PHYLength;


/******************************************************************************/
/****** Global Variables only used in Phy Test Files                     ******/
/******************************************************************************/
FullAddr_t SrcFAdd, DstFAdd;
MacAddr_t  DstAdd;

u16_t   PHYPANId;
u16_t   PHYTxShortAddress;
u16_t   PHYRxShortAddress;
u8_t    PHYTxLongAddress[8];
u8_t    PHYRxLongAddress[8];

u8_t    DSN_OLD;


/******************************************************************************/
/******************************************************************************/
/****** PHYTestMACAddInit()                                              ******/
/******************************************************************************/
/****** Brief: Address Initialisation                                    ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestMACAddInit(void)
 {
 PHYPANId           = PHY_PANID;
 PHYTxShortAddress  = PHY_TX_SHORTADD;
 PHYRxShortAddress  = PHY_RX_SHORTADD;
 memcpy(PHYTxLongAddress, (u8_t[])PHY_TX_LONGADD, 8);
 memcpy(PHYRxLongAddress, (u8_t[])PHY_RX_LONGADD, 8);
 }


/******************************************************************************/
/******************************************************************************/
/****** PHYTestMACTxInitialise()                                         ******/
/******************************************************************************/
/****** Brief:  Sequential Initialisation for using MAC layer in EVBME   ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t PHYTestMACTxInitialise(void)
 {
 u8_t status = MAC_SUCCESS;

 PHYTestMACAddInit();

 if((status = TDME_TESTMODE_request_sync(TDME_TEST_OFF)))                        // turn off test mode in order to be able to use MAC
  return(status);

 if((status = TDME_SETSFR_request_sync(0, 0xB2, 0x00)))                          // turn off AUTED
  return(status);

 PHYBuffer[0] = CCAM_EDORCS;
 if((status = HWME_SET_request_sync(HWME_CCAMODE, 1, PHYBuffer)))                // set CCA mode to ED OR CS
  return(status);

 PHYBuffer[0] = PHY_TESTPAR.EDTHRESHOLD;
 if((status = HWME_SET_request_sync(HWME_EDTHRESHOLD, 1, PHYBuffer)))            // set ED threshold to PHY_TESTPAR.EDTHRESHOLD
  return(status);

 if((status = MLME_SET_request_sync(nsIEEEAddress,   0, 8, PHYTxLongAddress)))   // set local long address
  return(status);

 if((status = MLME_SET_request_sync(macShortAddress, 0, 2, &PHYTxShortAddress))) // set local short address
  return(status);

 if((status = MLME_START_request_sync(
  PHYPANId,                  // PANId
  PHY_TESTPAR.CHANNEL,       // LogicalChannel
  15,                        // BeaconOrder
  15,                        // SuperframeOrder
  1,                         // PANCoordinator
  0,                         // BatteryLifeExtension
  1,                         // CoordRealignment
  NULLP,                     // *pCoordRealignSecurity
  NULLP)))                   // *pBeaconSecurity
  return(status);

 PHYBuffer[0] = 0;
 if((status = MLME_SET_request_sync(macRxOnWhenIdle, 0, 1, PHYBuffer)))          // turn receiver off
  return(status);

 return(status);
 } // End of PHYTestMACTxInitialise()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestMACRxInitialise()                                         ******/
/******************************************************************************/
/****** Brief:  Sequential Initialisation for using MAC layer in EVBME   ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t PHYTestMACRxInitialise(void)
 {
 u8_t status = MAC_SUCCESS;

 PHYTestMACAddInit();

 if((status = TDME_TESTMODE_request_sync(TDME_TEST_OFF)))                        // turn off test mode in order to be able to use MAC
  return(status);

 if((status = TDME_SETSFR_request_sync(0, 0xB2, 0x08)))                          // turn on AUTED
  return(status);

 if((status = MLME_SET_request_sync(macPANId,        0, 2, &PHYPANId)))          // set local PANId
  return(status);

 if((status = MLME_SET_request_sync(nsIEEEAddress,   0, 8, PHYRxLongAddress)))   // set local long address
  return(status);

 if((status = MLME_SET_request_sync(macShortAddress, 0, 2, &PHYRxShortAddress))) // set local short address
  return(status);

 PHYBuffer[0] = 1;
 if((status = MLME_SET_request_sync(macRxOnWhenIdle, 0, 1, PHYBuffer)))          // turn receiver on
  return(status);

 DSN_OLD = 0;

 return(status);
 } // End of PHYTestMACRxInitialise()


/******************************************************************************/
/****** PHY_TXPKT_MAC_request()                                          ******/
/******************************************************************************/
/****** Brief:  PHY Test Wrapper for MCPS_DATA_request()                 ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t PHY_TXPKT_MAC_request(void)
 {
 u8_t i;
 u8_t status = MAC_SUCCESS;
 u8_t randnum[2];
 u8_t attlen;

 PHYLength    =  PHY_TESTPAR.PACKETLENGTH;
 PHYBuffer[1] =  PHY_TESTRES.SEQUENCENUMBER;

 if(PHY_TESTPAR.PACKETDATATYPE == TDME_TXD_APPENDED)
  {
  for(i=0;i<PHY_TESTPAR.PACKETLENGTH;++i)
   PHYBuffer[i+2] = 0x00; // currently filled with 0's
  }
 else if(PHY_TESTPAR.PACKETDATATYPE == TDME_TXD_RANDOM)
  {
  for(i=0;i<PHY_TESTPAR.PACKETLENGTH;++i)
   {
   if((status = HWME_GET_request_sync(HWME_RANDOMNUM, &attlen, randnum)))
    return(status);
   else
    PHYBuffer[i+2] = randnum[0];
   }
  }
 else if(PHY_TESTPAR.PACKETDATATYPE == TDME_TXD_SEQRANDOM)
  {
  PHYBuffer[2] = PHY_TESTRES.SEQUENCENUMBER;
  for(i=1;i<PHY_TESTPAR.PACKETLENGTH;++i)
   {
   if((status = HWME_GET_request_sync(HWME_RANDOMNUM, &attlen, randnum)))
    return(status);
   else
    PHYBuffer[i+2] = randnum[0];
   }
  }
 else // PHY_TESTPAR.PACKETDATATYPE == TDME_TXD_COUNT)
  {
  for(i=0;i<PHY_TESTPAR.PACKETLENGTH;++i)
   PHYBuffer[i+2] = i;
 }

 DstAdd.ShortAddress = PHYRxShortAddress;

 MCPS_DATA_request(
  MAC_MODE_SHORT_ADDR,         // SrcAddrMode
  MAC_MODE_SHORT_ADDR,         // DstAddrMode
  PHYPANId,                    // DstPANId
  &DstAdd,                     // *pDstAddr
  PHYLength,                   // MsduLength
  PHYBuffer+2,                 // *pMsdu
  PHY_TESTRES.SEQUENCENUMBER,  // MsduHandle
  TXOPT_ACKREQ,                // TxOptions
  NULLP);                      // *pSecurity

  do
   {
   SPI_Wait();
   } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM);
 status = SPI_Receive.PData.DataCnf.Status;

 if(status == MAC_NO_ACK)
  {
  ++PHY_TESTRES.SHRERR_COUNT; // SHRERR_COUNT used for counting NO_ACK
  status = MAC_SUCCESS;
  }
 else if(status == MAC_CHANNEL_ACCESS_FAILURE)
  {
  ++PHY_TESTRES.PHRERR_COUNT; // PHRERR_COUNT used for counting CHANNEL_ACCESS_FAILURE
  status = MAC_SUCCESS;
  }
 else
  {
  ++PHY_TESTRES.SEQUENCENUMBER;
  }

 PHYBuffer[0] = status;

 return(status);
 } // End of PHY_TXPKT_MAC_request()


/******************************************************************************/
/******************************************************************************/
/****** PHY_RXPKT_MAC_indication()                                       ******/
/******************************************************************************/
/****** Brief:  PHY Test Wrapper for MCPS_DATA_indication()              ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t PHY_RXPKT_MAC_indication(void)
 {
 u8_t status = MAC_SUCCESS;
 u8_t MsduLength, MpduLinkQuality, DSN;
 u8_t edvallp, freqoffs;
 u8_t len;

 status = MCPS_DATA_indication(
  &SrcFAdd,              // *Src
  &DstFAdd,              // *Dst
  &MsduLength,           // *MsduLength
  &MpduLinkQuality,      // *MpduLinkQuality
  &DSN,                  // *DSN
  NULLP,                 // *pTimeStamp
  PHYBuffer+4,           // *pMsdu
  NULLP);                // *pSecurity

 if(!status)
  {
  if(DSN == DSN_OLD)      // check if same sequence number - discard if this is the case
   {
   status = MAC_INVALID_HANDLE;
   }
  else
   {
   PHY_TESTRES.PACKET_RECEIVED = 1; // Flag indication
   }
  }

 DSN_OLD = DSN;

 if(!status)
  status = HWME_GET_request_sync(HWME_EDVALLP,  &len, &edvallp);

 if(!status)
  status = HWME_GET_request_sync(HWME_FREQOFFS, &len, &freqoffs);

 PHYBuffer[0] = status;
 PHYBuffer[1] = edvallp;
 PHYBuffer[2] = MpduLinkQuality;
 PHYBuffer[3] = freqoffs;
 PHYLength    = MsduLength;

 return(status);
 } // End of PHY_RXPKT_MAC_indication()
