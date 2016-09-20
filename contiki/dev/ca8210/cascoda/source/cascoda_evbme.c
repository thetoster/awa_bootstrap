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
#include "dev/ca8210/cascoda/include/cascoda_debug.h"
#include "dev/ca8210/cascoda/include/cascoda_serial.h"
#include "dev/ca8210/cascoda/include/cascoda_spi.h"
#include "dev/ca8210/cascoda/include/cascoda_api.h"
#include "dev/ca8210/cascoda/include/cascoda_evbme.h"
#include "dev/ca8210/cascoda/ariadne/include/ariadne.h"

extern u32_t     TimerValue ;

/******************************************************************************/
/****** Local Definitions                                                ******/
/******************************************************************************/
// Wait Time between Power-Up and Initialisation [ms]
#define EVBME_TPUP   200
// Timeout for Wakeup Condition after Reset
#define EVBME_TWUP   50
// Reset Time during Initialisation
#define EVBME_TRESET 50

/******************************************************************************/
/****** Global Parameters that can by set via EVBME_SET_request          ******/
/******************************************************************************/
u8_t EVBME_SetVal[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/******************************************************************************/
/****** Global Parameters for Communications                             ******/
/******************************************************************************/
// Up   Stream Command when set
u8_t EVBMEIsUpStreamCommand = 0;    // Pass-Through SPI -> EVBME -> SERIAL
// Down Stream Command when set
u8_t EVBMEIsDownStreamCommand = 0;  // Pass Through SERIAL -> EVBME -> SPI


/******************************************************************************/
/******************************************************************************/
/****** EVBMEInitialise()                                                ******/
/******************************************************************************/
/****** Brief:  Initialises EVBME after Reset                            ******/
/******************************************************************************/
/****** Param:  version - Version String                                 ******/
/******************************************************************************/
/****** Return: SUCCESS/FAIL                                             ******/
/******************************************************************************/
/******************************************************************************/
u8_t EVBMEInitialise(u8_t *version)
 {
 u8_t status;

 A_MemoryInit();                  // initialise Memory

 BSP_Initialise();                // initialise chip
 BSP_SPIInit();                   // initialise/reset SPI interface and GPIO pins
 SPI_Receive.CommandId = SPI_IDLE;

 EVBME_SetVal[0] = 1;             // initialise/reset PIBs on higher layers

 BSP_ShowTitle(version);          // Show message on the LCD if available

 BSP_WaitTicks(EVBME_TPUP);       // make sure power to RF is fully on

 status = EVBME_Connect(version); // reset and connect RF

 EVBMEIsDownStreamCommand = 0;    // reset comms global variables
 EVBMEIsUpStreamCommand   = 0;

 return(status);
 } // End of EVBMEInitialise()


/******************************************************************************/
/******************************************************************************/
/****** EVBMEHandler()                                                   ******/
/******************************************************************************/
/****** Brief:  TEST15_4 Event Handler                                   ******/
/******         Handles Events in Polling Loop.                          ******/
/******         Checks for SPI Messages being received                   ******/
/******************************************************************************/
/****** Param:  version - Version String                                 ******/
/******************************************************************************/
/****** Return: 0: No SPI Messages received.                             ******/
/******         1: SPI Message received and ready for Dispatch           ******/
/******************************************************************************/
/******************************************************************************/
u8_t EVBMEHandler(void)
 {
 u8_t status = 0;

 BSP_ShowTime();

 if(BSP_SenseRFIRQ() == 0)
  {

  status = SPI_Exchange( NULLP, &SPI_Receive );

  if(status & SPIST_RX)
   {
   status = 1;
   EVBMEIsUpStreamCommand = 1;
   }
  else
   {
   status = 0;
   EVBMEIsUpStreamCommand = 0;
   }

  }
 else
  {

  if(SPI_RX_CMD_ID == SPI_IDLE)  // get spi rx buffer in case there was something stored and buffer is empty
   SPI_Pop_Rx_Buffer();
  if(SPI_RX_CMD_ID != SPI_IDLE)
   {
   status = 1;
   EVBMEIsUpStreamCommand = 1;
   }
  else
   {
   status = 0;
   EVBMEIsUpStreamCommand = 0;
   }

  }

 return(status);
 } // End of EVBMEHandler()


/******************************************************************************/
/******************************************************************************/
/****** EVBMEUpStreamDispatch()                                          ******/
/******************************************************************************/
/****** Brief:  Dispatch Branch for EVBME (UpStream, Serial)             ******/
/******************************************************************************/
/****** Param:  version - Version String                                 ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void EVBMEUpStreamDispatch(u8_t *version)
{
u8_t status;

EVBMEIsDownStreamCommand = 1;

if((SERIAL_RX_CMD_ID == EVBME_SET_REQUEST) ||
   (SERIAL_RX_CMD_ID == EVBME_GUI_CONNECTED) ||
   (SERIAL_RX_CMD_ID == EVBME_GUI_DISCONNECTED))
 {

    EVBMEIsDownStreamCommand = 0;

    switch ( SERIAL_RX_CMD_ID )
    {
    case EVBME_SET_REQUEST:
        status = EVBME_SET_request(SERIAL_RX_DATA[0], SERIAL_RX_DATA[1], SERIAL_RX_DATA + 2);
        if(status == EVBME_INVALID)
         dps("INVALID ATTRIBUTE\n");
        else if(status == EVBME_UNKNOWN)
         dps("UNKNOWN ATTRIBUTE\n");
        break;
    case EVBME_GUI_CONNECTED:
        EVBME_Connect(version);
        break;
    case EVBME_GUI_DISCONNECTED:
        EVBME_Disconnect();
        break;
    case EVBME_MESSAGE_INDICATION:
        break;
    }

  }
 else
  {
  // check if MAC/API straight-through command requires additional action
  EVBMECheckSerialCommand();
  }

} // End of EVBMEUpStreamDispatch()


/******************************************************************************/
/******************************************************************************/
/****** EVBMESendDownStream()                                            ******/
/******************************************************************************/
/****** Brief:  Sends UpStream Command from Serial DownStream to SPI     ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void EVBMESendDownStream()
 {
 u8_t status;

 if(EVBMEIsDownStreamCommand)
  {

  // Command to be sent over SPI
  SPI_TX_CMD_ID  = SERIAL_RX_CMD_ID;
  SPI_TX_CMD_LEN = SERIAL_RX_CMD_LEN;
  memcpy(SPI_TX_DATA, SERIAL_RX_DATA, SPI_TX_CMD_LEN);

//  status = SPI_Exchange( &SPI_Transmit, &SPI_Receive );

//  if(status & SPIST_TX)
//   EVBMEIsDownStreamCommand = 0;

  status = SPI_Send();

  if(status == 0)
   EVBMEIsDownStreamCommand = 0;

  }

 } // End of EVBMESendDownStream()


/******************************************************************************/
/******************************************************************************/
/****** EVBMESendUpStream()                                              ******/
/******************************************************************************/
/****** Brief:  Sends DownStream Command from API UpStream to Serial     ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void EVBMESendUpStream(void)
 {

 if((EVBMEIsUpStreamCommand) &&                               // Move from SPI_Receive to SerialTxBuffer and send
    (SPI_RX_CMD_ID !=  SPI_IDLE) &&
    (SPI_RX_CMD_ID !=  SPI_NACK))
  {
  if(SPI_RX_CMD_ID & SPI_S2M)                                 // Catch some Messages not bound for Upstream
   MAC_Message(SPI_RX_CMD_ID, SPI_RX_CMD_LEN, SPI_RX_DATA);
  SPI_RX_CMD_ID = SPI_IDLE;
  EVBMEIsUpStreamCommand = 0;
  }

 } // End of EVBMESendUpStream()


/******************************************************************************/
/******************************************************************************/
/****** EVBMEForceUpStream()                                             ******/
/******************************************************************************/
/****** Brief:  Forces a received DownStream Command to be sent UpStream ******/
/******************************************************************************/
/****** Param:  cmd:  Command (has been overwritten by sync Req/Cnf)     ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void EVBMEForceUpStream(u8_t cmd)
 {
 EVBMESetIsUpStreamCommand(1);
 SPI_RX_CMD_ID = cmd;
 EVBMESendUpStream();
 EVBMESetIsUpStreamCommand(0);
 } // End of EVBMEForceUpStream()


/******************************************************************************/
/******************************************************************************/
/****** EVBMESetIsUpStreamCommand()                                      ******/
/******************************************************************************/
/****** Brief:  Sets EVBMEIsUpStreamCommand and invalidates Buffer       ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void EVBMESetIsUpStreamCommand(u8_t SendUpStream)
 {

 EVBMEIsUpStreamCommand = SendUpStream;
 if(SendUpStream == 0)           // handled locally
  SPI_RX_CMD_ID = SPI_IDLE;

 } // End of EVBMESetIsUpStreamCommand()


/******************************************************************************/
/******************************************************************************/
/****** EVBMECheckSerialCommand()                                        ******/
/******************************************************************************/
/****** Brief:  Checks if MAC/API Command from Serial requires Action    ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void EVBMECheckSerialCommand(void)
 {
 u8_t status, val;

 if(SERIAL_RX_CMD_ID == SPI_MLME_SET_REQUEST)
  {
  if(TDME_CheckPIBAttribute(SERIAL_RX_DATA[0], SERIAL_RX_DATA[2], SERIAL_RX_DATA + 3))
   {
   EVBMEIsDownStreamCommand = 0;
   SPI_RX_CMD_ID  = SPI_MLME_SET_CONFIRM;
   SPI_RX_CMD_LEN = 3;
   SPI_RX_DATA[0] = MAC_INVALID_PARAMETER;
   SPI_RX_DATA[1] = SERIAL_RX_DATA[0];
   SPI_RX_DATA[2] = SERIAL_RX_DATA[1];
   EVBMESendUpStream();
   }
  }

 // MLME set / get phyTransmitPower
 if((SERIAL_RX_CMD_ID == SPI_MLME_SET_REQUEST) && (SERIAL_RX_DATA[0] == phyTransmitPower))
  {
  status = TDME_SetTxPower(SERIAL_RX_DATA[3]);
  EVBMEIsDownStreamCommand = 0;
  SPI_RX_CMD_ID  = SPI_MLME_SET_CONFIRM;
  SPI_RX_CMD_LEN = 3;
  SPI_RX_DATA[0] = status;
  SPI_RX_DATA[1] = SERIAL_RX_DATA[0];
  SPI_RX_DATA[2] = SERIAL_RX_DATA[1];
  EVBMESendUpStream();
  }
 if((SERIAL_RX_CMD_ID == SPI_MLME_GET_REQUEST) && (SERIAL_RX_DATA[0] == phyTransmitPower))
  {
  status = TDME_GetTxPower(&val);
  EVBMEIsDownStreamCommand = 0;
  SPI_RX_CMD_ID  = SPI_MLME_GET_CONFIRM;
  SPI_RX_CMD_LEN = 5;
  SPI_RX_DATA[0] = status;
  SPI_RX_DATA[1] = SERIAL_RX_DATA[0];
  SPI_RX_DATA[2] = SERIAL_RX_DATA[1];
  SPI_RX_DATA[3] = 1;
  SPI_RX_DATA[4] = val;
  EVBMESendUpStream();
  }

 if(      SERIAL_RX_CMD_ID == SPI_MLME_ASSOCIATE_REQUEST)
  TDME_ChannelInit(SERIAL_RX_DATA[0]);
 else if( SERIAL_RX_CMD_ID == SPI_MLME_START_REQUEST)
  TDME_ChannelInit(SERIAL_RX_DATA[2]);
 else if((SERIAL_RX_CMD_ID == SPI_MLME_SET_REQUEST) && (SERIAL_RX_DATA[0] == phyCurrentChannel))
  TDME_ChannelInit(SERIAL_RX_DATA[3]);
 else if((SERIAL_RX_CMD_ID == SPI_TDME_SET_REQUEST) && (SERIAL_RX_DATA[0] == TDME_CHANNEL))
  TDME_ChannelInit(SERIAL_RX_DATA[2]);
 // reset COORD Bit for Channel Filtering as Coordinator
 else if((MAC_Workarounds) && (SERIAL_RX_CMD_ID == SPI_MLME_RESET_REQUEST) && (SERIAL_RX_DATA[0] == 1))
  TDME_SETSFR_request_sync(0, 0xD8, 0);
 } // End of EVBMECheckSerialCommand()


/******************************************************************************/
/******************************************************************************/
/****** EVBMECheckSPICommand()                                           ******/
/******************************************************************************/
/****** Brief:  Checks if MAC/API Command from SPI requires Action       ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void EVBMECheckSPICommand(void)
 {

 if(MAC_Workarounds)
  {
  if(SPI_RX_CMD_ID == SPI_MCPS_DATA_CONFIRM)  // direct transmission - change malloc fail message
   {
   if(SPI_Receive.PData.DataCnf.Status == MAC_SYSTEM_ERROR)
    {
    SPI_Receive.PData.DataCnf.Status = MAC_TRANSACTION_OVERFLOW;
    }
   }
  }

 } // End of EVBMECheckSPICommand()


/******************************************************************************/
/******************************************************************************/
/****** EVBME_SET_request()                                              ******/
/******************************************************************************/
/****** Brief:  EVBME_SET_request according to EVBME Spec                ******/
/******************************************************************************/
/****** Param:  Attribute - Attribute Specifier                          ******/
/****** Param:  AttributeLength - Attribute Length                       ******/
/****** Param:  AttributeValue - Pointer to Attribute Value              ******/
/******************************************************************************/
/****** Return: EVBME Status                                             ******/
/******************************************************************************/
/******************************************************************************/
u8_t EVBME_SET_request(u8_t Attribute, u8_t AttributeLength, u8_t *AttributeValue )
 {
 u8_t status;

 /* branch for EVBME-SET Attributes */
 switch(Attribute)
  {

  case EVBME_RESETRF:
   if(AttributeLength != 1)
    {
    status = EVBME_INVALID;
    }
   else
    {
    status = EVBME_ResetRF(AttributeValue[0]);
    EVBME_SetVal[0] = AttributeValue[0];
    }
   break;

  case EVBME_CFGPINS:
   if((AttributeLength != 1) ||
      (AttributeValue[0] > 0x03))
    {
    status = EVBME_INVALID;
    }
   else
    {
    EVBME_SetVal[1] = AttributeValue[0];
    status = EVBME_SUCCESS;
    }
   break;

  default:
   status = EVBME_UNKNOWN;         /* what's that ??? */
   break;

  }

 return(status);
 } // End of EVBME_SET_request()


/******************************************************************************/
/******************************************************************************/
/****** EVBME_ResetRF()                                                  ******/
/******************************************************************************/
/****** Brief:  Reset RF device and check connection to RF               ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: SUCCESS/FAIL                                             ******/
/******************************************************************************/
/******************************************************************************/
u8_t EVBME_ResetRF(u8_t ms)
 {
 u32_t startticks, currentticks;
 u8_t command, condition;
 u8_t chipid[2];
 u8_t attlen;
 u8_t status;

 status = EVBME_FAIL;

 command = SPI_Receive.CommandId = SPI_IDLE;

 if(ms > 0)                    // reset RF
  BSP_ResetRF(ms);
 else
  BSP_ResetRF(1);

 startticks = AbsoluteTicks;  // wait for and time-out wake-up indication (EVBME_TWUP)
 do
  {
  currentticks = AbsoluteTicks - startticks;
  } while ((BSP_SenseRFIRQ() != 0) && (currentticks < EVBME_TWUP));

 SPI_Exchange( NULLP, &SPI_Receive );

 command   = SPI_Receive.CommandId;
 condition = SPI_Receive.PData.HWMEWakeupInd.WakeUpCondition;

 if(currentticks >= EVBME_TWUP)
  {
  dps("CA-821X connection timed out, check hardware\n");
  }
 else
  {
  if(command   != SPI_HWME_WAKEUP_INDICATION)
   {
   dps("CA-821X signalled 0x");
   dp_u8_2h(command);
   dps(" instead of wakeup indication, check status\n");
   }
  else
   {
   if(condition != HWME_WAKEUP_POWERUP)
    {
    dps("CA-821X wakeup condition not power-up, check status\n");
    }
   else
    {
    BSP_WaitTicks(1);
    if(TDME_ChipInit())
     {
     dps("CA-821X initialisation failed\n");
     }
    else
     {
     dps("CA-821X connected, ");
     if(HWME_GET_request_sync(HWME_CHIPID, &attlen, chipid))
      {
      dps("ID Failed");
      }
     else
      {
      status = EVBME_SUCCESS;
      dps("V");
      dp_u8_u(chipid[0]);   // product id
      dps(".");
      dp_u8_u(chipid[1]);   // version number
      // MAC_Workarounds for V1.1
      if((chipid[0] == 1) && (chipid[1] == 1))
       MAC_Workarounds = 1;
      else
       MAC_Workarounds = 0;
      // MAC_MPW for V0.x
      if(chipid[0] == 0)
       MAC_MPW = 1;
      else
       MAC_MPW = 0;
      }
     dpnl();
     }
    }
   }
  }

 SPI_Receive.CommandId = SPI_IDLE;

 return(status);
 } // End of EVBME_ResetRF()


/******************************************************************************/
/******************************************************************************/
/****** EVBME_Connect()                                                  ******/
/******************************************************************************/
/****** Brief:  Reset Test Modes, RF and check connection to RF          ******/
/*****          when GUI is connected                                    ******/
/******************************************************************************/
/****** Param:  version - Version String                                 ******/
/******************************************************************************/
/****** Return: SUCCESS/FAIL                                             ******/
/******************************************************************************/
/******************************************************************************/
u8_t EVBME_Connect(u8_t *version)
 {
 u8_t status;

 EVBME_SetVal[0] = 1;         // initialise/reset PIBs on higher layers
 dps("EVBME   connected");
 if(version != NULLP)
  {
  dps(", ");
  dps((char *)version);
  }
 dpnl();

 status = EVBME_ResetRF(EVBME_TRESET);  // reset RF

 return(status);
 } // End of EVBME_Connect()


/******************************************************************************/
/******************************************************************************/
/****** EVBME_Disconnect()                                               ******/
/******************************************************************************/
/****** Brief:  Reset Test Modes and RF                                  ******/
/*****          when GUI is disconnected                                 ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void EVBME_Disconnect(void)
 {
 EVBME_SetVal[0] = 1;
 BSP_ResetRF(EVBME_TRESET);      // reset RF
 } // End of EVBME_Disconnect()


/******************************************************************************/
/******************************************************************************/
/****** EVBME_WaitforResponse()                                          ******/
/******************************************************************************/
/****** Brief:  Wait for MAC Response with Timeout                       ******/
/******************************************************************************/
/****** Param:  command - SPI command to wait for                        ******/
/******         timeout_ms - timeout in [ms]                             ******/
/******************************************************************************/
/****** Return: 0: Success, 1: Fail (Timeout)                            ******/
/******************************************************************************/
/******************************************************************************/
u8_t EVBME_WaitforResponse(u8_t command, u32_t timeout_ms)
 {
 u32_t tstart;

 tstart = BSP_ReadAbsoluteTime();

 do
  {
  SPI_Receive.CommandId = SPI_IDLE;
  if(((BSP_ReadAbsoluteTime() - tstart) > timeout_ms) && (timeout_ms != 0))
   {
   return(1);
   }
  if( BSP_SenseRFIRQ() == 0 )
   SPI_Exchange( NULLP, &SPI_Receive );
 } while ( SPI_Receive.CommandId != command );

 return(0);
 } // End of EVBME_WaitforResponse()


/******************************************************************************/
/******************************************************************************/
/****** EVBME_CompareCommand()                                           ******/
/******************************************************************************/
/****** Brief:  Compare MAC Response with Command                        ******/
/******************************************************************************/
/****** Param:  command - SPI command to wait for                        ******/
/******************************************************************************/
/****** Return: 0: No Match, 1: Match                                    ******/
/******************************************************************************/
/******************************************************************************/
u8_t EVBME_CompareCommand(u8_t command)
 {

 if(SPI_Receive.CommandId == command)
  return(1);

 return(0);
 } // End of EVBME_CompareCommand()


/******************************************************************************/
/******************************************************************************/
/****** EVBME_PowerDown()                                               ******/
/******************************************************************************/
/****** Brief:  Send RF Device into Low-Power Mode                       ******/
/******************************************************************************/
/****** Param:  mode - HWME Low-Power Mode                               ******/
/******         sleeptime_ms - sleep time in [ms]                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void EVBME_PowerDown(u8_t mode, u32_t sleeptime_ms)
 {
 u8_t pparam[5] = {0, 0, 0, 0, 0};

 pparam[0] = mode;
 if(mode & 0x10)  // wake-up on sleep timer
  {
  pparam[1] = LS0_BYTE(sleeptime_ms);
  pparam[2] = LS1_BYTE(sleeptime_ms);
  pparam[3] = LS2_BYTE(sleeptime_ms);
  pparam[4] = LS3_BYTE(sleeptime_ms);
  }

 if(HWME_SET_request_sync(HWME_POWERCON, 5, pparam))
  dps("EVBME Power-Down failed\n");

 } // End of EVBME_PowerDown()


/******************************************************************************/
/******************************************************************************/
/****** EVBME_Wakeup()                                               ******/
/******************************************************************************/
/****** Brief:  Send RF Device into Low-Power Mode                       ******/
/******************************************************************************/
/****** Param:  mode - HWME Low-Power Mode                               ******/
/******         sleeptime_ms - sleep time in [ms]                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void EVBME_Wakeup(u8_t mode)
 {
 u8_t condition;
 u8_t mmode;

 SPI_RX_CMD_ID  = SPI_IDLE;

 // wake up device by gpio by sending dummy spi byte
 if(mode & 0x20)
  {
  SPI_TX_CMD_ID  = SPI_IDLE;
  SPI_TX_CMD_LEN = 0;
  SPI_Send();
  }

 if(SPI_RX_CMD_ID != SPI_HWME_WAKEUP_INDICATION)
  {
  if(EVBME_WaitforResponse(SPI_HWME_WAKEUP_INDICATION, 50))
   dps("EVBME Wakeup timeout\n");
  }

 condition = SPI_Receive.PData.HWMEWakeupInd.WakeUpCondition;

 mmode = mode & 0x0F;

 if((((mmode == 0x04) && (condition != HWME_WAKEUP_STBY_GPIO) && (condition != HWME_WAKEUP_POFF_SLT)) || // standby
     ((mmode == 0x0A) && (condition != HWME_WAKEUP_POFF_GPIO) && (condition != HWME_WAKEUP_POFF_SLT)) || // power-off 0
     ((mmode == 0x0C) && (condition != HWME_WAKEUP_POFF_SLT))) && // power-off 1
                         (condition != HWME_WAKEUP_POWERUP))
  dps("EVBME Wakeup failed\n");

 EVBMESetIsUpStreamCommand(0);

 } // End of EVBME_Wakeup()
