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
/****** EvaBoard Management Entity (EVBME) API Functions                 ******/
/******************************************************************************/
/******************************************************************************/
/****** Revision           Notes                                         ******/
/******************************************************************************/
/****** 1.0  19/07/14  WB  Release Baseline                              ******/
/******************************************************************************/
/******************************************************************************/
#include "dev/ca8210/cascoda/include/cascoda_types.h"
#include "dev/ca8210/cascoda/include/mac_messages.h"
#include "dev/ca8210/cascoda/include/cascoda_debug.h"
#include "dev/ca8210/cascoda/include/cascoda_serial.h"
#include "dev/ca8210/cascoda/include/cascoda_spi.h"
#include "dev/ca8210/cascoda/include/cascoda_evbme.h"
#include "dev/ca8210/apps/test15_4/include/test15_4_phy_evbme.h"
#include "dev/ca8210/apps/test15_4/include/test15_4_phy_tests.h"


/******************************************************************************/
/******************************************************************************/
/****** TEST15_4_PHY_Handler()                                           ******/
/******************************************************************************/
/****** Brief:  TEST15_4 Event Handler in Main Polling Loop              ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void TEST15_4_PHY_Handler(void)
{
static u8_t oldval = 0;

if(PHY_TESTMODE)
 PHYTestModeHandler();

 // EVBME_SetVal[0] for PHYTestReset()
 if(EVBME_SetVal[0] > 0)
  {
  PHYTestReset(); // reset test modes
  EVBME_SetVal[0] = 0;
  }

 // EVBME_SetVal[1] for PHY_TEST_USE_MAC
 if(EVBME_SetVal[1] != oldval)
  {
  PHYTestCfg(EVBME_SetVal[1]);
  oldval = EVBME_SetVal[1];
  }

} // End of TEST15_4_PHY_Handler()


/******************************************************************************/
/******************************************************************************/
/****** TEST15_4_PHY_UpStreamDispatch()                                  ******/
/******************************************************************************/
/****** Brief:  Dispatch Branch for EVBME Request (UpStream, Serial)     ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void TEST15_4_PHY_UpStreamDispatch(void)
{

if((SERIAL_RX_CMD_ID >= EVBME_PHY_TESTMODE_REQUEST) &&
   (SERIAL_RX_CMD_ID <= EVBME_PHY_REPORT_REQUEST))
 {

    EVBMEIsDownStreamCommand = 0;

    switch ( SERIAL_RX_CMD_ID )
    {
    case EVBME_PHY_TESTMODE_REQUEST:
        if(EVBME_PHY_TESTMODE_request(SERIAL_RX_DATA[0]))
         dps("INVALID TESTMODE\n");
        break;
    case EVBME_PHY_SET_REQUEST:
        if(EVBME_PHY_SET_request(SERIAL_RX_DATA[0], SERIAL_RX_DATA[1], SERIAL_RX_DATA + 2))
         dps("INVALID PARAMETER\n");
        break;
    case EVBME_PHY_REPORT_REQUEST:
        EVBME_PHY_REPORT_request();
        break;
    }

 }

} // End of TEST15_4_PHY_UpStreamDispatch()


/******************************************************************************/
/******************************************************************************/
/****** TEST15_4_PHY_DownStreamDispatch()                                ******/
/******************************************************************************/
/****** Brief:  Dispatch Branch for MAC Commands (DownStream, SPI)       ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void TEST15_4_PHY_DownStreamDispatch(void)
 {

 if(PHY_TESTMODE == PHY_TEST_RX_PER)
  {
  if(SPI_RX_CMD_ID == SPI_TDME_RXPKT_INDICATION)
   {
   PHY_RXPKT_indication();
   EVBMESetIsUpStreamCommand(0);
   }
  }
 else if((PHY_TESTMODE == PHY_TEST_RX_PSN) && (!PHY_TEST_USE_MAC))
  {
  if(SPI_RX_CMD_ID == SPI_TDME_RXPKT_INDICATION)
   {
   PHY_RXPKT_indication();
   EVBMESetIsUpStreamCommand(0);
   }
  }
 else if((PHY_TESTMODE == PHY_TEST_RX_PSN) && (PHY_TEST_USE_MAC))
  {
  if(SPI_RX_CMD_ID == SPI_MCPS_DATA_INDICATION)
   {
   PHY_RXPKT_MAC_indication();
   EVBMESetIsUpStreamCommand(0);
   }
  }
 else if(PHY_TESTMODE == PHY_TEST_RX_EDSN)
  {
  if(SPI_RX_CMD_ID == SPI_TDME_EDDET_INDICATION)
   {
   PHY_EDDET_indication();
   EVBMESetIsUpStreamCommand(0);
   }
  }
 else if((PHY_TESTMODE == PHY_TEST_TX_PKT) && (PHY_TEST_USE_MAC))
  {
  if(SPI_RX_CMD_ID == SPI_MCPS_DATA_CONFIRM)
   {
   EVBMESetIsUpStreamCommand(0);
   }
  }

 } // End of TEST15_4_PHY_DownStreamDispatch()


/******************************************************************************/
/******************************************************************************/
/****** EVBME_PHY_TESTMODE_request()                                     ******/
/******************************************************************************/
/****** Brief:  EVBME_PHY_TESTMODE_request according to EVBME Spec       ******/
/******************************************************************************/
/****** Param:  TestMode - Test Mode                                     ******/
/******************************************************************************/
/****** Return: EVBME Status                                             ******/
/******************************************************************************/
/******************************************************************************/
u8_t EVBME_PHY_TESTMODE_request(u8_t TestMode)
 {

 PHY_TESTSEQUENCECOUNT = 0;

 if(TestMode > PHY_TEST_MAX)
  {
  PHY_TESTMODE = PHY_TEST_OFF;
  return(EVBME_INVALID);
  }

 PHY_TESTMODE = TestMode + PHY_TEST_INIT;

 return(EVBME_SUCCESS);
 } // End of EVBME_PHY_TESTMODE_request()


/******************************************************************************/
/******************************************************************************/
/****** EVBME_PHY_SET_request()                                          ******/
/******************************************************************************/
/****** Brief:  EVBME_PHY_SET_request according to EVBME Spec            ******/
/******************************************************************************/
/****** Param:  Parameter - Parameter Specifier                          ******/
/****** Param:  ParameterLength - Parameter Length                       ******/
/****** Param:  ParameterValue - Pointer to Parameter Value              ******/
/******************************************************************************/
/****** Return: EVBME Status                                             ******/
/******************************************************************************/
/******************************************************************************/
u8_t EVBME_PHY_SET_request(u8_t Parameter, u8_t ParameterLength, u8_t *ParameterValue)
 {
 u8_t status;

 PHY_TESTSEQUENCECOUNT = 0;

 switch(Parameter)
  {

  case PHY_TESTPAR_PACKETPERIOD:
   if(ParameterLength == 2)
    {
    PHY_TESTPAR.PACKETPERIOD = ((u16_t)(ParameterValue[1]) << 8) +
                                (u16_t)(ParameterValue[0]);

    PHYTestReportTestParameters(PHY_TESTPAR_PACKETPERIOD);
    status = EVBME_SUCCESS;
    }
   else
    status = EVBME_INVALID;
   break;

  case PHY_TESTPAR_PACKETLENGTH:
   if(ParameterValue[0] < 128)
    {
    PHY_TESTPAR.PACKETLENGTH = ParameterValue[0];
    PHYTestReportTestParameters(PHY_TESTPAR_PACKETLENGTH);
    status = EVBME_SUCCESS;
    }
   else
    status = EVBME_INVALID;
   break;

  case PHY_TESTPAR_NUMBEROFPKTS:
   if(ParameterLength == 4)
    {
    PHY_TESTPAR.NUMBEROFPKTS = ((u32_t)(ParameterValue[3]) << 24) +
                               ((u32_t)(ParameterValue[2]) << 16) +
                               ((u32_t)(ParameterValue[1]) <<  8) +
                                (u32_t)(ParameterValue[0]);
    PHYTestReportTestParameters(PHY_TESTPAR_NUMBEROFPKTS);
    status = EVBME_SUCCESS;
    }
   else
    status = EVBME_INVALID;
   break;

  case PHY_TESTPAR_PACKETDATATYPE:
   if(ParameterValue[0] <= TDME_MAX_TXD)
    {
    PHY_TESTPAR.PACKETDATATYPE = ParameterValue[0];
    PHYTestReportTestParameters(PHY_TESTPAR_PACKETDATATYPE);
    status = EVBME_SUCCESS;
    }
   else
    status = EVBME_INVALID;
   break;

  case PHY_TESTPAR_CHANNEL:
   if((ParameterValue[0] >= 11) &&
      (ParameterValue[0] <= 26))
    {
    PHY_TESTPAR.CHANNEL = ParameterValue[0];
    if(PHY_TESTMODE & PHY_TEST_CMD) // update TDME-PIB
     {
     PHY_TEST_REINIT_TDME_ATTRIBUTE = TDME_CHANNEL;
     PHY_TESTMODE |= PHY_TEST_REINIT;
     }
    PHYTestReportTestParameters(PHY_TESTPAR_CHANNEL);
    status = EVBME_SUCCESS;
    }
   else
    status = EVBME_INVALID;
   break;

  case PHY_TESTPAR_TXPOWER:
   if((ParameterLength == 3) &&
      (ParameterValue[0] <= 63) &&
      (ParameterValue[1] <=  7) &&
      (ParameterValue[2] <=  1))
    {
    PHY_TESTPAR.TXPOWER_IB    = ParameterValue[0];
    PHY_TESTPAR.TXPOWER_PB    = ParameterValue[1];
    PHY_TESTPAR.TXPOWER_BOOST = ParameterValue[2];
    if(PHY_TESTMODE & PHY_TEST_CMD) // update TDME-PIB
     {
     PHY_TEST_REINIT_TDME_ATTRIBUTE = TDME_TX_CONFIG;
     PHY_TESTMODE |= PHY_TEST_REINIT;
     }
    PHYTestReportTestParameters(PHY_TESTPAR_TXPOWER);
    status = EVBME_SUCCESS;
    }
   else
    status = EVBME_INVALID;
   break;

  case PHY_TESTPAR_EDTHRESHOLD:
   if(ParameterLength == 1)
    {
    PHY_TESTPAR.EDTHRESHOLD = ParameterValue[0];
    if(PHY_TESTMODE & PHY_TEST_CMD) // update TDME-PIB
     {
     PHY_TEST_REINIT_TDME_ATTRIBUTE = TDME_ED_CONFIG;
     PHY_TESTMODE |= PHY_TEST_REINIT;
     }
    PHYTestReportTestParameters(PHY_TESTPAR_EDTHRESHOLD);
    status = EVBME_SUCCESS;
    }
   else
    status = EVBME_INVALID;
   break;

  case PHY_TESTPAR_RX_FFSYNC:
   if(ParameterValue[0] <= 1)
    {
    PHY_TESTPAR.RX_FFSYNC = ParameterValue[0];
    if(PHY_TESTMODE & PHY_TEST_CMD) // update TDME-PIB
     {
     PHY_TEST_REINIT_TDME_ATTRIBUTE = TDME_RX_CONFIG;
     PHY_TESTMODE |= PHY_TEST_REINIT;
     }
    PHYTestReportTestParameters(PHY_TESTPAR_RX_FFSYNC);
    status = EVBME_SUCCESS;
    }
   else
    status = EVBME_INVALID;
   break;

  case PHY_TESTPAR_LO_1_RXTXB:
   if(ParameterValue[0] <= 1)
    {
    PHY_TESTPAR.LO_1_RXTXB = ParameterValue[0];
    if(PHY_TESTMODE & PHY_TEST_CMD) // update TDME-PIB
     {
     PHY_TEST_REINIT_TDME_ATTRIBUTE = TDME_LO_1_CONFIG;
     PHY_TESTMODE |= PHY_TEST_REINIT;
     }
    PHYTestReportTestParameters(PHY_TESTPAR_LO_1_RXTXB);
    status = EVBME_SUCCESS;
    }
   else
    status = EVBME_INVALID;
   break;

  case PHY_TESTPAR_LO_2_FDAC:
   if(ParameterValue[0] <= 48)
    {
    PHY_TESTPAR.LO_2_FDAC = ParameterValue[0];
    if(PHY_TESTMODE & PHY_TEST_CMD) // update TDME-PIB
     {
     PHY_TEST_REINIT_TDME_ATTRIBUTE = TDME_LO_2_CONFIG;
     PHY_TESTMODE |= PHY_TEST_REINIT;
     }
    PHYTestReportTestParameters(PHY_TESTPAR_LO_2_FDAC);
    status = EVBME_SUCCESS;
    }
   else
    status = EVBME_INVALID;
   break;

  case PHY_TESTPAR_LO_3_LOCKS:
   if(ParameterLength == 1)
    {
    PHY_TESTPAR.LO_3_LOCKS = ParameterValue[0];
    PHYTestReportTestParameters(PHY_TESTPAR_LO_3_LOCKS);
    status = EVBME_SUCCESS;
    }
   else
    status = EVBME_INVALID;
   break;

  case PHY_TESTPAR_LO_3_PERIOD:
   if(ParameterLength == 1)
    {
    PHY_TESTPAR.LO_3_PERIOD = ParameterValue[0];
    PHYTestReportTestParameters(PHY_TESTPAR_LO_3_PERIOD);
    status = EVBME_SUCCESS;
    }
   else
    status = EVBME_INVALID;
   break;

  case PHY_TESTPAR_ATM:
   if(ParameterValue[0] <= 31)
    {
    PHY_TESTPAR.ATM = ParameterValue[0];
    if(PHY_TESTMODE & PHY_TEST_CMD) // update TDME-PIB
     {
     PHY_TEST_REINIT_TDME_ATTRIBUTE = TDME_ATM_CONFIG;
     PHY_TESTMODE |= PHY_TEST_REINIT;
     }
    PHYTestReportTestParameters(PHY_TESTPAR_ATM);
    status = EVBME_SUCCESS;
    }
   else
    status = EVBME_INVALID;
   break;

  case PHY_TESTPAR_MPW2_OVWR:
   if(ParameterValue[0] <= 3)
    {
    PHY_TESTPAR.MPW2_OVWR = ParameterValue[0];
    if(PHY_TESTMODE & PHY_TEST_CMD) // update TDME-PIB
     {
     PHY_TEST_REINIT_TDME_ATTRIBUTE = TDME_MPW2_OVWR;
     PHY_TESTMODE |= PHY_TEST_REINIT;
     }
    PHYTestReportTestParameters(PHY_TESTPAR_MPW2_OVWR);
    status = EVBME_SUCCESS;
    }
   else
    status = EVBME_INVALID;
   break;

  default:
    status = EVBME_INVALID;
   break;

  }

 return(status);
 } // End of EVBME_PHY_SET_request()


/******************************************************************************/
/******************************************************************************/
/****** EVBME_PHY_REPORT_request()                                       ******/
/******************************************************************************/
/****** Brief:  EVBME_PHY_REPORT_request according to EVBME Spec         ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void EVBME_PHY_REPORT_request(void)
 {

 PHYTestReportTestParameters(PHY_TESTPAR_ALL);

 } // End of EVBME_PHY_REPORT_request()
