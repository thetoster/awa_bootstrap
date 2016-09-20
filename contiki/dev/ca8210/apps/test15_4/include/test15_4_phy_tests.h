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
/****** PHY Tests Definitions and Function Declarations                  ******/
/******************************************************************************/
/******************************************************************************/
/****** Revision           Notes                                         ******/
/******************************************************************************/
/****** 1.0  19/07/14  WB  Release Baseline                              ******/
/******************************************************************************/
/******************************************************************************/
#include "dev/ca8210/cascoda/include/cascoda_types.h"

#ifndef TEST15_4_PHY_TESTS_H
#define TEST15_4_PHY_TESTS_H


/******************************************************************************/
/****** PHY Testmode Definitions                                         ******/
/******************************************************************************/
// Initialisation Bitmask
#define PHY_TEST_INIT     0x80
// Re-Initialisation Bitmask
#define PHY_TEST_REINIT   0x40
// Testmode Bitmask
#define PHY_TEST_CMD      0x0F
// Testmode Definitions
#define PHY_TEST_OFF      0x00    // Testmode Off
#define PHY_TEST_TX_PKT   0x01    // Transmit Periodic Packets
#define PHY_TEST_TX_CONT  0x02    // Transmit Continuous
#define PHY_TEST_RX_PER   0x03    // Receive PER Test
#define PHY_TEST_RX_PSN   0x04    // Receive Packet Sniffer
#define PHY_TEST_RX_EDSN  0x05    // Receive Energy Detect Sniffer
#define PHY_TEST_LO_1     0x06    // LO Test 1 (Closed Loop, Tx/Rx)
#define PHY_TEST_LO_2     0x07    // LO Test 2 (VCO Open Loop)
#define PHY_TEST_LO_3     0x08    // LO Test 3 (Locking Test)
#define PHY_TEST_MAX      PHY_TEST_LO_3


/******************************************************************************/
/****** PHY Testmode Parameter Enumerations                              ******/
/******************************************************************************/
#define PHY_TESTPAR_PACKETPERIOD    0x00    // Packet Period [ms]
#define PHY_TESTPAR_PACKETLENGTH    0x01    // Packet Length [bytes]
#define PHY_TESTPAR_NUMBEROFPKTS    0x02    // Number of Packets for Test
#define PHY_TESTPAR_PACKETDATATYPE  0x03    // Packet Data Type (Payload)
#define PHY_TESTPAR_CHANNEL         0x04    // TDME-PIB 802.15.4 Channel
#define PHY_TESTPAR_TXPOWER         0x05    // TDME-PIB Transmit Power
#define PHY_TESTPAR_EDTHRESHOLD     0x06    // TDME-PIB ED Threshold
#define PHY_TESTPAR_RX_FFSYNC       0x07    // TDME-PIB Rx Flag False Sync
#define PHY_TESTPAR_LO_1_RXTXB      0x08    // TDME-PIB LO Test 1 Tx/Rx
#define PHY_TESTPAR_LO_2_FDAC       0x09    // TDME-PIB LO Test 2 FDAC Value
#define PHY_TESTPAR_LO_3_LOCKS      0x0A    // LO Test 3 Number of Locks per Tx/Rx Channel
#define PHY_TESTPAR_LO_3_PERIOD     0x0B    // LO Test 3 Locking Test Period [ms]
#define PHY_TESTPAR_ATM             0x0C    // TDME-PIB Analog Test Bus Configuration
#define PHY_TESTPAR_MPW2_OVWR       0x0D    // TDME-PIB MPW2 Overwrite
#define PHY_TESTPAR_MAX             PHY_TESTPAR_MPW2_OVWR
#define PHY_TESTPAR_ALL             (PHY_TESTPAR_MAX+1) // for Reporting


/******************************************************************************/
/****** PHY Testmode Parameter Defaults                                  ******/
/******************************************************************************/
#define PHY_TESTPARDEF_PACKETPERIOD    1000 // 10
#define PHY_TESTPARDEF_PACKETLENGTH    20
#define PHY_TESTPARDEF_PACKETDATATYPE  TDME_TXD_RANDOM
#define PHY_TESTPARDEF_NUMBEROFPKTS    10000
#define PHY_TESTPARDEF_CHANNEL         0x12
#define PHY_TESTPARDEF_TXPOWER_IB      1
#define PHY_TESTPARDEF_TXPOWER_PB      3
#define PHY_TESTPARDEF_TXPOWER_BOOST   0
#define PHY_TESTPARDEF_TXCONT          0
#define PHY_TESTPARDEF_EDTHRESHOLD     0x80
#define PHY_TESTPARDEF_RX_FFSYNC       0
#define PHY_TESTPARDEF_LO_1_RXTXB      0
#define PHY_TESTPARDEF_LO_2_FDAC       32
#define PHY_TESTPARDEF_ATM             0x00
#define PHY_TESTPARDEF_MPW2_OVWR       0x03
#define PHY_TESTPARDEF_LO_3_LOCKS      20
#define PHY_TESTPARDEF_LO_3_PERIOD     10


/******************************************************************************/
/****** PHY Testmode Setup Parameter Structure                           ******/
/******************************************************************************/
typedef struct PHYTestPar
{
    u16_t           PACKETPERIOD;
    u8_t            PACKETLENGTH;
    u32_t           NUMBEROFPKTS;
    u8_t            PACKETDATATYPE;
    u8_t            CHANNEL;
    u8_t            TXPOWER_IB;
    u8_t            TXPOWER_PB;
    u8_t            TXPOWER_BOOST;
    u8_t            TXCONT;
    u8_t            EDTHRESHOLD;
    u8_t            RX_FFSYNC;
    u8_t            LO_1_RXTXB;
    u8_t            LO_2_FDAC;
    u8_t            LO_3_LOCKS;
    u8_t            LO_3_PERIOD;
    u8_t            ATM;
    u8_t            MPW2_OVWR;
} PHYTestPar_t;


/******************************************************************************/
/****** PHY Testmode Results/Runtime Parameter Structure                 ******/
/******************************************************************************/
typedef struct PHYTestRes
{
    u16_t           SEQUENCENUMBER;    // Packet Sequence Number (SN)
    u32_t           PACKET_COUNT;      // Packet Count
    u8_t            PACKET_RECEIVED;   // Packet Received Flag
    u8_t            TEST_RUNNING;      // Test Running Flag
    u32_t           LOERR_COUNT;       // Error Counter for LO Locking Failures
    u32_t           CRCERR_COUNT;      // Error Counter for CRC/FCS Errors
    u32_t           PHRERR_COUNT;      // Error Counter for PHR (Packet Header) Errors
    u32_t           SHRERR_COUNT;      // Error Counter for SHR (SFD) Errors
    u32_t           PREERR_COUNT;      // Error Counter for Preamble Errors / Missed Packets
    i16_t           FO_AVG;            // Averaged Frequency Offset (between Analysis Reports)
    u16_t           ED_AVG;            // Averaged ED Value         (between Analysis Reports)
    u16_t           CS_AVG;            // Averaged CS Value         (between Analysis Reports)
    i16_t           FO_AVG_TOTAL;      // Total Averaged Frequency Offset
    u16_t           ED_AVG_TOTAL;      // Total Averaged ED Value
    u16_t           CS_AVG_TOTAL;      // Total Averaged CS Value
    u8_t            ED_MAX;            // Maximum ED Value
    u8_t            ED_MIN;            // Minimum ED Value
    u8_t            CS_MAX;            // Maximum CS Value
    u8_t            CS_MIN;            // Minimum CS Value
} PHYTestRes_t;


/******************************************************************************/
/****** Locally used Definitions for PHY Tests                           ******/
/******************************************************************************/
#define PHY_TEST_REPORT_PERIOD 5000    // report period in [ms]


/******************************************************************************/
/****** Global Variables defined in test15_4_phy_tests.c                  ******/
/******************************************************************************/
extern u8_t                 PHY_TESTMODE;
extern u8_t                 PHY_LASTTESTMODE;
extern u8_t                 PHY_TESTSEQUENCECOUNT;
extern u8_t                 PHY_TEST_REINIT_TDME_ATTRIBUTE;
extern u8_t                 PHY_TEST_USE_MAC;
extern PHYTestPar_t         PHY_TESTPAR;
extern PHYTestRes_t         PHY_TESTRES;
extern u8_t                 PHY_ERROR_REPORTED;

/******************************************************************************/
/****** Function Declarations for test15_4_phy_tests.c                   ******/
/******************************************************************************/
// PHY Test Functions
void PHYTestModeHandler(void);
u8_t PHYTestInitialise(void);
u8_t PHYTestReinitialise(void);
u8_t PHYTestTransmitPacket(void);
void PHYTestReceivePacketPER(void);
void PHYTestReceivePacketPSN(void);
void PHYTestReceiveED(void);
u8_t PHYTestLOLocking(void);
void PHYTestExit(u8_t status);
void PHYTestInitTestResults(void);
void PHYTestReset(void);
void PHYTestStatistics1(void);
void PHYTestStatistics2(void);
void PHYTestStatistics3(void);
u16_t divu16round(u16_t va, u16_t vb);
i16_t divs16round(i16_t va, i16_t vb);
// PHY Test Report Functions
void PHYTestReportExit(u8_t status);
void PHYTestReportTestMode(void);
void PHYTestReportTestParameters(u8_t parameter);
void PHYTestReportPacketTransmitted(void);
void PHYTestReportTransmitPacketAnalysis(void);
void PHYTestReportPacketReceived(void);
void PHYTestReportReceivedPacketAnalysis(void);
void PHYTestReportPERTestResult(void);
void PHYTestReportEDReceived(void);
void PHYTestReportLOLocking(u8_t ntest);
// PHY Functions in EVBME Attribute Control
void PHYTestCfg(u8_t val);
// PHY Test Wrappers for TDME Commands
u8_t PHY_TESTMODE_request(void);
u8_t PHY_SET_request(u8_t evbme_attribute);
u8_t PHY_TXPKT_request(void);
u8_t PHY_LOTLK_request(u8_t ch, u8_t rx_txb);
u8_t PHY_RXPKT_indication(void);
u8_t PHY_EDDET_indication(void);
u8_t PHY_ERROR_indication(void);
/******************************************************************************/
/****** Function Declarations for test15_4_phy_tests_mac.c               ******/
/******************************************************************************/
// PHY Test Functions
void PHYTestMACAddInit(void);
u8_t PHYTestMACTxInitialise(void);
u8_t PHYTestMACRxInitialise(void);
u8_t PHY_TXPKT_MAC_request(void);
u8_t PHY_RXPKT_MAC_indication(void);


#endif /* TEST15_4_PHY_TESTS_H */
