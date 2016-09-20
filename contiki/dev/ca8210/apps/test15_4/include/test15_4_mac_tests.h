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
/****** MAC Tests Definitions/Declarations                               ******/
/******************************************************************************/
/******************************************************************************/
/****** Revision           Notes                                         ******/
/******************************************************************************/
/****** 1.0  19/07/14  WB  Release Baseline                              ******/
/******************************************************************************/
/******************************************************************************/
#include "dev/ca8210/cascoda/include/cascoda_types.h"

#ifndef TEST15_4_MAC_TESTS_H
#define TEST15_4_MAC_TESTS_H


/******************************************************************************/
/****** Global Variables for MAC Tests defined in test15_4_mac_tests.c   ******/
/******************************************************************************/
extern u8_t    MyChannel;
extern u8_t    MyDeviceNumber;
extern u16_t   MyShortAddress;
extern u16_t   MyPANId;
extern u8_t    MyIEEEAddress[8];
extern u16_t   DstShortAddress;
extern u8_t    DstIEEEAddress[8];


/******************************************************************************/
/****** Data from SetupAwaitAssoc() and SetupAwaitOrphan()               ******/
/******************************************************************************/
extern u8_t    AssocDeviceAddress[8];
extern u8_t    OrphanDeviceAddress[8];
extern u8_t    AllZeroes[8];
extern u16_t   StoredAssocShortAddress;
extern u8_t    StoredAssocStatus;
extern u16_t   StoredOrphanShortAddress;


/******************************************************************************/
/****** PIB Test Data                                                    ******/
/******************************************************************************/
extern SecSpec_t     Security;


/******************************************************************************/
/****** Function Declarations for test15_4_mac_tests.c                   ******/
/******************************************************************************/
u8_t InitialiseDevice( u8_t Channel, u16_t PANId, u16_t ShortAddress, u8_t *pExtAddress );
u8_t StartDevice( u8_t Length, u8_t *pBeacon );
void InitSecurity( void );
void PIBTest( void );
void SecPIBTest( void );
void OrphanScanTest( void );
void SimpleTxTest( u8_t TxOpt );
u8_t DataTest0( u8_t i, u8_t TxOpt, u16_t DestShort );
u8_t DataTest1( u8_t i, u8_t TxOpt, u16_t DestShort );
u8_t DataTest( u8_t NoOfRequests, u8_t TxOpt, u16_t DestShort, u8_t Type );
void ScanTest( void );
void AssocNonSleepyTest( void );
void AssocSleepyTest( void );
u8_t AssocTest( u8_t Attribute, u8_t Seconds );
void SetupAwaitAssoc( u8_t *pDeviceAddress, u16_t AssocShortAddress, u8_t Status );
void SetupAwaitOrphan( u8_t *pDeviceAddress, u16_t OrphanShortAddress );


#endif // TEST15_4_MAC_TESTS_H
