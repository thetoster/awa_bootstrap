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

#include <string.h>
#include <clock.h>
#include "dev/ca8210/cascoda/include/cascoda_types.h"
#include "dev/ca8210/ports/include/cascoda_bsp.h"
#include "dev/ca8210/cascoda/include/mac_messages.h"
#include "dev/ca8210/cascoda/include/cascoda_spi.h"
#include "dev/ca8210/cascoda/include/cascoda_evbme.h"


/******************************************************************************/
/****** Global Variables for SPI Message Buffers                         ******/
/******************************************************************************/
MAC_Message_t   SPI_Transmit, SPI_Receive;


/******************************************************************************/
/****** Global Variables for SPI Rx Buffer FIFO for saving Messages      ******/
/******************************************************************************/
MAC_Message_t   SPI_Receive_Buffer[SPI_RX_FIFO_SIZE];
u8_t            SPI_Rx_Wr_Ptr = 0;    // Write Pointer
u8_t            SPI_Rx_Rd_Ptr = 0;    // Read Pointer
u8_t            SPI_Rx_Ptr    = 0;    // Buffer Pointer

u8_t            SPI_MLME_SCANNING = 0;


/******************************************************************************/
/******************************************************************************/
/****** SPI_Exchange()                                                   ******/
/******************************************************************************/
/****** Brief:  Exchange Messages across SPI                             ******/
/******************************************************************************/
/****** Param:  pTxBuffer - Pointer to Transmit Buffer or NULLP          ******/
/****** Param:  pRxBuffer - Pointer to Receive Buffer                    ******/
/******************************************************************************/
/****** Return: Status  1 - Tx OK no Rx   (SPIST_TX)                     ******/
/******                 2 - No Tx Rx OK   (SPIST_RX)                     ******/
/******                 3 - Tx Ok Rx OK                                  ******/
/******                 0 - No Rx No Tx                                  ******/
/******              0xF0 - Error                                        ******/
/******************************************************************************/
/******************************************************************************/
u8_t SPI_Exchange( MAC_Message_t *pTxBuffer, MAC_Message_t *pRxBuffer )
 {
 u8_t i, j;
 u8_t TxLeft, RxLeft;
 u8_t OutByte, InByte;
 u8_t Status = 0;
 u32_t T_Start, T_Now;
 u8_t rx_packet_detect = 0;
 u8_t rx_scan_done = 0;

 BSP_SetRFSSBHigh();
 for(i=0;i<100;++i);									  // wait for a couple of us

 if(pRxBuffer->CommandId != SPI_IDLE)                     // save rx buffer in case there's a packet in it
  {
  if(SPI_Push_Rx_Buffer())
   {
   SPI_Error("SPI_Exchange: Rx Push Fail");
   return SPIST_ERR;                                      // can't save rx packet - buffer full
   }
  }

 if (pTxBuffer == NULLP)                                  // nothing to tx
  {
  if(BSP_SenseRFIRQ() != 0)                               // no tx or rx - why was SPI_Exchange() called?
   {
   return 0;
   }
  OutByte = SPI_IDLE;
  TxLeft  = SPI_IDLE;
  Status  = 0;
  }
 else                                                     // tx servicing
  {
  OutByte = pTxBuffer->CommandId;
  TxLeft  = pTxBuffer->Length;
  Status  = SPIST_TX;
  }

 // start access
 BSP_SetRFSSBLow();                                       // start access
 pRxBuffer->CommandId = BSP_SPIExchangeByte( OutByte );   // get the first 2 bytes
 if(pRxBuffer->CommandId != SPI_NACK)
  pRxBuffer->Length    = BSP_SPIExchangeByte( TxLeft );

 T_Now   = 0;
 T_Start = AbsoluteTicks;
 while(( pRxBuffer->CommandId == SPI_NACK) ||             // check for SPI_NACK and backoff if necessary
       ((pRxBuffer->CommandId == SPI_IDLE) && (pRxBuffer->Length == SPI_NACK)))
  {

  BSP_SetRFSSBHigh();

  T_Now = AbsoluteTicks - T_Start;
  if(T_Now > SPI_T_TIMEOUT)                              // timeout for SPI_NACK backoff - something's wrong
   {
   SPI_Error("SPI_Exchange: NACK Timeout");
   T_Start = AbsoluteTicks;
   return SPIST_ERR;
   }

  BSP_WaitTicks(SPI_T_BACKOFF);                          // back off

  BSP_SetRFSSBLow();                                     // try again
  pRxBuffer->CommandId = BSP_SPIExchangeByte( OutByte );
  if(pRxBuffer->CommandId != SPI_NACK)
   pRxBuffer->Length    = BSP_SPIExchangeByte( TxLeft );

  }

 if(pRxBuffer->CommandId == SPI_IDLE)                    // nothing (initially) to rx
  {
  pRxBuffer->Length = SPI_IDLE;
  RxLeft = 0;
  if(TxLeft == SPI_IDLE)                                 // no tx ?
   {
   BSP_SetRFSSBHigh();
   return 0;
   }
  }
 else                                                    // set rx parameters
  {
  RxLeft = pRxBuffer->Length;
  if(TxLeft == SPI_IDLE)
   {
   TxLeft = 0;
   }
  Status |= SPIST_RX;
  }

 j = 0;
 for(i = 0; ((TxLeft != 0) || (RxLeft != 0)); i++)       // data i/o
  {

  if(TxLeft > 0)                                         // assign tx data if available
   {
   OutByte = pTxBuffer->PData.Payload[i];
   TxLeft--;
   }
  else
   {
   OutByte = SPI_IDLE;
   }

  InByte = BSP_SPIExchangeByte( OutByte );               // exchange SPI data

  if(Status & SPIST_RX)                                  // get rx data if available
   {
   pRxBuffer->PData.Payload[j] = InByte;
   ++j;
   if(RxLeft > 0)
    {
    RxLeft--;
    }
   }

  // check for mis-aligned rx packets: 1st rx byte (command) mis-aligned  (MPW Run, MAC_MPW?)
  if(pRxBuffer->CommandId != SPI_IDLE)
   {
   if(( (pRxBuffer->CommandId & 0x80)   ||
      (!(pRxBuffer->CommandId & SPI_S2M)) ) &&
        (!rx_scan_done))
    {
    pRxBuffer->CommandId = pRxBuffer->Length;
    pRxBuffer->Length = InByte;
    RxLeft = pRxBuffer->Length;
    j = 0;
    Status |= SPIST_RX;
    rx_scan_done = 1;
    }
   }

  // check for mis-aligned rx packets: rx packet starting during transmission (Engineering Run, MAC_Workarounds?)
  if(rx_packet_detect == 1)                                  // get rx packet length and re-initialise
   {
   pRxBuffer->Length = InByte;
   RxLeft = pRxBuffer->Length;
   j = 0;
   Status |= SPIST_RX;
   rx_packet_detect = 0;
   }

  if(pRxBuffer->CommandId == SPI_IDLE)
   {

   if(!(InByte & 0x80) &&                                    // get rx command id
      (InByte & SPI_S2M) &&
      (InByte != 0x00) &&
      (!rx_scan_done))
    {
     pRxBuffer->CommandId = InByte;
     rx_packet_detect = 1;
     rx_scan_done = 1;
    }
   }

  }

 BSP_SetRFSSBHigh();                                      // end access

 if (pTxBuffer != NULLP)
  {
  if(pTxBuffer->CommandId == SPI_MLME_SCAN_REQUEST)       // disable SPI sends during scan
   SPI_MLME_SCANNING = 1;
  else if((pRxBuffer->CommandId == SPI_MLME_SCAN_CONFIRM) ||
          (pRxBuffer->CommandId == SPI_HWME_WAKEUP_INDICATION))
   SPI_MLME_SCANNING = 0;                                 // re-enable SPI sends
  }

 if( (pRxBuffer->CommandId & 0x80) ||                    // check if packet is valid
    !(pRxBuffer->CommandId & SPI_S2M) ||
     (pRxBuffer->Length == 0xFF) ||
     (pRxBuffer->Length == 0x00))
  {
  pRxBuffer->CommandId = SPI_IDLE;
  pRxBuffer->Length    = SPI_IDLE;
  Status &= ~SPIST_RX;
  }
 else
  {
  EVBMECheckSPICommand();                                // check if API command requires modification
  }

 return Status;
 } // End of SPI_Exchange()


/******************************************************************************/
/******************************************************************************/
/****** SPI_Send()                                                       ******/
/******************************************************************************/
/****** Brief:  Send Request over SPI                                    ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: 0: Success                                               ******/
/******         1: Error                                                 ******/
/******************************************************************************/
/******************************************************************************/
u8_t SPI_Send( void )
 {
 u8_t  Status;
 u32_t T_Start, T_Now;

 SPI_Receive.CommandId = SPI_IDLE;
 SPI_Receive.Length    = SPI_IDLE;

 if(SPI_MLME_SCANNING == 1)
  {
  SPI_Error("SPI_Send: MLME_SCAN in Progress");
  return(1);
  }

 Status = SPI_Exchange( &SPI_Transmit, &SPI_Receive );   // tx packet
 if(Status == SPIST_ERR)
  {
  SPI_Error("SPI_Send: Send Exchange Fail");
  return 1;
  }

 if((SPI_Transmit.CommandId & SPI_SYN) && (SPI_Transmit.CommandId != SPI_IDLE))
  {

  if((SPI_Receive.CommandId != SPI_IDLE) &&              // catch and save piggy-back asynchronous rx
     (SPI_Receive.CommandId != SPI_NACK))
   {
   if(SPI_Push_Rx_Buffer())
    {
    SPI_Error("SPI_Send: Rx Push Fail");
    return 1;                                            // can't save rx packet - buffer full
    }
   }

  SPI_Receive.CommandId = SPI_IDLE;
  SPI_Receive.Length    = SPI_IDLE;

  T_Now   = 0;
  T_Start = AbsoluteTicks;

  while(1)                                               // wait for synchronous rx packet
   {
   T_Now = AbsoluteTicks - T_Start;                      // timeout for synchronous confirm - something's wrong
   if(T_Now > SPI_T_TIMEOUT)
    {
    SPI_Error("SPI_Send: Synchronous Confirm Timeout");
    return 1;
    }

   if(BSP_SenseRFIRQ() == 0)
    {
    Status = SPI_Exchange( NULLP, &SPI_Receive );
    if(Status == SPIST_ERR)
     {
     SPI_Error("SPI_Send: Synchronous Confirm Exchange Fail");
     return 1;
     }
    break;
    }

   }

  // check synchronous pairing
  if(((SPI_Transmit.CommandId == SPI_MCPS_PURGE_REQUEST)     && (SPI_Receive.CommandId  != SPI_MCPS_PURGE_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_MLME_GET_REQUEST)       && (SPI_Receive.CommandId  != SPI_MLME_GET_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_MLME_RESET_REQUEST)     && (SPI_Receive.CommandId  != SPI_MLME_RESET_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_MLME_RX_ENABLE_REQUEST) && (SPI_Receive.CommandId  != SPI_MLME_RX_ENABLE_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_MLME_SET_REQUEST)       && (SPI_Receive.CommandId  != SPI_MLME_SET_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_MLME_START_REQUEST)     && (SPI_Receive.CommandId  != SPI_MLME_START_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_MLME_POLL_REQUEST)      && (SPI_Receive.CommandId  != SPI_MLME_POLL_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_HWME_SET_REQUEST)       && (SPI_Receive.CommandId  != SPI_HWME_SET_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_HWME_GET_REQUEST)       && (SPI_Receive.CommandId  != SPI_HWME_GET_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_HWME_HAES_REQUEST)      && (SPI_Receive.CommandId  != SPI_HWME_HAES_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_TDME_SETSFR_REQUEST)    && (SPI_Receive.CommandId  != SPI_TDME_SETSFR_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_TDME_GETSFR_REQUEST)    && (SPI_Receive.CommandId  != SPI_TDME_GETSFR_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_TDME_TESTMODE_REQUEST)  && (SPI_Receive.CommandId  != SPI_TDME_TESTMODE_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_TDME_SET_REQUEST)       && (SPI_Receive.CommandId  != SPI_TDME_SET_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_TDME_TXPKT_REQUEST)     && (SPI_Receive.CommandId  != SPI_TDME_TXPKT_CONFIRM)) ||
     ((SPI_Transmit.CommandId == SPI_TDME_LOTLK_REQUEST)     && (SPI_Receive.CommandId  != SPI_TDME_LOTLK_CONFIRM)))
   {
   SPI_Error("SPI_Send: Synchronous Pairing of REQUEST - CONFIRM invalid");
   return 1;
   }

  }

 // note: this causes issues with vitaq runs, but should not be necessary anyhow
 // SPI_Transmit.CommandId = SPI_IDLE;
 // SPI_Transmit.Length    = SPI_IDLE;

 return 0;
 } // End of SPI_Send()


/******************************************************************************/
/******************************************************************************/
/****** SPI_Push_Rx_Buffer()                                             ******/
/******************************************************************************/
/****** Brief:  Pushes Rx Buffer on FIFO                                 ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: 0: Success                                               ******/
/******         1: FIFO Full                                             ******/
/******************************************************************************/
/******************************************************************************/
u8_t SPI_Push_Rx_Buffer( void )
 {

 if(SPI_Receive.CommandId != SPI_IDLE)
  {

  if(SPI_Rx_Ptr >= SPI_RX_FIFO_SIZE)     // buffer overflow
   {
   return 1;
   }

  SPI_Receive_Buffer[SPI_Rx_Wr_Ptr].CommandId = SPI_Receive.CommandId;
  SPI_Receive_Buffer[SPI_Rx_Wr_Ptr].Length    = SPI_Receive.Length;
  memcpy(SPI_Receive_Buffer[SPI_Rx_Wr_Ptr].PData.Payload,
         SPI_Receive.PData.Payload,
         SPI_Receive.Length);

  ++SPI_Rx_Ptr;
  ++SPI_Rx_Wr_Ptr;
  if(SPI_Rx_Wr_Ptr >= SPI_RX_FIFO_SIZE)  // circular buffer
   SPI_Rx_Wr_Ptr = 0;

  }

 SPI_Receive.CommandId = SPI_IDLE;
 return 0;

 } // End of SPI_Push_Rx_Buffer()


/******************************************************************************/
/******************************************************************************/
/****** SPI_Pop_Rx_Buffer()                                              ******/
/******************************************************************************/
/****** Brief:  Pops Rx Buffer from FIFO                                 ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: 0: Success                                               ******/
/******         1: FIFO Full                                             ******/
/******************************************************************************/
/******************************************************************************/
void SPI_Pop_Rx_Buffer( void )
 {

 if(SPI_Rx_Ptr > 0)                      // buffer not empty
  {

  SPI_Receive.CommandId = SPI_Receive_Buffer[SPI_Rx_Rd_Ptr].CommandId;
  SPI_Receive.Length    = SPI_Receive_Buffer[SPI_Rx_Rd_Ptr].Length;
  memcpy(SPI_Receive.PData.Payload,
         SPI_Receive_Buffer[SPI_Rx_Rd_Ptr].PData.Payload,
         SPI_Receive_Buffer[SPI_Rx_Rd_Ptr].Length);

  SPI_Receive_Buffer[SPI_Rx_Rd_Ptr].CommandId = SPI_IDLE;

  --SPI_Rx_Ptr;
  ++SPI_Rx_Rd_Ptr;
  if(SPI_Rx_Rd_Ptr >= SPI_RX_FIFO_SIZE)  // circular buffer
   SPI_Rx_Rd_Ptr = 0;
  }
 else                                   // reset wr + rd pointers if buffer is empty
  {
  SPI_Rx_Wr_Ptr = 0;
  SPI_Rx_Rd_Ptr = 0;
  }

 } // End of SPI_Pop_Rx_Buffer()


/******************************************************************************/
/******************************************************************************/
/****** SPI_Wait()                                                       ******/
/******************************************************************************/
/****** Brief:  Wait for Confirm over SPI                                ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void SPI_Wait( void )
 {

 while ( BSP_SenseRFIRQ() != 0 )
  ;
 SPI_Receive.CommandId = SPI_IDLE;
 SPI_Exchange( NULLP, &SPI_Receive );

 } // End of SPI_Wait()


/******************************************************************************/
/******************************************************************************/
/****** SPI_Flush()                                                      ******/
/******************************************************************************/
/****** Brief:  Flushes SPI                                              ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void SPI_Flush( void )
 {
 u16_t i;

 BSP_SetRFSSBLow();

 for(i=0;i<255+2;++i)
  BSP_SPIExchangeByte(0xFF);

 BSP_SetRFSSBHigh();

 } // End of SPI_Flush()


#include "dev/ca8210/cascoda/include/cascoda_debug.h"

/******************************************************************************/
/******************************************************************************/
/****** SPI_Error()                                                      ******/
/******************************************************************************/
/****** Brief:  SPI Error Message                                        ******/
/******************************************************************************/
/****** Param:  message - Error Message                                  ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void SPI_Error(char *message)
 {

 BSP_SetRFSSBHigh();
 dps("SPI ");
 dp_u32_u(AbsoluteTicks);
 dps("ms: ");
 dps(message);
 dpnl();
// SPI_Flush();
 }
