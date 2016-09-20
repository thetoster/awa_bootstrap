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
/****** PHY Test Functions                                               ******/
/******************************************************************************/
/******************************************************************************/
/****** Revision           Notes                                         ******/
/******************************************************************************/
/****** 1.0  19/07/14  WB  Release Baseline                              ******/
/******************************************************************************/
/******************************************************************************/
#include "dev/ca8210/cascoda/include/cascoda_types.h"
#include "dev/ca8210/ports/include/cascoda_bsp.h"
#include "dev/ca8210/cascoda/include/mac_messages.h"
#include "dev/ca8210/cascoda/include/cascoda_debug.h"
#include "dev/ca8210/cascoda/include/cascoda_api.h"
#include "dev/ca8210/apps/test15_4/include/test15_4_phy_tests.h"


/******************************************************************************/
/****** Global Variables used commonly                                   ******/
/******************************************************************************/
u8_t                 PHY_TESTMODE = PHY_TEST_OFF;
u8_t                 PHY_LASTTESTMODE = PHY_TEST_OFF;
u8_t                 PHY_TESTSEQUENCECOUNT;
u8_t                 PHY_TEST_REINIT_TDME_ATTRIBUTE;
u8_t                 PHY_TEST_USE_MAC;
PHYTestPar_t         PHY_TESTPAR;
PHYTestRes_t         PHY_TESTRES;


/******************************************************************************/
/****** Global Variables only used in this File                          ******/
/******************************************************************************/
u16_t                PHY_TEST_COUNTTMP1;
u16_t                PHY_TEST_COUNTTMP2;
u16_t                PHY_TEST_COUNTTMP3;
u8_t                 PHY_TEST_FLAG;

u8_t                 PHYBuffer[256];
u8_t                 PHYLength;


/******************************************************************************/
/******************************************************************************/
/****** PHYTestModeHandler()                                             ******/
/******************************************************************************/
/****** Brief:  Dispatch Branch for PHY Tests including Initialisation   ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestModeHandler(void)
 {
 u8_t status = MAC_SUCCESS;

 if(PHY_TESTMODE & PHY_TEST_INIT)
  {
  if(((PHY_LASTTESTMODE & PHY_TEST_CMD) == PHY_TEST_RX_PSN) &&
     ((PHY_TESTMODE     & PHY_TEST_CMD) != PHY_TEST_RX_PSN)) // Rx packet analysis at end of test
   {
   PHYTestStatistics2();
   PHYTestReportReceivedPacketAnalysis();
   }
  if(((PHY_LASTTESTMODE & PHY_TEST_CMD) == PHY_TEST_TX_PKT) &&
     ((PHY_TESTMODE     & PHY_TEST_CMD) != PHY_TEST_TX_PKT)) // Tx packet analysis at end of test
   {
   PHYTestReportTransmitPacketAnalysis();
   }
  if((status = PHYTestInitialise()))
   PHYTestExit(status);
  }
 else if(PHY_TESTMODE & PHY_TEST_REINIT)
  {
  if((status = PHYTestReinitialise()))
   PHYTestExit(status);
  }
 else if(PHY_TESTMODE == PHY_TEST_OFF)
  {
  PHYTestExit(MAC_SUCCESS);
  }
 else if(PHY_TESTMODE == PHY_TEST_TX_PKT)
  {
  if((status = PHYTestTransmitPacket()))
   PHYTestExit(status);
  }
 else if(PHY_TESTMODE == PHY_TEST_RX_PER)
  {
  PHYTestReceivePacketPER();
  }
 else if(PHY_TESTMODE == PHY_TEST_RX_PSN)
  {
  PHYTestReceivePacketPSN();
  }
 else if(PHY_TESTMODE == PHY_TEST_RX_EDSN)
  {
  PHYTestReceiveED();
  }
 else if(PHY_TESTMODE == PHY_TEST_LO_3)
  {
  if((status = PHYTestLOLocking()))
   PHYTestExit(status);
  }

 // Note: PHY_TEST_TX_CONT, PHY_TEST_LO_1 and PHY_TEST_LO_2 don't need any interaction, only setup

 PHY_LASTTESTMODE = PHY_TESTMODE;

 } // End of PHYTestModeHandler()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestInitialise()                                              ******/
/******************************************************************************/
/****** Brief:  Sequential Initialisation of TDME                        ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
/****** First initialises all TDME Test Attributes to make sure they are ******/
/****** the same as here in the PHY EVBME.                               ******/
/******************************************************************************/
/****** The following steps are sequentially performed:                  ******/
/******  0: Initialise: TDME_SET_request_sync TDME_CHANNEL               ******/
/******  1: Initialise: TDME_SET_request_sync TDME_TX_CONFIG             ******/
/******  2: Initialise: TDME_SET_request_sync TDME_ED_CONFIG             ******/
/******  3: Initialise: TDME_SET_request_sync TDME_RX_CONFIG             ******/
/******  4: Initialise: TDME_SET_request_sync TDME_LO_1_CONFIG           ******/
/******  5: Initialise: TDME_SET_request_sync TDME_LO_2_CONFIG           ******/
/******  6: Initialise: TDME_SET_request_sync TDME_ATM_CONFIG            ******/
/******  7: Initialise: TDME_SET_request_sync TDME_MPW2_OVWR             ******/
/******  8: Set Test Mode with TDME_TESTMODE_request_sync                ******/
/******  9: Finish Initialisation and start Test                         ******/
/******************************************************************************/
/******************************************************************************/
u8_t PHYTestInitialise(void)
 {
 u8_t status = MAC_SUCCESS;

 if(     PHY_TESTSEQUENCECOUNT ==  0)
  {
  BSP_StopTimerUs();
  status = PHY_SET_request(TDME_CHANNEL);
  ++PHY_TESTSEQUENCECOUNT;
  }
 else if(PHY_TESTSEQUENCECOUNT ==  1)
  {
  switch(PHY_TESTMODE & PHY_TEST_CMD)   // set TXCONT according to test mode before TDME_SET_request
   {
   case PHY_TEST_TX_CONT:
   case PHY_TEST_LO_1:
   case PHY_TEST_LO_2:
    PHY_TESTPAR.TXCONT = 1;
    break;
   default:
    PHY_TESTPAR.TXCONT = 0;
    break;
   }
  status = PHY_SET_request(TDME_TX_CONFIG);
  ++PHY_TESTSEQUENCECOUNT;
  }
 else if(PHY_TESTSEQUENCECOUNT ==  2)
  {
  status = PHY_SET_request(TDME_ED_CONFIG);
  ++PHY_TESTSEQUENCECOUNT;
  }
 else if(PHY_TESTSEQUENCECOUNT ==  3)
  {
  status = PHY_SET_request(TDME_RX_CONFIG);
  ++PHY_TESTSEQUENCECOUNT;
  }
 else if(PHY_TESTSEQUENCECOUNT ==  4)
  {
  status = PHY_SET_request(TDME_LO_1_CONFIG);
  ++PHY_TESTSEQUENCECOUNT;
  }
 else if(PHY_TESTSEQUENCECOUNT ==  5)
  {
  status = PHY_SET_request(TDME_LO_2_CONFIG);
  ++PHY_TESTSEQUENCECOUNT;
  }
 else if(PHY_TESTSEQUENCECOUNT ==  6)
  {
  status = PHY_SET_request(TDME_ATM_CONFIG);
  ++PHY_TESTSEQUENCECOUNT;
  }
 else if(PHY_TESTSEQUENCECOUNT ==  7)
  {
  if(MAC_MPW)
   status = PHY_SET_request(TDME_MPW2_OVWR);
  else
   status = MAC_SUCCESS;
  ++PHY_TESTSEQUENCECOUNT;
  }
 else if(PHY_TESTSEQUENCECOUNT ==  8)
  {
  status = PHY_TESTMODE_request();
  ++PHY_TESTSEQUENCECOUNT;
  }
 else
  {
  status = MAC_SUCCESS;
  PHY_TESTMODE &= ~PHY_TEST_INIT;
  PHY_TESTSEQUENCECOUNT = 0;
  PHYTestInitTestResults();
  if(((PHY_TESTMODE     & PHY_TEST_CMD) == PHY_TEST_TX_PKT) && PHY_TEST_USE_MAC)
   status = PHYTestMACTxInitialise();
  if(((PHY_TESTMODE     & PHY_TEST_CMD) == PHY_TEST_RX_PSN) && PHY_TEST_USE_MAC)
   status = PHYTestMACRxInitialise();
  PHYTestReportTestMode();
  if((PHY_TESTMODE == PHY_TEST_TX_PKT) ||
     (PHY_TESTMODE == PHY_TEST_RX_PSN) ||
     (PHY_TESTMODE == PHY_TEST_LO_3))
   {
   BSP_StopTimerUs();
   BSP_StartTimerUs();
   }
  if((PHY_TESTMODE & PHY_TEST_CMD) == PHY_TEST_LO_3)
   {
   PHY_TESTRES.TEST_RUNNING = 1;
   PHY_TEST_COUNTTMP1 =  0; // nr of locking tests
   PHY_TEST_COUNTTMP2 = 11; // channel
   PHY_TEST_COUNTTMP3 =  0; // rx_txb
   }
  }

 return(status);
 } // End of PHYTestInitialise()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReinitialise()                                            ******/
/******************************************************************************/
/****** Brief:  Test Parameter Change and TDME Update                    ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
/****** Reinitialises specific TDME Attribute by TDME-SET if the PHY     ******/
/****** EVBME PIB has been changed by EVBME-PHY_SET                      ******/
/******************************************************************************/
/****** The following steps are sequentially performed:                  ******/
/******  0: Initialise: TDME_SET_request_sync attribute                  ******/
/******  1: Finish reinitialisation                                      ******/
/******************************************************************************/
u8_t PHYTestReinitialise(void)
 {
 u8_t status = MAC_SUCCESS;

 if(     PHY_TESTSEQUENCECOUNT ==  0)
  {
  status = PHY_SET_request(PHY_TEST_REINIT_TDME_ATTRIBUTE);
  ++PHY_TESTSEQUENCECOUNT;
  }
 else
  {
  PHY_TESTMODE &= ~PHY_TEST_REINIT;
  PHY_TESTSEQUENCECOUNT = 0;
  status = MAC_SUCCESS;
  }

 return(status);
 } // End of PHYTestReinitialise()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestTransmitPacket()                                          ******/
/******************************************************************************/
/****** Brief:  PHY Test Transmit Packet                                 ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
/****** The following steps are sequentially performed:                  ******/
/******  0: Wait for Timeout and call TDME_TXPKT_request_sync()          ******/
/******  1: Report Transmission                                          ******/
/******************************************************************************/
u8_t PHYTestTransmitPacket(void)
 {
 u8_t status = MAC_SUCCESS;

 if((PHY_TESTMODE == PHY_TEST_TX_PKT) && (PHY_TESTRES.PACKET_COUNT == 0))
  PHY_TESTRES.TEST_RUNNING = 1;

 if(PHY_TESTRES.TEST_RUNNING)
  {

  if(BSP_GetTimerUs() > 1000*PHY_TESTPAR.PACKETPERIOD)
   {

   BSP_StopTimerUs();
   BSP_StartTimerUs();

   if(PHY_TEST_USE_MAC)
    status = PHY_TXPKT_MAC_request();
   else
    status = PHY_TXPKT_request();
   if(status == TDME_LO_ERROR)
    status = MAC_SUCCESS;

   ++PHY_TESTRES.PACKET_COUNT;
   ++PHY_TEST_COUNTTMP1;

   // report detailed packet information if packet period 500 ms or more
   if((PHY_TESTPAR.PACKETPERIOD >= 500) ||
      ((PHY_TESTPAR.NUMBEROFPKTS <= 100) && (PHY_TESTPAR.NUMBEROFPKTS != 0)))
    {
    PHYTestReportPacketTransmitted();
    }
   // report statistics every 5 to 12 seconds
   else
    {
    if(((PHY_TESTPAR.PACKETPERIOD >=  250) && (PHY_TEST_COUNTTMP1 >=  20)) ||
       ((PHY_TESTPAR.PACKETPERIOD >=  100) && (PHY_TEST_COUNTTMP1 >=  50)) ||
       ((PHY_TESTPAR.PACKETPERIOD >=   50) && (PHY_TEST_COUNTTMP1 >= 100)) ||
       ((PHY_TESTPAR.PACKETPERIOD >=   25) && (PHY_TEST_COUNTTMP1 >= 200)) ||
       ((PHY_TESTPAR.PACKETPERIOD  <   25) && (PHY_TEST_COUNTTMP1 >= 500)))
     {
     PHYTestReportTransmitPacketAnalysis();
     PHY_TEST_COUNTTMP1 = 0;
     }
    }

   }

  // stop test if number of packets is 100 or less
  if((PHY_TESTRES.PACKET_COUNT >= PHY_TESTPAR.NUMBEROFPKTS) &&
     (PHY_TESTPAR.NUMBEROFPKTS <= 100) &&
     (PHY_TESTPAR.NUMBEROFPKTS != 0))
   {
   PHY_TESTRES.TEST_RUNNING = 0;
   }

  }

 return(status);
 } // End of PHYTestTransmitPacket()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReceivePacketPER()                                        ******/
/******************************************************************************/
/****** Brief:  PHY Test Receive Packet in PER Mode (Packet Error Rate)  ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestReceivePacketPER(void)
 {
 u32_t tper;
 u8_t missed_packet;

 tper = BSP_GetTimerUs();

 if((((tper > 1000*PHY_TESTPAR.PACKETPERIOD) &&  PHY_TEST_FLAG) ||   // 1.0 * PACKETPERIOD when previous packet was missed
     ((tper > 1500*PHY_TESTPAR.PACKETPERIOD) && ~PHY_TEST_FLAG)) &&  // 1.5 * PACKETPERIOD when previous packet was received
      (PHY_TESTPAR.PACKETPERIOD > 0) &&
       PHY_TESTRES.TEST_RUNNING)                                     // missed packet
  missed_packet = 1;
 else
  missed_packet = 0;

 if(PHY_TESTRES.PACKET_RECEIVED || missed_packet)
  {

  if((PHY_TESTMODE == PHY_TEST_RX_PER) && (PHY_TESTRES.PACKET_COUNT == 0))
   PHY_TESTRES.TEST_RUNNING = 1;

  if(PHY_TESTRES.TEST_RUNNING)
   {
   BSP_StopTimerUs();
   BSP_StartTimerUs();
   }

  if(missed_packet)
   {
   ++PHY_TESTRES.PREERR_COUNT;
   PHY_TEST_FLAG = 1; // last packet missed
   missed_packet = 0;
   }

  if(PHY_TESTRES.PACKET_RECEIVED)
   {
   PHYTestStatistics1();
   PHY_TEST_FLAG = 0; // last packet received
   if(PHY_TESTPAR.PACKETPERIOD >= 500)
    PHYTestReportPacketReceived();
   }

  if(PHY_TESTRES.PACKET_COUNT <= PHY_TESTPAR.NUMBEROFPKTS)
   {
   ++PHY_TESTRES.PACKET_COUNT;
   ++PHY_TEST_COUNTTMP1;
   }

  if(PHY_TESTRES.TEST_RUNNING && (PHY_TESTRES.PACKET_COUNT >= PHY_TESTPAR.NUMBEROFPKTS))
   {
   TDME_TESTMODE_request_sync(TDME_TEST_OFF); // disable receiver at end
   PHYTestStatistics3();
   PHYTestReportPERTestResult();
   PHY_TESTRES.TEST_RUNNING = 0;
   }
  else
   {
   if(((PHY_TESTPAR.PACKETPERIOD >= 1000) && (PHY_TEST_COUNTTMP1 >=   5)) ||
      ((PHY_TESTPAR.PACKETPERIOD >=  250) && (PHY_TEST_COUNTTMP1 >=  20)) ||
      ((PHY_TESTPAR.PACKETPERIOD >=  100) && (PHY_TEST_COUNTTMP1 >=  50)) ||
      ((PHY_TESTPAR.PACKETPERIOD >=   50) && (PHY_TEST_COUNTTMP1 >= 100)) ||
      ((PHY_TESTPAR.PACKETPERIOD >=   25) && (PHY_TEST_COUNTTMP1 >= 200)) ||
      ((PHY_TESTPAR.PACKETPERIOD  <   25) && (PHY_TEST_COUNTTMP1 >= 500)))
    {
    if(PHY_TESTRES.PACKET_RECEIVED)
     PHYTestStatistics2();
    PHYTestReportReceivedPacketAnalysis();
    if(PHY_TESTRES.PACKET_RECEIVED)
     {
     // reset averages
     PHY_TESTRES.FO_AVG = 0;
     PHY_TESTRES.ED_AVG = 0;
     PHY_TESTRES.CS_AVG = 0;
     }
    PHY_TEST_COUNTTMP1 = 0;
    }
   }

  PHY_TESTRES.PACKET_RECEIVED = 0;
  }

 } // End of PHYTestReceivePacketPER()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReceivePacketPSN()                                        ******/
/******************************************************************************/
/****** Brief:  PHY Test Receive Packet in in PSN Mode (Packet Sniffer)  ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestReceivePacketPSN(void)
 {

 if(PHY_TESTRES.PACKET_RECEIVED)
  {

  PHY_TESTRES.PACKET_RECEIVED = 0;

  PHYTestStatistics1();

  ++PHY_TESTRES.PACKET_COUNT;
  ++PHY_TEST_COUNTTMP1;

  if(PHY_TEST_USE_MAC != 2)
   PHYTestReportPacketReceived();
  }
 else if(BSP_GetTimerUs() > 1000*PHY_TEST_REPORT_PERIOD)
  {
  BSP_StopTimerUs();
  BSP_StartTimerUs();

  PHYTestStatistics2();
  //PHYTestReportReceivedPacketAnalysis();

  // reset averages
  PHY_TESTRES.FO_AVG = 0;
  PHY_TESTRES.ED_AVG = 0;
  PHY_TESTRES.CS_AVG = 0;

  PHY_TEST_COUNTTMP1 = 0;
  }

 } // End of PHYTestReceivePacketPSN()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReceiveED()                                               ******/
/******************************************************************************/
/****** Brief:  PHY Test Receive in ED Detect Mode (Energy Sniffer)      ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestReceiveED(void)
 {

 if(PHY_TESTRES.PACKET_RECEIVED)
  {
  PHY_TESTRES.PACKET_RECEIVED = 0;
  ++PHY_TESTRES.PACKET_COUNT;
  PHYTestReportEDReceived();
  }

 } // End of PHYTestReceiveED()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestLOLocking()                                               ******/
/******************************************************************************/
/****** Brief:  PHY Test LO_3 (Locking) Test                             ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
/****** The following steps are sequentially performed:                  ******/
/****** for each CHANNEL and LO_3_LOCKS:                                 ******/
/******  0: Wait for LO_3_PERIOD Timeout and call TDME_LOTLK_request()   ******/
/******  1: Report Test Result                                           ******/
/******************************************************************************/
/****** Parameter Storage:                                               ******/
/****** PHY_TEST_COUNTTMP1:  Nr of Locking Tests                         ******/
/****** PHY_TEST_COUNTTMP2:  Channel                                     ******/
/****** PHY_TEST_COUNTTMP3:  Rx/Txb                                      ******/
/******************************************************************************/
/******************************************************************************/
u8_t PHYTestLOLocking(void)
 {
 u8_t status = MAC_SUCCESS;
 u8_t ntest, channel, rx_txb;

 if(PHY_TESTRES.TEST_RUNNING)
  {

  ntest   = (u8_t)PHY_TEST_COUNTTMP1;
  channel = (u8_t)PHY_TEST_COUNTTMP2;
  rx_txb  = (u8_t)PHY_TEST_COUNTTMP3;

  if(BSP_GetTimerUs() > 1000*PHY_TESTPAR.LO_3_PERIOD)
   {
   BSP_StopTimerUs();
   BSP_StartTimerUs();
   PHY_TESTSEQUENCECOUNT = 1;
   status = PHY_LOTLK_request(channel, rx_txb);
   if(status == TDME_LO_ERROR)
    status = MAC_SUCCESS;
   }
  else
   {
   if(PHY_TESTSEQUENCECOUNT ==  1)
    {
    PHY_TESTSEQUENCECOUNT = 0;
    PHYTestReportLOLocking(ntest);
    if(ntest < PHY_TESTPAR.LO_3_LOCKS-1)
     {
     ++PHY_TEST_COUNTTMP1;
     }
    else
     {
     PHY_TEST_COUNTTMP1 = 0;
     if(channel < 26)
      {
      ++PHY_TEST_COUNTTMP2;
      }
     else
      {
      PHY_TEST_COUNTTMP2 = 11;
      if(rx_txb == 0)
       {
       PHY_TEST_COUNTTMP3 = 1;
       }
      else
       {
       PHY_TEST_COUNTTMP1 = 0;
       PHY_TEST_COUNTTMP2 = 0;
       PHY_TEST_COUNTTMP3 = 0;
       PHY_TESTRES.TEST_RUNNING = 0;
       }
      }
     }
    status = MAC_SUCCESS;
    }
   else
    {
    PHY_TESTSEQUENCECOUNT = 0;
    status = MAC_SUCCESS;
    }
   }
  }
 else
  {
  PHY_TESTSEQUENCECOUNT = 0;
  status = MAC_SUCCESS;
  }

 return(status);

 } // End of PHYTestLOLocking()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestExit()                                                    ******/
/******************************************************************************/
/****** Brief:  PHY Test Mode Exit                                       ******/
/******************************************************************************/
/****** Param:  status - Status to be reported                           ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestExit(u8_t status)
 {
 PHY_TESTMODE = PHY_TEST_OFF;
 PHY_TESTSEQUENCECOUNT = 0;
 PHYTestReportExit(status);
 } // End of PHYTestExit()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestInitTestResults()                                         ******/
/******************************************************************************/
/****** Brief:  PHY Test Initialise Results and Run Parameters           ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestInitTestResults(void)
 {

 PHY_TESTRES.SEQUENCENUMBER  = 0;
 PHY_TESTRES.PACKET_COUNT    = 0;
 PHY_TESTRES.PACKET_RECEIVED = 0;
 PHY_TESTRES.TEST_RUNNING    = 0;
 PHY_TESTRES.LOERR_COUNT     = 0;
 PHY_TESTRES.CRCERR_COUNT    = 0;
 PHY_TESTRES.PHRERR_COUNT    = 0;
 PHY_TESTRES.SHRERR_COUNT    = 0;
 PHY_TESTRES.PREERR_COUNT    = 0;
 PHY_TESTRES.FO_AVG          = 0;
 PHY_TESTRES.ED_AVG          = 0;
 PHY_TESTRES.CS_AVG          = 0;
 PHY_TESTRES.FO_AVG_TOTAL    = 0;
 PHY_TESTRES.ED_AVG_TOTAL    = 0;
 PHY_TESTRES.CS_AVG_TOTAL    = 0;
 PHY_TESTRES.ED_MAX          = 0;
 PHY_TESTRES.ED_MIN          = 255;
 PHY_TESTRES.CS_MAX          = 0;
 PHY_TESTRES.CS_MIN          = 255;

 PHY_TEST_COUNTTMP1          = 0;
 PHY_TEST_COUNTTMP2          = 0;
 PHY_TEST_COUNTTMP3          = 0;
 PHY_TEST_FLAG               = 0;

 } // End of PHYTestInitTestResults()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReset()                                                   ******/
/******************************************************************************/
/****** Brief:  PHY Test Reset Testmode PIB                              ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestReset(void)
 {
 PHY_TESTMODE           = PHY_TEST_OFF;
 PHY_TESTSEQUENCECOUNT  = 0;
 PHY_TEST_USE_MAC       = 0;

 PHY_TESTPAR.PACKETPERIOD    = PHY_TESTPARDEF_PACKETPERIOD;
 PHY_TESTPAR.PACKETLENGTH    = PHY_TESTPARDEF_PACKETLENGTH;
 PHY_TESTPAR.NUMBEROFPKTS    = PHY_TESTPARDEF_NUMBEROFPKTS;
 PHY_TESTPAR.PACKETDATATYPE  = PHY_TESTPARDEF_PACKETDATATYPE;
 PHY_TESTPAR.CHANNEL         = PHY_TESTPARDEF_CHANNEL;
 PHY_TESTPAR.TXPOWER_IB      = PHY_TESTPARDEF_TXPOWER_IB;
 PHY_TESTPAR.TXPOWER_PB      = PHY_TESTPARDEF_TXPOWER_PB;
 PHY_TESTPAR.TXPOWER_BOOST   = PHY_TESTPARDEF_TXPOWER_BOOST;
 PHY_TESTPAR.TXCONT          = PHY_TESTPARDEF_TXCONT;
 PHY_TESTPAR.EDTHRESHOLD     = PHY_TESTPARDEF_EDTHRESHOLD;
 PHY_TESTPAR.RX_FFSYNC       = PHY_TESTPARDEF_RX_FFSYNC;
 PHY_TESTPAR.LO_1_RXTXB      = PHY_TESTPARDEF_LO_1_RXTXB;
 PHY_TESTPAR.LO_2_FDAC       = PHY_TESTPARDEF_LO_2_FDAC;
 PHY_TESTPAR.LO_3_LOCKS      = PHY_TESTPARDEF_LO_3_LOCKS;
 PHY_TESTPAR.LO_3_PERIOD     = PHY_TESTPARDEF_LO_3_PERIOD;
 PHY_TESTPAR.ATM             = PHY_TESTPARDEF_ATM;
 PHY_TESTPAR.MPW2_OVWR       = PHY_TESTPARDEF_MPW2_OVWR;
 } // End of PHYTestReset()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestStatistics1()                                             ******/
/******************************************************************************/
/****** Brief:  Calculate Test Statistics for each Packet                ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestStatistics1(void)
 {
 i8_t sv;

 // sums for averages
 if(PHY_TEST_COUNTTMP1 < 256)
  {
  sv         = (i8_t)PHYBuffer[3];
  PHY_TESTRES.FO_AVG += (i16_t)sv;
  PHY_TESTRES.ED_AVG += (u16_t)PHYBuffer[1];
  PHY_TESTRES.CS_AVG += (u16_t)PHYBuffer[2];

  ++PHY_TEST_COUNTTMP2;
  }

 // determine min/max
 if(PHYBuffer[1] > PHY_TESTRES.ED_MAX)
  PHY_TESTRES.ED_MAX = PHYBuffer[1];
 if(PHYBuffer[1] < PHY_TESTRES.ED_MIN)
  PHY_TESTRES.ED_MIN = PHYBuffer[1];
 if(PHYBuffer[2] > PHY_TESTRES.CS_MAX)
  PHY_TESTRES.CS_MAX = PHYBuffer[2];
 if(PHYBuffer[2] < PHY_TESTRES.CS_MIN)
  PHY_TESTRES.CS_MIN = PHYBuffer[2];

 } // End of PHYTestStatistics1()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestStatistics2()                                             ******/
/******************************************************************************/
/****** Brief:  Calculate Test Statistics before each Analysis           ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestStatistics2(void)
 {

 if(PHY_TEST_COUNTTMP2 > 0)
  {

  // determine averages
  PHY_TESTRES.FO_AVG = divs16round(PHY_TESTRES.FO_AVG, (i16_t)PHY_TEST_COUNTTMP2);
  PHY_TESTRES.ED_AVG = divu16round(PHY_TESTRES.ED_AVG, (u16_t)PHY_TEST_COUNTTMP2);
  PHY_TESTRES.CS_AVG = divu16round(PHY_TESTRES.CS_AVG, (u16_t)PHY_TEST_COUNTTMP2);

  // sums for overall averages
  if(PHY_TEST_COUNTTMP3 < 256)
   {
   PHY_TESTRES.FO_AVG_TOTAL += PHY_TESTRES.FO_AVG;
   PHY_TESTRES.ED_AVG_TOTAL += PHY_TESTRES.ED_AVG;
   PHY_TESTRES.CS_AVG_TOTAL += PHY_TESTRES.CS_AVG;
   ++PHY_TEST_COUNTTMP3;
   }

  }

 PHY_TEST_COUNTTMP2 = 0;

 } // End of PHYTestStatistics2()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestStatistics3()                                             ******/
/******************************************************************************/
/****** Brief:  Calculate Test Statistics at End of Test                 ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestStatistics3(void)
 {

 // determine total averages
 if(PHY_TEST_COUNTTMP3 > 0)
  {
  PHY_TESTRES.FO_AVG_TOTAL = divs16round(PHY_TESTRES.FO_AVG_TOTAL, (i16_t)PHY_TEST_COUNTTMP3);
  PHY_TESTRES.ED_AVG_TOTAL = divu16round(PHY_TESTRES.ED_AVG_TOTAL, (u16_t)PHY_TEST_COUNTTMP3);
  PHY_TESTRES.CS_AVG_TOTAL = divu16round(PHY_TESTRES.CS_AVG_TOTAL, (u16_t)PHY_TEST_COUNTTMP3);
  }
 else if(PHY_TEST_COUNTTMP2 > 0)
  {
  PHY_TESTRES.FO_AVG_TOTAL = divs16round(PHY_TESTRES.FO_AVG, (i16_t)PHY_TEST_COUNTTMP2);
  PHY_TESTRES.ED_AVG_TOTAL = divu16round(PHY_TESTRES.ED_AVG, (u16_t)PHY_TEST_COUNTTMP2);
  PHY_TESTRES.CS_AVG_TOTAL = divu16round(PHY_TESTRES.CS_AVG, (u16_t)PHY_TEST_COUNTTMP2);
  }
 else
  {
  PHY_TESTRES.FO_AVG_TOTAL = 0;
  PHY_TESTRES.ED_AVG_TOTAL = 0;
  PHY_TESTRES.CS_AVG_TOTAL = 0;
  }

 } // End of PHYTestStatistics3()


/******************************************************************************/
/******************************************************************************/
/****** divu16round()                                                    ******/
/******************************************************************************/
/****** Brief:  Division of Unsigned 16-Bit Values with Rounding         ******/
/******************************************************************************/
/****** Param:  va - Dividend                                            ******/
/****** Param:  vb - Divisor                                             ******/
/******************************************************************************/
/****** Return: Result                                                   ******/
/******************************************************************************/
/******************************************************************************/
u16_t divu16round(u16_t va, u16_t vb)
 {
 u16_t vr;

 vr    = va  / vb;

 // positive truncated down, check if needs correction
 if((va % vb) > (vb / 2))
  vr += 1;

 return(vr);
 } // End of divu16round()


/******************************************************************************/
/******************************************************************************/
/****** divs16round()                                                    ******/
/******************************************************************************/
/****** Brief:  Division of Signed 16-Bit Values with Rounding           ******/
/******************************************************************************/
/****** Param:  va - Dividend                                            ******/
/****** Param:  vb - Divisor                                             ******/
/******************************************************************************/
/****** Return: Result                                                   ******/
/******************************************************************************/
/******************************************************************************/
i16_t divs16round(i16_t va, i16_t vb)
 {
 i16_t vr;

 vr    = va  / vb;

 if(vr > 0)
  {
  // positive truncated down, check if needs correction
  if((va % vb) > (vb / 2))
   vr += 1;
  }
 else
  {
  // negative truncated up, check if needs correction
  if((va % vb) < (-vb / 2))
   vr -= 1;
  }

 return(vr);
 } // End of divs16round()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReportExit()                                              ******/
/******************************************************************************/
/****** Brief:  Report Testmode Exit                                     ******/
/******************************************************************************/
/****** Param:  status - Status to be reported                           ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestReportExit(u8_t status)
 {

 dps("PHY Testmode Exit. Status: ");
 switch(status)
  {
  case MAC_SUCCESS:                 dps("Success");                       break;
  case MAC_SYSTEM_ERROR:            dps("MAC System Error");              break;
  case MAC_CHANNEL_ACCESS_FAILURE:  dps("MAC Channel Access Failure");    break;
  case MAC_NO_ACK:                  dps("MAC no ACK");                    break;
  default:                          dps("Unknown: 0x"); dp_u8_2h(status); break;
  }
 dpnl();

 } // End of PHYTestReportExit()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReportTestMode()                                          ******/
/******************************************************************************/
/****** Brief:  Report Test Mode after Initialisation is done            ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestReportTestMode(void)
 {

 dps("PHY Testmode initialised for ");

 switch(PHY_TESTMODE & PHY_TEST_CMD)
  {
  case PHY_TEST_OFF:      dps("Normal Mode");          break;
  case PHY_TEST_TX_PKT:   dps("Tx (Packets)");         break;
  case PHY_TEST_TX_CONT:  dps("Tx (Continuous)");      break;
  case PHY_TEST_RX_PER:   dps("Rx (PER Test)");        break;
  case PHY_TEST_RX_PSN:   dps("Rx (Packet Sniffer)");  break;
  case PHY_TEST_RX_EDSN:  dps("Rx (Energy Sniffer)");  break;
  case PHY_TEST_LO_1:     dps("LO1 (Closed Loop)");    break;
  case PHY_TEST_LO_2:     dps("LO2 (Open Loop)");      break;
  case PHY_TEST_LO_3:     dps("LO3 (Locking Test)");   break;
  default:                dps("Unknown Test Mode");    break;
  }
 dpnl();

 } // End of PHYTestReportTestMode()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReportTestParameters()                                    ******/
/******************************************************************************/
/****** Brief:  Report Test Parameter PIB                                ******/
/******************************************************************************/
/****** Param:  parameter - Specific Parameter or PHY_TESTPAR_ALL        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestReportTestParameters(u8_t parameter)
 {

 if(parameter == PHY_TESTPAR_ALL)
  {
  dps("PHY Test Parameter Set:\n");
  }
 if((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_PACKETPERIOD))
  {
  dps("PACKETPERIOD   = "); dp_u16_u(PHY_TESTPAR.PACKETPERIOD); dps(" ms\n");
  }
 if((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_PACKETLENGTH))
  {
  dps("PACKETLENGTH   = "); dp_u8_u(PHY_TESTPAR.PACKETLENGTH); dps(" Bytes\n");
  }
 if((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_NUMBEROFPKTS))
  {
  dps("NUMBEROFPKTS   = "); dp_u32_u(PHY_TESTPAR.NUMBEROFPKTS); dpnl();
  }
 if((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_PACKETDATATYPE))
  {
  dps("PACKETDATATYPE = ");
  switch(PHY_TESTPAR.PACKETDATATYPE)
   {
   case TDME_TXD_RANDOM:    dps("Random");                         break;
   case TDME_TXD_SEQRANDOM: dps("Random with Sequence Number");    break;
   case TDME_TXD_COUNT:     dps("Count (Increment)");              break;
   case TDME_TXD_APPENDED:  dps("Appended");                       break;
   default:                 dps("???");                            break;
   }
  dpnl();
  }
 if((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_CHANNEL))
  {
  dps("CHANNEL        = "); dp_u8_u(PHY_TESTPAR.CHANNEL); dps(" (0x"); dp_u8_2h(PHY_TESTPAR.CHANNEL); dps(")\n");
  }
 if((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_TXPOWER))
  {
  dps("TXPOWER        = IB: "); dp_u8_u(PHY_TESTPAR.TXPOWER_IB);
  dps("; PB: ");                dp_u8_u(PHY_TESTPAR.TXPOWER_PB);
  dps("; BOOST: ");             dp_u8_u(PHY_TESTPAR.TXPOWER_BOOST);
  dpnl();
  }
 if((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_EDTHRESHOLD))
  {
  dps("EDTHRESHOLD    = "); dp_u8_u(PHY_TESTPAR.EDTHRESHOLD); dpnl();
  }
 if((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_RX_FFSYNC))
  {
  dps("RX_FFSYNC      = "); dp_u8_u(PHY_TESTPAR.RX_FFSYNC); dps(" (");
   if(PHY_TESTPAR.RX_FFSYNC == 0)
    dps("Off");
   else
    dps("On");
  dps(")\n");
  }
 if((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_LO_1_RXTXB))
  {
  dps("LO_1_RXTXB     = "); dp_u8_u(PHY_TESTPAR.LO_1_RXTXB); dps(" (");
  if(PHY_TESTPAR.LO_1_RXTXB == 0)
   dps("Tx");
  else
   dps("Rx");
  dps(")\n");
  }
 if((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_LO_2_FDAC))
  {
  dps("LO_2_FDAC      = "); dp_u8_u(PHY_TESTPAR.LO_2_FDAC); dps(" (0x"); dp_u8_2h(PHY_TESTPAR.LO_2_FDAC); dps(")\n");
  }
 if((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_LO_3_LOCKS))
  {
  dps("LO_3_LOCKS     = "); dp_u8_u(PHY_TESTPAR.LO_3_LOCKS); dpnl();
  }
 if((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_LO_3_PERIOD))
  {
  dps("LO_3_PERIOD    = "); dp_u8_u(PHY_TESTPAR.LO_3_PERIOD); dpnl();
  }
 if((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_ATM))
  {
  dps("ATM            = "); dp_u8_u(PHY_TESTPAR.ATM); dps(" (0x"); dp_u8_2h(PHY_TESTPAR.ATM); dps(")\n");
  }
 } // End of PHYTestReportTestParameters()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReportPacketTransmitted()                                 ******/
/******************************************************************************/
/****** Brief:  Report Successful Transmission of Packet                 ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestReportPacketTransmitted(void)
 {
 u8_t i;

 if(PHYBuffer[0] == TDME_LO_ERROR)
  dps("Tx: LO Error");
 else
  {

  dps("Tx: SN="); dp_u8_3u(PHYBuffer[1]); dps(" PL="); dp_u8_3u(PHYLength); dps(":");

  for(i=0;i<PHYLength-2;++i)
   {
   dps(" "); dp_u8_2h(PHYBuffer[i+2]);
   }
  }

 dpnl();

 } // End of PHYTestReportPacketTransmitted()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReportTransmitPacketAnalysis()                            ******/
/******************************************************************************/
/****** Brief:  Report Packet Transmission Analysis                      ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestReportTransmitPacketAnalysis(void)
 {
 dps("Tx: "); dp_u32_u(PHY_TESTRES.PACKET_COUNT); dps(" Packets sent\n");
 if(PHY_TEST_USE_MAC)
  {
  dp_u32_u(PHY_TESTRES.SHRERR_COUNT); dps(" No-Acks; ");
  dp_u32_u(PHY_TESTRES.PHRERR_COUNT); dps(" Channel Access Failures\n");
  }
 } // End of PHYTestReportTransmitPacketAnalysis()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReportPacketReceived()                                    ******/
/******************************************************************************/
/****** Brief:  Report Reception of Packet                               ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestReportPacketReceived(void)
 {
 u8_t i;
 i8_t sv;

 if(PHYBuffer[0] == TDME_LO_ERROR)
  dps("Rx: LO Error");
 else
  {

  dps("Rx: PL="); dp_u8_3u(PHYLength); dps(":");

  for(i=0;i<PHYLength-2;++i)
   {
   dps(" "); dp_u8_2h(PHYBuffer[i+4]);
   }

  if(     PHYBuffer[0] == TDME_FCS_ERROR)
   dps("; CRCErr");
  else if(PHYBuffer[0] == TDME_SHR_ERROR)
   dps("; SHRErr");
  else if(PHYBuffer[0] == TDME_PHR_ERROR)
   dps("; PHRErr");

  sv = (i8_t)PHYBuffer[3];

  dps("; ED: "); dp_u8_u(PHYBuffer[1]); dps("; CS: "); dp_u8_u(PHYBuffer[2]); dps("; FO: "); dp_s8_s(sv);

  }

 dpnl();

 } // End of PHYTestReportPacketReceived()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReportReceivedPacketAnalysis()                            ******/
/******************************************************************************/
/****** Brief:  Report Packet Reception Analysis                         ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestReportReceivedPacketAnalysis(void)
 {
 u32_t errcount_total;

 errcount_total = PHY_TESTRES.CRCERR_COUNT
                + PHY_TESTRES.PREERR_COUNT;

 dps("Rx: "); dp_u32_u(PHY_TESTRES.PACKET_COUNT); dps(" Packets received, "); dp_u32_u(errcount_total); dps(" Errors\n");

 if(PHY_TESTPAR.PACKETPERIOD != 0)
  {
  if(PHY_TESTPAR.RX_FFSYNC)
   {
   dps("Rx Error  Analysis: CRC: "); dp_u32_u(PHY_TESTRES.CRCERR_COUNT);
   dps("; PHR: ");                   dp_u32_u(PHY_TESTRES.PHRERR_COUNT);
   dps("; SHR: ");                   dp_u32_u(PHY_TESTRES.SHRERR_COUNT);
   dps("; PRE: ");                   dp_u32_u(PHY_TESTRES.PREERR_COUNT);
   dpnl();
   }
  else
   {
   dps("Rx Error  Analysis: CRC: "); dp_u32_u(PHY_TESTRES.CRCERR_COUNT);
   dps("; Missed: ");                dp_u32_u(PHY_TESTRES.PREERR_COUNT);
   dpnl();
   }
  }

 if(!PHY_TEST_FLAG) // don't report if missed packet
  {
  dps("Rx Signal Analysis: ED: "); dp_u16_u(PHY_TESTRES.ED_AVG);
  dps("; CS: ");                   dp_u16_u(PHY_TESTRES.CS_AVG);
  dps("; FO: ");                   dp_s16_s(PHY_TESTRES.FO_AVG);
  dpnl();
  }

 } // End of PHYTestReportReceivedPacketAnalysis()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReportPERTestResult()                                     ******/
/******************************************************************************/
/****** Brief:  Report Complete Packet Error Rate (PER) Test Result      ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestReportPERTestResult(void)
 {
 u32_t errcount_total;

 errcount_total = PHY_TESTRES.CRCERR_COUNT
                + PHY_TESTRES.PREERR_COUNT;

 dps("PER Test Result: ");   dp_u32_u(PHY_TESTRES.PACKET_COUNT);
 dps(" Packets received, "); dp_u32_u(errcount_total);
 dps(" Errors\n");

 if(PHY_TESTPAR.RX_FFSYNC)
  {
  dps("PER Error Analysis: CRC: "); dp_u32_u(PHY_TESTRES.CRCERR_COUNT);
  dps("; PHR: ");                   dp_u32_u(PHY_TESTRES.PHRERR_COUNT);
  dps("; SHR: ");                   dp_u32_u(PHY_TESTRES.SHRERR_COUNT);
  dps("; PRE: ");                   dp_u32_u(PHY_TESTRES.PREERR_COUNT);
  dpnl();
  }
 else
  {
  dps("PER Error Analysis: CRC: "); dp_u32_u(PHY_TESTRES.CRCERR_COUNT);
  dps("; Missed: ");                dp_u32_u(PHY_TESTRES.PREERR_COUNT);
  dpnl();
  }

 dps("Rx Overall Averages: ED: "); dp_u16_u(PHY_TESTRES.ED_AVG_TOTAL);
 dps("; CS: ");                    dp_u16_u(PHY_TESTRES.CS_AVG_TOTAL);
 dps("; FO: ");                    dp_s16_s(PHY_TESTRES.FO_AVG_TOTAL);
 dpnl();

 dps("Rx Min/Max Analysis: ED Max: "); dp_u8_u(PHY_TESTRES.ED_MAX);
 dps("; ED Min: ");                    dp_u8_u(PHY_TESTRES.ED_MIN);
 dps("; CS Max: ");                    dp_u8_u(PHY_TESTRES.CS_MAX);
 dps("; CS Min: ");                    dp_u8_u(PHY_TESTRES.CS_MIN);
 dpnl();

 if(PHY_TESTPAR.RX_FFSYNC)
  {
  dps("Rx Missed Pkt Analysis: SHR: "); dp_u32_u(PHY_TESTRES.SHRERR_COUNT);
  dps("; PHR: ");                       dp_u32_u(PHY_TESTRES.PHRERR_COUNT);
  dpnl();
  }

 dps("PER Test completed\n");

 } // End of PHYTestReportPERTestResult()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReportEDReceived()                                        ******/
/******************************************************************************/
/****** Brief:  Report Reception of ED above Threshold                   ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestReportEDReceived(void)
 {
 u16_t tat;

 tat = ((u16_t)(PHYBuffer[5]) << 8)
     +  (u16_t)(PHYBuffer[4]);

 dps("ED above "); dp_u8_3u(PHYBuffer[1]); dps(":");
 dps(" ED=");      dp_u8_3u(PHYBuffer[2]);
 dps(" CS=");      dp_u8_3u(PHYBuffer[3]);
 dps(" THigh=");   dp_u16_5u(tat);
 dps(" N=");       dp_u32_u(PHY_TESTRES.PACKET_COUNT);
 dpnl();

 } // End of PHYTestReportEDReceived()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestReportLOLocking()                                         ******/
/******************************************************************************/
/****** Brief:  Report LO Test 3 Result (Locking Test)                   ******/
/******************************************************************************/
/****** Param:  ntest - Number of Test                                   ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestReportLOLocking(u8_t ntest)
 {

 if(PHYBuffer[2])
  dps("Rx");
 else
  dps("Tx");
 dps(" Ch=");    dp_u8_3u(PHYBuffer[1]);
 dps(" N=");     dp_u8_3u(ntest);
 dps(" FDAC=");  dp_u8_3u(PHYBuffer[3]);
 dps(" AMP=");   dp_u8_3u(PHYBuffer[4]);
 if(!PHYBuffer[2])
  {
  dps(" TXCAL="); dp_u8_3u(PHYBuffer[5]);
  }
 if(PHYBuffer[0] == TDME_LO_ERROR)
  dps(" LOCK FAILURE");
 dpnl();

 } // End of PHYTestReportLOLocking()


/******************************************************************************/
/******************************************************************************/
/****** PHYTestCfg()                                                     ******/
/******************************************************************************/
/****** Brief:  PHY Change Test Configuration                            ******/
/******************************************************************************/
/****** Param:  val - Value to set the Configurations to                 ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PHYTestCfg(u8_t val)
 {
 if(val == 0)
  {
  dps("PHYTest configured to PHY only\n");
  PHY_TEST_USE_MAC = 0;
  }
 else if(val == 1)
  {
  dps("PHYTest configured to MAC Usage\n");
  PHY_TEST_USE_MAC = 1;
  }
 else if(val == 2)
  {
  dps("PHYTest configured to MAC Usage (Rx silent)\n");
  PHY_TEST_USE_MAC = 2;
  }
 else
  {
  dps("PHYTest Configuration unknown\n");
  }
 } // End of PHYTestCfg()


/******************************************************************************/
/******************************************************************************/
/****** PHY_TESTMODE_request()                                           ******/
/******************************************************************************/
/****** Brief:  PHY Test Wrapper for TDME_TESTMODE_request_sync          ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t PHY_TESTMODE_request(void)
 {
 u8_t status = MAC_SUCCESS;
 u8_t tdme_testmode;

 switch(PHY_TESTMODE & PHY_TEST_CMD)
  {
  case PHY_TEST_OFF:      tdme_testmode = TDME_TEST_OFF;   break;
  case PHY_TEST_TX_PKT:   tdme_testmode = TDME_TEST_TX;    break;
  case PHY_TEST_TX_CONT:  tdme_testmode = TDME_TEST_TX;    break;
  case PHY_TEST_RX_PER:   tdme_testmode = TDME_TEST_RX;    break;
  case PHY_TEST_RX_PSN:   tdme_testmode = TDME_TEST_RX;    break;
  case PHY_TEST_RX_EDSN:  tdme_testmode = TDME_TEST_ED;    break;
  case PHY_TEST_LO_1:     tdme_testmode = TDME_TEST_LO_1;  break;
  case PHY_TEST_LO_2:     tdme_testmode = TDME_TEST_LO_2;  break;
  case PHY_TEST_LO_3:     tdme_testmode = TDME_TEST_LO_3;  break;
  default:                tdme_testmode = TDME_TEST_OFF;   break;
  }

 status = TDME_TESTMODE_request_sync(tdme_testmode);

 return(status);
 } // End of PHY_TESTMODE_request()


/******************************************************************************/
/******************************************************************************/
/****** PHY_SET_request()                                                ******/
/******************************************************************************/
/****** Brief:  PHY Test Wrapper for TDME_SET_request_sync               ******/
/******************************************************************************/
/****** Param:  attribute - TDME Attribute                               ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t PHY_SET_request(u8_t attribute)
 {
 u8_t status = MAC_SUCCESS;

 if(     attribute == TDME_CHANNEL)
  {
  PHYLength    =  1;
  PHYBuffer[0] =  PHY_TESTPAR.CHANNEL;
  }
 else if(attribute == TDME_TX_CONFIG)
  {
  PHYLength    =  4;
  PHYBuffer[0] =  PHY_TESTPAR.TXPOWER_IB;
  PHYBuffer[1] =  PHY_TESTPAR.TXPOWER_PB;
  PHYBuffer[2] =  PHY_TESTPAR.TXPOWER_BOOST;
  PHYBuffer[3] =  PHY_TESTPAR.TXCONT;
  }
 else if(attribute == TDME_ED_CONFIG)
  {
  PHYLength    =  1;
  PHYBuffer[0] =  PHY_TESTPAR.EDTHRESHOLD;
  }
 else if(attribute == TDME_RX_CONFIG)
  {
  PHYLength    =  1;
  PHYBuffer[0] =  PHY_TESTPAR.RX_FFSYNC;
  }
 else if(attribute == TDME_LO_1_CONFIG)
  {
  PHYLength    =  2;
  PHYBuffer[0] =  PHY_TESTPAR.LO_1_RXTXB;
  PHYBuffer[1] =  PHY_TESTPAR.CHANNEL;
  }
 else if(attribute == TDME_LO_2_CONFIG)
  {
  PHYLength    =  1;
  PHYBuffer[0] =  PHY_TESTPAR.LO_2_FDAC;
  }
 else if(attribute == TDME_ATM_CONFIG)
  {
  PHYLength    =  1;
  PHYBuffer[0] =  PHY_TESTPAR.ATM;
  }
 else if(attribute == TDME_MPW2_OVWR)
  {
  PHYLength    =  1;
  PHYBuffer[0] =  PHY_TESTPAR.MPW2_OVWR;
  }
 else  // ???
  {
  PHYLength    =  1;
  PHYBuffer[0] =  0x00;
  }

 status = TDME_SET_request_sync(attribute, PHYLength, PHYBuffer);

 return(status);
 } // End of PHY_SET_request()


/******************************************************************************/
/******************************************************************************/
/****** PHY_TXPKT_request()                                              ******/
/******************************************************************************/
/****** Brief:  PHY Test Wrapper for TDME_TXPKT_request_sync             ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t PHY_TXPKT_request(void)
 {
 u8_t i;
 u8_t status = MAC_SUCCESS;

 PHYLength    =  PHY_TESTPAR.PACKETLENGTH;
 PHYBuffer[1] =  PHY_TESTRES.SEQUENCENUMBER;

 if(PHY_TESTPAR.PACKETDATATYPE == TDME_TXD_APPENDED)
  {
  for(i=0;i<PHY_TESTPAR.PACKETLENGTH;++i)
   PHYBuffer[i+2] = 0x00; // currently filled with 0's
  }

 status = TDME_TXPKT_request_sync(PHY_TESTPAR.PACKETDATATYPE, &PHYBuffer[1], &PHYLength, PHYBuffer+2);
 PHYBuffer[0] = status;

 ++PHY_TESTRES.SEQUENCENUMBER;

 return(status);
 } // End of PHY_TXPKT_request()


/******************************************************************************/
/******************************************************************************/
/****** PHY_LOTLK_request()                                              ******/
/******************************************************************************/
/****** Brief:  PHY Test Wrapper for TDME_LOTLK_request_sync             ******/
/******************************************************************************/
/****** Param:  ch - 802.15.4 Channel to be tested                       ******/
/****** Param:  rx_txb - rx Mode to be tested when 1, Tx Mode when 0     ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t PHY_LOTLK_request(u8_t ch, u8_t rx_txb)
 {
 u8_t status = MAC_SUCCESS;

 PHYBuffer[1] = ch;
 PHYBuffer[2] = rx_txb;

 status = TDME_LOTLK_request_sync(&PHYBuffer[1], &PHYBuffer[2],  &PHYBuffer[3],  &PHYBuffer[4],  &PHYBuffer[5]);
 PHYBuffer[0] = status;

 return(status);
 } // End of PHY_LOTLK_request()


/******************************************************************************/
/******************************************************************************/
/****** PHY_RXPKT_indication()                                           ******/
/******************************************************************************/
/****** Brief:  PHY Test Wrapper for TDME_RXPKT_indication               ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t PHY_RXPKT_indication(void)
 {
 u8_t status = MAC_SUCCESS;

 status = TDME_RXPKT_indication(&PHYBuffer[1], &PHYBuffer[2], &PHYBuffer[3], &PHYLength, PHYBuffer+4);
 PHYBuffer[0] = status;

 if(     status == TDME_LO_ERROR)
  ++PHY_TESTRES.LOERR_COUNT;
 else if(status == TDME_FCS_ERROR)
  ++PHY_TESTRES.CRCERR_COUNT;
 else if(status == TDME_SHR_ERROR)
  ++PHY_TESTRES.SHRERR_COUNT;
 else if(status == TDME_PHR_ERROR)
  ++PHY_TESTRES.PHRERR_COUNT;

 PHY_TESTRES.PACKET_RECEIVED = 1; // Flag indication

 return(status);
 } // End of PHY_RXPKT_indication()


/******************************************************************************/
/******************************************************************************/
/****** PHY_EDDET_indication()                                           ******/
/******************************************************************************/
/****** Brief:  PHY Test Wrapper for TDME_EDDET_indication               ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t PHY_EDDET_indication(void)
 {
 u8_t status = MAC_SUCCESS;

 status = TDME_EDDET_indication(&PHYBuffer[1], &PHYBuffer[2],  &PHYBuffer[3],  PHYBuffer+4);

 PHY_TESTRES.PACKET_RECEIVED = 1; // Flag indication

 return(status);
 } // End of PHY_EDDET_indication()
