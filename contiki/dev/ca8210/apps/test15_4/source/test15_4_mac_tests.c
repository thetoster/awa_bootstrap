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
/****** MAC Test Functions                                               ******/
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
#include "dev/ca8210/cascoda/include/cascoda_debug.h"
#include "dev/ca8210/cascoda/include/cascoda_spi.h"
#include "dev/ca8210/cascoda/include/cascoda_api.h"
#include "dev/ca8210/cascoda/include/cascoda_evbme.h"


/******************************************************************************/
/****** Configured Parameters                                            ******/
/******************************************************************************/
u8_t    MyChannel;
u8_t    MyDeviceNumber;
u16_t   MyShortAddress;
u16_t   MyPANId;
u8_t    MyIEEEAddress[8];
u16_t   DstShortAddress;
u8_t    DstIEEEAddress[8];


/******************************************************************************/
/****** Current State of Device                                          ******/
/******************************************************************************/
#define MDS_NOTSET        (0)
#define MDS_DEV1122       (1)
#define MDS_DEV3344       (2)
#define MDS_DEV1122CD     (9)
#define MDS_DEV3344CD     (10)
#define MDS_SLEEPYRFC     (16)
#define MDS_RFC           (17)

u8_t    MyDeviceState = MDS_NOTSET;


/******************************************************************************/
/****** Data from SetupAwaitAssoc() and SetupAwaitOrphan()               ******/
/******************************************************************************/

u8_t    AssocDeviceAddress[8] = {0,0,0,0,0,0,0,0};
u8_t    OrphanDeviceAddress[8] = {0,0,0,0,0,0,0,0};
u8_t    AllZeroes[8] = {0,0,0,0,0,0,0,0};
u16_t   StoredAssocShortAddress;
u8_t    StoredAssocStatus;
u16_t   StoredOrphanShortAddress;


/******************************************************************************/
/****** PIB Test Data                                                    ******/
/******************************************************************************/
#define MYPANIDLS     (MyPANId&0xFF)
#define MYPANIDMS     (MyPANId>>8)

#define MYSHORTLS     (MyShortAddress&0xFF)
#define MYSHORTMS     (MyShortAddress>>8)

#define REMSHORTLS    (DstShortAddress&0xFF)
#define REMSHORTMS    (DstShortAddress>>8)

extern MAC_Message_t   SPI_Transmit, SPI_Receive;

u8_t    PIBBuffer[256];
u8_t    PIBLength;

MacAddr_t     TestDest;
SecSpec_t     Security;
u8_t          Handle;
u8_t    DataBuffer[128] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
                           0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
                           0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
                           0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
                           0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
                           0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
                           0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
                           0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
                           0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
                           0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
                           0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
                           0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
                           0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
                           0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
                           0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
                           0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10};

u8_t    TransitionBuffer[144] =
                        {0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,
                         0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x11,0x21,
                         0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,
                         0x1C,0x1D,0x1E,0x1F,0x22,0x32,0x42,0x52,0x62,
                         0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
                         0x33,0x43,0x53,0x63,0x73,0x83,0x93,0xA3,0xB3,
                         0x3C,0x3D,0x3E,0x3F,0x44,0x54,0x64,0x74,0x84,
                         0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x55,0x65,
                         0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
                         0x66,0x76,0x86,0x96,0xA6,0xB6,0xC6,0xD6,0xE6,
                         0x6F,0x77,0x87,0x97,0xA7,0xB7,0xC7,0xD7,0xE7,
                         0x7F,0x88,0x98,0xA8,0xB8,0xC8,0xD8,0xE8,0xF9,
                         0x99,0xA9,0xB9,0xC9,0xD9,0xE9,0xFA,0xAB,0xAC,
                         0xAD,0xAE,0xAF,0xB1,0xBB,0xCB,0xDB,0xEB,0xFC,
                         0xCA,0xCC,0xDC,0xEC,0xFD,0xDE,0xDF,0xEE,0xFF,
                         0xF3,0xF6,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


u8_t    Beacon[20] = { 0x01,'B','e','a','c','o','n',' ','P','a','y','l','o','a','d',
                       ' ',' ',' ',' ',' '};

/******************************************************************************/
/****** Data for testing Security PIB Set Requests                       ******/
/******************************************************************************/
M_KeyDescriptor_t   MaxKeyDesc =
{
    /* Fixed */
    { LOOKUP_DESC_TABLE_SIZE, KEY_DEVICE_TABLE_SIZE,
    KEY_USAGE_TABLE_SIZE,{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15} },
    /* KeyIdLookupList */
    {  {  { 0xa,0xb,0xc,0xd,0xe,0xf,0x1,0x2,0x3}, 1 },
       {  { 0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9}, 1 },
       {  { 0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb}, 1 },
       {  { 0x5,0x6,0x7,0x8,0x9 }, 0 },
       {  { 0xa,0xb,0xc,0xd,0xe }, 0 }
    },
    /* KeyDeviceList */
    { {3}, {1}, {2}, {5}, {0} },
    /* KeyUsageList */
    { {1}, {7} }
};

M_DeviceDescriptor_t DeviceTable[4] =
{
    { { 0xAA, 0x1A }, {0x34, 0x12}, {1,2,3,4,0,0,0,0},{0,0,0,0},1},
    { { 0xAA, 0x1A }, {0x78, 0x56}, {1,2,3,4,0,0,0,1},{0,0,0,0},1},
    { { 0xAA, 0x1A }, {0xBC, 0x9A}, {1,2,3,4,0,0,0,2},{0,0,0,0},1},
    { { 0xAA, 0x1A }, {0xF0, 0xDE}, {1,2,3,4,0,0,0,3},{0,0,0,0},1}
};

M_SecurityLevelDescriptor_t SecLevDesc = {0, 1, 7, 1 };


/******************************************************************************/
/****** SEP2 Security                                                    ******/
/******************************************************************************/
u8_t  sepDefaultKeySource[] = {0,0,0,0,0,0,0,0xff};

u8_t  sepKeyDesc[] =
{
    1,  /* KeyIdLookupListEntries */
    1,  /* KeyDeviceListEntries */
    1,  /* KeyUsageListEntries */
    /* KeyLookupDataEntry */
    0xbf,0xbe,0xbd,0xbc,0xbb,0xba,0xb9,0xb8, /*Key */
    0xb7,0xb6,0xb5,0xb4,0xb3,0xb2,0xb1,0xb0,
    1,                    /* LookupData[0] */
    0,0,0,0,0,0,0,0xff,   /* LookupData[1] - [7] */
    1,                    /* LookupData size code */
    /* KeyDeviceListEntry */
    0,  /* index = 0, uniquedevice = 0,blacklisted = 0 */
    /* KeyUsageListEntry */
    1
};

u8_t sepDeviceDescriptor[] =
{
    0,0,
    0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,
    0
};


/******************************************************************************/
/******************************************************************************/
/****** InitialiseDevice()                                               ******/
/******************************************************************************/
/****** Brief:  Set up Short Address etc. from MyDeviceNumber            ******/
/******************************************************************************/
/****** Param:  Channel - Channel Number                                 ******/
/****** Param:  PANId - PAN Id                                           ******/
/****** Param:  ShortAddress - Device Short Address                      ******/
/****** Param:  pExtAddress - Pointer to Device Extended Address         ******/
/******************************************************************************/
/****** Return: Initialisation Status                                    ******/
/******************************************************************************/
/******************************************************************************/
u8_t InitialiseDevice( u8_t Channel, u16_t PANId, u16_t ShortAddress, u8_t *pExtAddress )
{
    u8_t    Status = MAC_SUCCESS;

    dps("INITIALISE DEVICE\n");
    MyChannel = Channel;
    MyPANId = PANId;
    MyShortAddress = ShortAddress;
    memcpy( MyIEEEAddress, pExtAddress, 8 );

    dps("Reset PIB\n");
    Status = MLME_RESET_request_sync( 1 );
    if ( Status != MAC_SUCCESS ) return Status;
    PIBBuffer[0] = MyChannel;   /* Channel number */
    dps("Set channel ");dpDec(MyChannel);dpnl();
    Status = MLME_SET_request_sync( phyCurrentChannel, 0, 1, PIBBuffer );
    if ( Status != MAC_SUCCESS ) return Status;
    PIBBuffer[0] = MYSHORTLS; PIBBuffer[1] = MYSHORTMS;
    dps("Set short address "); dp16(MyShortAddress);dpnl();
    Status = MLME_SET_request_sync( macShortAddress, 0, 2, PIBBuffer );
    if ( Status != MAC_SUCCESS ) return Status;
    Status = MLME_SET_request_sync( nsIEEEAddress, 0, 8, MyIEEEAddress );
    if ( Status != MAC_SUCCESS ) return Status;
    PIBBuffer[0] = MYPANIDLS; PIBBuffer[1] = MYPANIDMS;
    Status = MLME_SET_request_sync( macPANId, 0, 2, PIBBuffer );
    if ( Status != MAC_SUCCESS ) return Status;
    PIBBuffer[0] = 1;
    Status = MLME_SET_request_sync( macRxOnWhenIdle, 0, 1, PIBBuffer );
    if ( Status != MAC_SUCCESS ) return Status;
    TestDest.ShortAddress = DstShortAddress;
    Security.SecurityLevel = 0;
    dps("DEVICE INITIALISED OK\n");
    return Status;
} // End of InitialiseDevice()


/******************************************************************************/
/******************************************************************************/
/****** StartDevice()                                                    ******/
/******************************************************************************/
/****** Brief:  Set up Short Address etc. from MyDeviceNumber            ******/
/******************************************************************************/
/****** Param:  Length - Beacon Length                                   ******/
/****** Param:  pBeacon - Pointer to Beacon                              ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t StartDevice( u8_t Length, u8_t *pBeacon )
{
    u8_t  Status = MAC_SUCCESS;

    dps("START DEVICE\n");
    PIBBuffer[0] = Length;
    Security.SecurityLevel = 0;
    Status = MLME_SET_request_sync( macBeaconPayloadLength, 0, 1, PIBBuffer );
    if ( Status != MAC_SUCCESS ) return Status;
    Status = MLME_SET_request_sync( macBeaconPayload, 0, Length, pBeacon );
    if ( Status != MAC_SUCCESS ) return Status;
    Status = MLME_START_request_sync( MyPANId, MyChannel, 15, 15, 1, 0, 0, &Security, &Security );
    if ( Status != MAC_SUCCESS ) return Status;
    dps("DEVICE STARTED OK\n");
    return Status;
} // End of StartDevice()


/******************************************************************************/
/******************************************************************************/
/****** InitSecurity()                                                   ******/
/******************************************************************************/
/****** Brief:  Initialise Security PIB                                  ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void InitSecurity( void )
{
      dps("INITIALISE SECURITY\n");
      dps("set mac security enabled = 1\n");
      PIBBuffer[0] = 1;
      MLME_SET_request_sync( macSecurityEnabled, 0, 1, PIBBuffer );
      dps("Set macDefaultKeySource\n");
      MLME_SET_request_sync( macDefaultKeySource, 0, 8, sepDefaultKeySource );
      dps("Set macKeyTableEntries to 1\n");
      PIBBuffer[0] = 1;
      MLME_SET_request_sync( macKeyTableEntries, 0, 1, PIBBuffer );
      MLME_SET_request_sync( macKeyTable, 0, sizeof(sepKeyDesc), sepKeyDesc );
      MLME_GET_request_sync( macKeyTable, 0, &PIBLength, PIBBuffer );
      PIBBuffer[0] = 1;
      MLME_SET_request_sync( macDeviceTableEntries, 0, 1, PIBBuffer );
      sepDeviceDescriptor[0] = MYPANIDLS;
      sepDeviceDescriptor[1] = MYPANIDMS;
      sepDeviceDescriptor[2] = REMSHORTLS;
      sepDeviceDescriptor[3] = REMSHORTMS;
      memcpy( sepDeviceDescriptor+4, DstIEEEAddress, 8 );
      MLME_SET_request_sync( macDeviceTable, 0,
                             sizeof(sepDeviceDescriptor), sepDeviceDescriptor );
      PIBBuffer[0] = MyChannel;   /* Channel number */
      MLME_SET_request_sync( phyCurrentChannel, 0, 1, PIBBuffer );
      PIBBuffer[0] = MYSHORTLS; PIBBuffer[1] = MYSHORTMS;
      MLME_SET_request_sync( macShortAddress, 0, 2, PIBBuffer );
      MLME_SET_request_sync( nsIEEEAddress, 0, 8, MyIEEEAddress );
      PIBBuffer[0] = MYPANIDLS; PIBBuffer[1] = MYPANIDMS;
      MLME_SET_request_sync( macPANId, 0, 2, PIBBuffer );
      PIBBuffer[0] = 1;
      MLME_SET_request_sync( macRxOnWhenIdle, 0, 1, PIBBuffer );
      TestDest.ShortAddress = DstShortAddress;
      Security.SecurityLevel = 5;
      Security.KeyIdMode = 1;
      Security.KeyIndex = 1;
      memcpy( Security.KeySource, sepDefaultKeySource, 8);
      dps("SECURITY SET UP\n");
} // End of InitSecurity()


/******************************************************************************/
/******************************************************************************/
/****** PIBTest()                                                        ******/
/******************************************************************************/
/****** Brief:  Test MLME_RESET, MLME_SET and MLME_GET Functions         ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void PIBTest( void )
{
      dps("PIB TEST\n");
      dps("Reset PIB to default values\n");
      MLME_RESET_request_sync( 1 );
      EVBMEForceUpStream(SPI_MLME_RESET_CONFIRM);
      dps("macBeaconPayload\n");
      MLME_GET_request_sync( macBeaconPayload, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      dps("macBSN\n");
      MLME_GET_request_sync( macBSN, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      dps("macDSN\n");
      MLME_GET_request_sync( macDSN, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      dps("macBeaconTxTime\n");
      MLME_GET_request_sync( macBeaconTxTime, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      dps("macCoordExtendedAddress\n");
      MLME_GET_request_sync( macCoordExtendedAddress, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      dps("macCoordShortAddress\n");
      MLME_GET_request_sync( macCoordShortAddress, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      dps("macMaxFrameTotalWaitTime\n");
      MLME_GET_request_sync( macMaxFrameTotalWaitTime, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      dps("macSyncSymbolOffset\n");
      MLME_GET_request_sync( macSyncSymbolOffset, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      dps("macTransactionPersistenceTime\n");
      MLME_GET_request_sync( macTransactionPersistenceTime, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      dps("Set macCoordExtendedAddress and read back\n");
      MLME_SET_request_sync( macCoordExtendedAddress, 0, 8, MyIEEEAddress );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      MLME_GET_request_sync( macCoordExtendedAddress, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      dps("PIB TEST FINISHED\n");
} // End of PIBTest()


/******************************************************************************/
/******************************************************************************/
/****** SecPIBTest()                                                     ******/
/******************************************************************************/
/****** Brief:  Test MLME_RESET, MLME_SET and MLME_GET Functions         ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void SecPIBTest( void )
{
      dpslow("SEC PIB TEST\n");
      dpslow("Running SecPIB test\n");
      dpslow("Reset PIB\n");
      MLME_RESET_request_sync( 1 );
      EVBMEForceUpStream(SPI_MLME_RESET_CONFIRM);

      dpslow("Get first three entries of macKeyTable\n");
      dpslow("Should get error F9 Invalid Index\n");
      MLME_GET_request_sync( macKeyTable, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      MLME_GET_request_sync( macKeyTable, 1, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      MLME_GET_request_sync( macKeyTable, 2, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Set macKeyTableEntries to 3\n");
      dpslow("Should get error E8 Invalid parameter\n");
      PIBBuffer[0] = 3;
      MLME_SET_request_sync( macKeyTableEntries, 0, 1, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Set macKeyTableEntries to 1\n");
      dpslow("Should get status 00 Success\n");
      PIBBuffer[0] = 1;
      MLME_SET_request_sync( macKeyTableEntries, 0, 1, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Set entry 0 of macKeyTable\n");
      dpslow("Should get status 00 Success\n");
      MLME_SET_request_sync( macKeyTable, 0, sizeof(MaxKeyDesc), &MaxKeyDesc );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Set entry 1 of macKeyTable\n");
      dpslow("Should get status 00 Success (although out of range)\n");
      MLME_SET_request_sync( macKeyTable, 1, sizeof(MaxKeyDesc), &MaxKeyDesc );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Get entry 0 of macKeyTable\n");
      dpslow("Should get status 00 Success\n");
      MLME_GET_request_sync( macKeyTable, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Get entry 1 of macKeyTable\n");
      dpslow("Should get status F9 Invalid Index\n");
      MLME_GET_request_sync( macKeyTable, 1, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Get entry 2 of macKeyTable\n");
      dpslow("Should get status F9 Invalid Index\n");
      MLME_GET_request_sync( macKeyTable, 2, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Set macKeyTableEntries to 2\n");
      dpslow("Should get status 00 Success\n");
      PIBBuffer[0] = 2;
      MLME_SET_request_sync( macKeyTableEntries, 0, 1, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Set entry 0 of macKeyTable\n");
      dpslow("Should get status 00 Success\n");
      MLME_SET_request_sync( macKeyTable, 0, sizeof(MaxKeyDesc), &MaxKeyDesc );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Set entry 1 of macKeyTable\n");
      dpslow("Should get status 00 Success\n");
      MLME_SET_request_sync( macKeyTable, 1, sizeof(MaxKeyDesc), &MaxKeyDesc );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Get entry 0 of macKeyTable\n");
      dpslow("Should get status 00 Success\n");
      MLME_GET_request_sync( macKeyTable, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Get entry 1 of macKeyTable\n");
      dpslow("Should get status 00 Success\n");
      MLME_GET_request_sync( macKeyTable, 1, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Get entry 2 of macKeyTable\n");
      dpslow("Should get status F9 Invalid Index\n");
      MLME_GET_request_sync( macKeyTable, 2, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Set entry 0 of macKeyTable with invalid no of lookup list entries\n");
      dpslow("Should get status E8 Invalid parameter\n");
      MaxKeyDesc.Fixed.KeyIdLookupListEntries++;
      MLME_SET_request_sync( macKeyTable, 0, sizeof(MaxKeyDesc), &MaxKeyDesc );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      MaxKeyDesc.Fixed.KeyIdLookupListEntries--;
      MLME_SET_request_sync( macKeyTable, 0, sizeof(MaxKeyDesc), &MaxKeyDesc );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Set entry 0 of macKeyTable with invalid no of device list entries\n");
      dpslow("Should get status E8 Invalid parameter\n");
      MaxKeyDesc.Fixed.KeyDeviceListEntries++;
      MLME_SET_request_sync( macKeyTable, 0, sizeof(MaxKeyDesc), &MaxKeyDesc );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      MaxKeyDesc.Fixed.KeyDeviceListEntries--;
      MLME_SET_request_sync( macKeyTable, 0, sizeof(MaxKeyDesc), &MaxKeyDesc );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Set entry 0 of macKeyTable with invalid no of usage list entries\n");
      dpslow("Should get status E8 Invalid parameter\n");
      MaxKeyDesc.Fixed.KeyUsageListEntries++;
      MLME_SET_request_sync( macKeyTable, 0, sizeof(MaxKeyDesc), &MaxKeyDesc );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      MaxKeyDesc.Fixed.KeyUsageListEntries--;
      MLME_SET_request_sync( macKeyTable, 0, sizeof(MaxKeyDesc), &MaxKeyDesc );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("End of macKeyTable test\n");

      dpslow("Get first three entries of macDeviceTable\n");
      dpslow("Should get error F9 Invalid Index\n");
      MLME_GET_request_sync( macDeviceTable, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      MLME_GET_request_sync( macDeviceTable, 1, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      MLME_GET_request_sync( macDeviceTable, 2, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Set macDeviceTableEntries to 6\n");
      dpslow("Should get error E8 Invalid parameter\n");
      PIBBuffer[0] = 6;
      MLME_SET_request_sync( macDeviceTableEntries, 0, 1, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Set macDeviceTableEntries to 3\n");
      dpslow("Should get status 00 Success\n");
      PIBBuffer[0] = 3;
      MLME_SET_request_sync( macDeviceTableEntries, 0, 1, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Get first three entries of macDeviceTable\n");
      dpslow("Should get status 00 Success but reset values\n");
      MLME_GET_request_sync( macDeviceTable, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      MLME_GET_request_sync( macDeviceTable, 1, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      MLME_GET_request_sync( macDeviceTable, 2, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Get fourth entry of macDeviceTable\n");
      dpslow("Should get error F9 Invalid Index\n");
      MLME_GET_request_sync( macDeviceTable, 3, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Set entry 0 of macDeviceTable\n");
      dpslow("Should get status 00 Success\n");
      MLME_SET_request_sync( macDeviceTable, 0, sizeof(M_DeviceDescriptor_t), DeviceTable );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Set entry 1 of macDeviceTable\n");
      dpslow("Should get status 00 Success\n");
      MLME_SET_request_sync( macDeviceTable, 1, sizeof(M_DeviceDescriptor_t), DeviceTable+1 );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Set entry 2 of macDeviceTable\n");
      dpslow("Should get status 00 Success\n");
      MLME_SET_request_sync( macDeviceTable, 2, sizeof(M_DeviceDescriptor_t), DeviceTable+2 );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Set entry 3 of macDeviceTable\n");
      dpslow("Should get status 00 Success\n");
      MLME_SET_request_sync( macDeviceTable, 3, sizeof(M_DeviceDescriptor_t), DeviceTable+3 );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Get first three entries of macDeviceTable\n");
      dpslow("Should get status 00 Success\n");
      MLME_GET_request_sync( macDeviceTable, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      MLME_GET_request_sync( macDeviceTable, 1, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      MLME_GET_request_sync( macDeviceTable, 2, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Get fourth entry of macDeviceTable\n");
      dpslow("Should get error F9 Invalid Index\n");
      MLME_GET_request_sync( macDeviceTable, 3, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      dpslow("End of macDeviceTableTest\n");

      dpslow("Get first three entries of macSecurityLevelTable\n");
      dpslow("Should get error F9 Invalid Index\n");
      MLME_GET_request_sync( macSecurityLevelTable, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      MLME_GET_request_sync( macSecurityLevelTable, 1, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      MLME_GET_request_sync( macSecurityLevelTable, 2, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Set macSecurityLevelTableEntries to 3\n");
      dpslow("Should get error E8 Invalid parameter\n");
      PIBBuffer[0] = 3;
      MLME_SET_request_sync( macSecurityLevelTableEntries, 0, 1, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Set macSecurityLevelTableEntries to 1\n");
      dpslow("Should get status 00 Success\n");
      PIBBuffer[0] = 1;
      MLME_SET_request_sync( macSecurityLevelTableEntries, 0, 1, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Get first three entries of macSecurityLevelTable\n");
      dpslow("Should get status 00 Success but reset values\n");
      MLME_GET_request_sync( macSecurityLevelTable, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      dpslow("Should get status F9 Invalid Index\n");
      MLME_GET_request_sync( macSecurityLevelTable, 1, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      MLME_GET_request_sync( macSecurityLevelTable, 2, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Set entry 0 of macSecurityLevelTable\n");
      dpslow("Should get status 00 Success\n");
      MLME_SET_request_sync( macSecurityLevelTable, 0, sizeof(M_SecurityLevelDescriptor_t), &SecLevDesc );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Set entry 1 of macSecurityLevelTable\n");
      dpslow("Should get status 00 Success\n");
      MLME_SET_request_sync( macSecurityLevelTable, 1, sizeof(M_SecurityLevelDescriptor_t), &SecLevDesc );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Set entry 2 of macSecurityLevelTable\n");
      dpslow("Should get status F9 Invalid Index\n");
      MLME_SET_request_sync( macSecurityLevelTable, 2, sizeof(M_SecurityLevelDescriptor_t), &SecLevDesc );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);

      dpslow("Get first three entries of macSecurityLevelTable\n");
      dpslow("Should get status 00 Success\n");
      MLME_GET_request_sync( macSecurityLevelTable, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      dpslow("Should get status F9 Invalid Index\n");
      MLME_GET_request_sync( macSecurityLevelTable, 1, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      MLME_GET_request_sync( macSecurityLevelTable, 2, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("End of macSecurityLevelTableTest\n");

      dpslow("Test macFrameCounter\n");
      MLME_GET_request_sync( macFrameCounter, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      PIBBuffer[0] = 1;
      PIBBuffer[1] = 2;
      PIBBuffer[2] = 3;
      PIBBuffer[3] = 4;
      MLME_SET_request_sync( macFrameCounter, 0, 4, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      MLME_GET_request_sync( macFrameCounter, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Test macAutoRequestSecurityLevel\n");
      MLME_GET_request_sync( macAutoRequestSecurityLevel, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      PIBBuffer[0] = 7;
      MLME_SET_request_sync( macAutoRequestSecurityLevel, 0, 1, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      MLME_GET_request_sync( macAutoRequestSecurityLevel, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Test macAutoRequestKeyIdMode\n");
      MLME_GET_request_sync( macAutoRequestKeyIdMode, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      PIBBuffer[0] = 1;
      MLME_SET_request_sync( macAutoRequestKeyIdMode, 0, 1, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      MLME_GET_request_sync( macAutoRequestKeyIdMode, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Test macAutoRequestKeySource\n");
      MLME_GET_request_sync( macAutoRequestKeySource, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      PIBBuffer[0] = 1;
      PIBBuffer[1] = 2;
      PIBBuffer[2] = 3;
      PIBBuffer[3] = 4;
      PIBBuffer[4] = 5;
      PIBBuffer[5] = 6;
      PIBBuffer[6] = 7;
      PIBBuffer[7] = 8;
      MLME_SET_request_sync( macAutoRequestKeySource, 0, 8, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      MLME_GET_request_sync( macAutoRequestKeySource, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Test macAutoRequestKeyIndex\n");
      MLME_GET_request_sync( macAutoRequestKeyIndex, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      PIBBuffer[0] = 1;
      MLME_SET_request_sync( macAutoRequestKeyIndex, 0, 1, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      MLME_GET_request_sync( macAutoRequestKeyIndex, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Test macDefaultKeySource\n");
      MLME_GET_request_sync( macDefaultKeySource, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      PIBBuffer[0] = 1;
      PIBBuffer[1] = 2;
      PIBBuffer[2] = 3;
      PIBBuffer[3] = 4;
      PIBBuffer[4] = 5;
      PIBBuffer[5] = 6;
      PIBBuffer[6] = 7;
      PIBBuffer[7] = 8;
      MLME_SET_request_sync( macDefaultKeySource, 0, 8, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      MLME_GET_request_sync( macDefaultKeySource, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);


      dpslow("Test macPANCoordExtendedAddress\n");
      MLME_GET_request_sync( macPANCoordExtendedAddress, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      PIBBuffer[0] = 8;
      PIBBuffer[1] = 7;
      PIBBuffer[2] = 6;
      PIBBuffer[3] = 5;
      PIBBuffer[4] = 4;
      PIBBuffer[5] = 3;
      PIBBuffer[6] = 2;
      PIBBuffer[7] = 1;
      MLME_SET_request_sync( macPANCoordExtendedAddress, 0, 8, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      MLME_GET_request_sync( macPANCoordExtendedAddress, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("Test macPANCoordShortAddress\n");
      MLME_GET_request_sync( macPANCoordShortAddress, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
      PIBBuffer[0] = 0x33;
      PIBBuffer[1] = 0x44;
      MLME_SET_request_sync( macPANCoordShortAddress, 0, 2, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      MLME_GET_request_sync( macPANCoordShortAddress, 0, &PIBLength, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);

      dpslow("End of Sec Pib tests\n");
} // End of SecPIBTest()


/******************************************************************************/
/******************************************************************************/
/****** OrphanScanTest()                                                 ******/
/******************************************************************************/
/****** Brief:  Test Orphan Scan                                         ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void OrphanScanTest( void )
{
      Security.SecurityLevel = 0;
      MLME_RESET_request_sync( 0 );
      EVBMEForceUpStream(SPI_MLME_RESET_CONFIRM);
      memcpy( MyIEEEAddress, (u8_t []){0xFF,0,0,0,0,0,0,0}, 8 );
      MLME_SET_request_sync( nsIEEEAddress, 0, 8, MyIEEEAddress );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      PIBBuffer[0] = MYPANIDLS; PIBBuffer[1] = MYPANIDMS;
      MLME_SET_request_sync( macPANId, 0, 2, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      MLME_SCAN_request( ORPHAN_SCAN, 0x7FFF800, 5, &Security );
      do
       {
           SPI_Wait();
       } while ( SPI_Receive.CommandId != SPI_MLME_SCAN_CONFIRM );
      EVBMEForceUpStream(SPI_MLME_SCAN_CONFIRM);

} // End of OrphanScanTest()


/******************************************************************************/
/******************************************************************************/
/****** SetupAwaitOrphan()                                               ******/
/******************************************************************************/
/****** Brief:  Setup data to reply to Orphan indication                 ******/
/******************************************************************************/
/****** Param:  -  pDeviceAddress - IEEE address of device expected      ******/
/****** Param:  -  OrphanShortAddress - Short address of orphan          ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void SetupAwaitOrphan( u8_t *pDeviceAddress, u16_t OrphanShortAddress )
{
    memcpy( OrphanDeviceAddress, pDeviceAddress, 8 );
    StoredOrphanShortAddress = OrphanShortAddress;
} // End of SetupAwaitOrphan()


/******************************************************************************/
/******************************************************************************/
/****** SetupAwaitAssoc()                                                ******/
/******************************************************************************/
/****** Brief:  Setup data to reply to Assoc indication                  ******/
/******************************************************************************/
/****** Param:  -  pDeviceAddress - IEEE address of device expected      ******/
/****** Param:  -  AssocShortAddress - Short address of new dev          ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void SetupAwaitAssoc( u8_t *pDeviceAddress, u16_t AssocShortAddress, u8_t Status )
{
    memcpy( AssocDeviceAddress, pDeviceAddress, 8 );
    StoredAssocShortAddress = AssocShortAddress;
    StoredAssocStatus = Status;
} // End of SetupAwaitAssoc()


/******************************************************************************/
/******************************************************************************/
/****** SimpleTxTest()                                                   ******/
/******************************************************************************/
/****** Brief:  Test Transmissions to No-One                             ******/
/******************************************************************************/
/****** Param:  TxOpt - TX Options Bit Field                             ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void SimpleTxTest( u8_t TxOpt )
{
      MLME_RESET_request_sync( 0 );
      EVBMEForceUpStream(SPI_MLME_RESET_CONFIRM);
      PIBBuffer[0] = MyChannel;   /* Channel number */
      MLME_SET_request_sync( phyCurrentChannel, 0, 1, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      PIBBuffer[0] = MYSHORTLS; PIBBuffer[1] = MYSHORTMS;
      MLME_SET_request_sync( macShortAddress, 0, 2, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      PIBBuffer[0] = MYPANIDLS; PIBBuffer[1] = MYPANIDMS;
      MLME_SET_request_sync( macPANId, 0, 2, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      PIBBuffer[0] = 1;
      MLME_SET_request_sync( macRxOnWhenIdle, 0, 1, PIBBuffer );
      EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
      TestDest.ShortAddress = DstShortAddress;
      //Security.SecurityLevel = 0;
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 10, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 20, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 30, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 40, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 50, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 60, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 70, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);

      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 10, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 20, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 30, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 40, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 50, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 60, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 70, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 10, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 20, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 30, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 40, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 50, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 60, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 70, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 80, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 90, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 100, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 108, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 110, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &TestDest, 120, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);

} // End of SimpleTxTest()


/******************************************************************************/
/******************************************************************************/
/****** DataTest0()                                                      ******/
/******************************************************************************/
/****** Brief:  Test Data Transmissions                                  ******/
/******************************************************************************/
/****** Param:  i - Sequence Number                                      ******/
/****** Param:  TxOpt - TX Options Bit Field                             ******/
/****** Param:  DestShort - Destination Short Adddress                   ******/
/******************************************************************************/
/****** Return: 0                                                        ******/
/******************************************************************************/
/******************************************************************************/
u8_t DataTest0( u8_t i, u8_t TxOpt, u16_t DestShort )
{
      MacAddr_t   DstAddr;

      DstAddr.ShortAddress = DestShort;
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &DstAddr, (i%12)*10, DataBuffer, Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);

      return 0;
} // End of DataTest0()


/******************************************************************************/
/******************************************************************************/
/****** DataTest1()                                                      ******/
/******************************************************************************/
/****** Brief:  Test Data Transmissions                                  ******/
/******************************************************************************/
/****** Param:  i - Sequence Number                                      ******/
/****** Param:  TxOpt - TX Options Bit Field                             ******/
/****** Param:  DestShort - Destination Short Adddress                   ******/
/******************************************************************************/
/****** Return: 0                                                        ******/
/******************************************************************************/
/******************************************************************************/
u8_t DataTest1( u8_t i, u8_t TxOpt, u16_t DestShort )
{
      MacAddr_t   DstAddr;

      DstAddr.ShortAddress = 0xFFFF;
      MCPS_DATA_request( MAC_MODE_SHORT_ADDR, MAC_MODE_SHORT_ADDR, MyPANId,
                         &DstAddr, 9, TransitionBuffer + ((i%16)*9), Handle++, TxOpt + TXOPT_ACKREQ,
                         &Security );
      do
      {
          SPI_Wait();
      } while ( SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM );
      EVBMEForceUpStream(SPI_MCPS_DATA_CONFIRM);

      return 0;
} // End of DataTest1()


/******************************************************************************/
/******************************************************************************/
/****** DataTest()                                                       ******/
/******************************************************************************/
/****** Brief:  Test Data Transmissions                                  ******/
/******************************************************************************/
/****** Param:  NoOfRequests - Number of Data Requests                   ******/
/****** Param:  TxOpt - TX Options Bit Field                             ******/
/****** Param:  DestShort - Destination Short Adddress                   ******/
/****** Param:  Type - Data Type (0-3)                                   ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t DataTest( u8_t NoOfRequests, u8_t TxOpt, u16_t DestShort, u8_t Type )
{
    u8_t    i;

    dps("DATA TEST\n");
    for ( i = 0; i < NoOfRequests; i++ )
    {
        switch ( Type )
        {
        case 0:   /* Monotonic, different lengths */
            Security.SecurityLevel = 0;
            DataTest0( i, TxOpt, DestShort );
            break;
        case 1:   /* Transition */
            Security.SecurityLevel = 0;
            DataTest1( i, TxOpt, DestShort );
            break;
        case 2:
            Security.SecurityLevel = 5;
            Security.KeyIdMode = 1;
            Security.KeyIndex = 1;
            memcpy( Security.KeySource, sepDefaultKeySource, 8);
            DataTest0( i, TxOpt, DestShort );
            break;
        case 3:
            Security.SecurityLevel = 5;
            Security.KeyIdMode = 1;
            Security.KeyIndex = 1;
            memcpy( Security.KeySource, sepDefaultKeySource, 8);
            DataTest1( i, TxOpt, DestShort );
            break;

        default:
            dps("Invalid Data Type\n");
            return 0xFF;
        }
    }
    dps("DATA TEST FINISHED\n");
    return 0;
} // End of DataTest()


/******************************************************************************/
/******************************************************************************/
/****** ScanTest()                                                       ******/
/******************************************************************************/
/****** Brief:  Test Scans                                               ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void ScanTest( void )
{
      u8_t i;

      dps("SCAN TEST\n");
      Security.SecurityLevel = 0;
      MLME_RESET_request_sync( 0 );
      EVBMEForceUpStream(SPI_MLME_RESET_CONFIRM);
      dps("Perform Energy detect scan on all channels with Scan Duration 0 to 7\n");
      for ( i=0; i <= 7; i++)
      {
          MLME_SCAN_request( 0, 0x7FFF800, i, &Security );
          do
          {
              SPI_Wait();
          } while ( SPI_Receive.CommandId != SPI_MLME_SCAN_CONFIRM );
      EVBMEForceUpStream(SPI_MLME_SCAN_CONFIRM);
      }
      dps("Perform Energy detect scan on channel 11 with Scan Duration 0 to 14\n");
      for ( i=0; i <= 14; i++)
      {
          MLME_SCAN_request( 0, 0x800, i, &Security );
          do
          {
              SPI_Wait();
          } while ( SPI_Receive.CommandId != SPI_MLME_SCAN_CONFIRM );
      EVBMEForceUpStream(SPI_MLME_SCAN_CONFIRM);
      }
      dps("Perform Active Scan on all channels with Scan Duration 0 to 7\n");
      for ( i=0; i <= 7; i++)
      {
          MLME_SCAN_request( 1, 0x7FFF800, i, &Security );
          do
          {
              SPI_Wait();
          } while ( SPI_Receive.CommandId != SPI_MLME_SCAN_CONFIRM );
      EVBMEForceUpStream(SPI_MLME_SCAN_CONFIRM);
      }
      dps("Perform Active Scan on channel 11 with Scan Duration 0 to 14\n");
      for ( i=0; i <= 14; i++)
      {
          MLME_SCAN_request( 1, 0x800, i, &Security );
          do
          {
              SPI_Wait();
          } while ( SPI_Receive.CommandId != SPI_MLME_SCAN_CONFIRM );
      EVBMEForceUpStream(SPI_MLME_SCAN_CONFIRM);
      }
      dps("SCAN TEST FINISHED\n");

} // End of ScanTest()


/******************************************************************************/
/******************************************************************************/
/****** AssocNonSleepyTest()                                             ******/
/******************************************************************************/
/****** Brief:  Test Associate                                           ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void AssocNonSleepyTest( void )
{
    u8_t    i;

    MLME_RESET_request_sync( 1 );
    EVBMEForceUpStream(SPI_MLME_RESET_CONFIRM);
    MyIEEEAddress[0] = 0xFF;
    for ( i = 1; i < 8; i++ )
    {
        MyIEEEAddress[i] = 0;
    }

    MLME_SET_request_sync( nsIEEEAddress, 0, 8, MyIEEEAddress );
    EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
    PIBBuffer[0] = 0xFE; PIBBuffer[1] = 0xFF;
    dps("Set short address FFFE\n");
    MLME_SET_request_sync( macShortAddress, 0, 2, PIBBuffer );
    EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
    PIBBuffer[0] = 0;
    dps("Set macAutoRequest to False\n");
    MLME_SET_request_sync( macAutoRequest, 0, 1, PIBBuffer );
    EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
    Security.SecurityLevel = 0;
    MLME_SCAN_request( ACTIVE_SCAN, (((u32_t)1)<< MyChannel), 5, &Security );
    do
    {
        SPI_Wait();
    } while ( SPI_Receive.CommandId != SPI_MLME_SCAN_CONFIRM );
    EVBMEForceUpStream(SPI_MLME_SCAN_CONFIRM);

    TestDest.ShortAddress = 0x1122;
    MLME_ASSOCIATE_request( MyChannel, 2, MyPANId, &TestDest, 0xCF, &Security );
    do
    {
        SPI_Wait();
    } while ( SPI_Receive.CommandId != SPI_MLME_ASSOCIATE_CONFIRM );
    EVBMEForceUpStream(SPI_MLME_ASSOCIATE_CONFIRM);
    MyShortAddress = SPI_Receive.PData.AssocCnf.AssocShortAddress[0] +
      ((u16_t)SPI_Receive.PData.AssocCnf.AssocShortAddress[1]<<8);

} // End of AssocNonSleepyTest()


/******************************************************************************/
/******************************************************************************/
/****** AssocSleepyTest()                                                ******/
/******************************************************************************/
/****** Brief:  Test Associate                                           ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void AssocSleepyTest( void )
{
    u8_t    i;
    u8_t    Interval[2];
    FullAddr_t  Coord = {2,{ 0xaa, 0x1b },{ 0x22, 0x11,0,0,0,0,0,0} };

    MLME_RESET_request_sync( 1 );
    EVBMEForceUpStream(SPI_MLME_RESET_CONFIRM);
    MyIEEEAddress[0] = 0xFF;
    for ( i = 1; i < 8; i++ )
    {
        MyIEEEAddress[i] = 0;
    }

    MLME_SET_request_sync( nsIEEEAddress, 0, 8, MyIEEEAddress );
    EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
    PIBBuffer[0] = 0xFE; PIBBuffer[1] = 0xFF;
    dps("Set short address FFFE\n");
    MLME_SET_request_sync( macShortAddress, 0, 2, PIBBuffer );
    EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
    PIBBuffer[0] = 0;
    dps("Set macAutoRequest to False\n");
    MLME_SET_request_sync( macAutoRequest, 0, 1, PIBBuffer );
    EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
    Security.SecurityLevel = 0;
    MLME_SCAN_request( ACTIVE_SCAN, (((u32_t)1)<< MyChannel), 5, &Security );
    do
    {
        SPI_Wait();
    } while ( SPI_Receive.CommandId != SPI_MLME_SCAN_CONFIRM );
    EVBMEForceUpStream(SPI_MLME_SCAN_CONFIRM);

    TestDest.ShortAddress = 0x1122;
    MLME_ASSOCIATE_request( MyChannel, 2, MyPANId, &TestDest, 0xC0, &Security );
    do
    {
        SPI_Wait();
    } while ( SPI_Receive.CommandId != SPI_MLME_ASSOCIATE_CONFIRM );
    EVBMEForceUpStream(SPI_MLME_ASSOCIATE_CONFIRM);
    MyShortAddress = SPI_Receive.PData.AssocCnf.AssocShortAddress[0] +
      ((u16_t)SPI_Receive.PData.AssocCnf.AssocShortAddress[1]<<8);
    dps("Set short address ");dp16(MyShortAddress);dpnl();

    PIBBuffer[0] = MYSHORTLS;
    PIBBuffer[1] = MYSHORTMS;
    MLME_SET_request_sync( macShortAddress, 0, 2, PIBBuffer );
    EVBMEForceUpStream(SPI_MLME_SET_CONFIRM);
    MLME_GET_request_sync( macMaxFrameTotalWaitTime, 0, &PIBLength, PIBBuffer );
    EVBMEForceUpStream(SPI_MLME_GET_CONFIRM);
    dps ("MaxFrameTotalWaitTime ");dp16(PIBBuffer[0] + (PIBBuffer[1]<<8));dpnl();
    Interval[0] = 50;
    Interval[1] = 0;
    MLME_POLL_request_sync( Coord,Interval,&Security);
    EVBMEForceUpStream(SPI_MLME_POLL_CONFIRM);

} // End of AssocSleepyTest()


/******************************************************************************/
/******************************************************************************/
/****** AssocTest()                                                      ******/
/******************************************************************************/
/****** Brief:  Test Associate                                           ******/
/******************************************************************************/
/****** Param:  Attribute - Capability Information Attribute             ******/
/****** Param:  Seconds - Poll Request Interval Time [s]                 ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t AssocTest( u8_t Attribute, u8_t Seconds )
{
    u8_t    Interval[2];
    u8_t    PanChosen;
    FullAddr_t  Coord = {2,{ 0xaa, 0x1b },{ 0x22, 0x11,0,0,0,0,0,0} };

    dps("ASSOCIATE REQUEST\n");
    MLME_RESET_request_sync( 1 );
    MyIEEEAddress[0] = 0x22;
    MyIEEEAddress[1] = 0x22;
    MyIEEEAddress[2] = 0x22;
    MyIEEEAddress[3] = 0x22;
    MyIEEEAddress[4] = 0xa0;
    MyIEEEAddress[5] = 0x0d;
    MyIEEEAddress[6] = 0x5c;
    MyIEEEAddress[7] = 0xca;

    MLME_SET_request_sync( nsIEEEAddress, 0, 8, MyIEEEAddress );
    PIBBuffer[0] = 0xFE; PIBBuffer[1] = 0xFF;
    dps("Set short address FFFE\n");
    MLME_SET_request_sync( macShortAddress, 0, 2, PIBBuffer );
    PIBBuffer[0] = 0;
    dps("Set macAutoRequest to False\n");
    MLME_SET_request_sync( macAutoRequest, 0, 1, PIBBuffer );
    Security.SecurityLevel = 0;
    PanChosen = 0;
    MLME_SCAN_request( ACTIVE_SCAN, 0x7FFF800, 5, &Security );
    do
    {
        SPI_Wait();
        if ( SPI_Receive.CommandId == SPI_MLME_BEACON_NOTIFY_INDICATION )
        {
            if ( PanChosen == 0 )
            {
                Coord = SPI_Receive.PData.BeaconInd.PanDescriptor.Coord;
                MyChannel = SPI_Receive.PData.BeaconInd.PanDescriptor.LogicalChannel;

                PanChosen = 1;
            }
        }
    } while ( SPI_Receive.CommandId != SPI_MLME_SCAN_CONFIRM );

    if ( PanChosen == 0 )
    {
        dps("No Beacons\n");
        return 0xFF;
    }

    TestDest.ShortAddress = Coord.Address[0] + ( Coord.Address[1] << 8 );
    MyPANId = Coord.PANId[0] + ( Coord.PANId[1] << 8 );

    MLME_ASSOCIATE_request( MyChannel, 2, MyPANId, &TestDest, Attribute|0x80, &Security );
    do
    {
        SPI_Wait();
    } while ( SPI_Receive.CommandId != SPI_MLME_ASSOCIATE_CONFIRM );
    MyShortAddress = SPI_Receive.PData.AssocCnf.AssocShortAddress[0] +
      ((u16_t)SPI_Receive.PData.AssocCnf.AssocShortAddress[1]<<8);
    dps("Set short address ");dp16(MyShortAddress);dpnl();

    PIBBuffer[0] = MYSHORTLS;
    PIBBuffer[1] = MYSHORTMS;
    MLME_SET_request_sync( macShortAddress, 0, 2, PIBBuffer );
    MLME_GET_request_sync( macMaxFrameTotalWaitTime, 0, &PIBLength, PIBBuffer );
    dps ("MaxFrameTotalWaitTime ");dp16(PIBBuffer[0] + (PIBBuffer[1]<<8));dpnl();
    Interval[0] = LS_BYTE(Seconds*10);
    Interval[1] = MS_BYTE(Seconds*10);
    MLME_POLL_request_sync( Coord,Interval,&Security);
    dps("ASSOCIATE TEST END\n");
    return 0;

} // End of AssocTest()
