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

#ifndef MAC_MESSAGES_H
#define MAC_MESSAGES_H

#include "dev/ca8210/cascoda/include/cascoda_types.h"
#include "ieee_802_15_4.h"
#include "hwme_tdme.h"

#define MAX_HWME_ATTRIBUTE_SIZE  16
#define MAX_TDME_ATTRIBUTE_SIZE  2


/******************************************************************************/
/****** Structures used in the MAC MCPS/MLME Procedure Definitions       ******/
/******************************************************************************/

typedef struct FullAddr
{
    u8_t            AddressMode;
    u8_t            PANId[2];
    u8_t            Address[8];
} FullAddr_t;

typedef union MacAddr
{
    u16_t           ShortAddress;
    u8_t            IEEEAddress[8];
} MacAddr_t;

typedef struct SecSpec
{
    u8_t            SecurityLevel;
    u8_t            KeyIdMode;
    u8_t            KeySource[8];
    u8_t            KeyIndex;
} SecSpec_t;

typedef struct PanDescriptor
{
    FullAddr_t      Coord;
    u8_t            LogicalChannel;
    u8_t            SuperframeSpec[2];
    u8_t            GTSPermit;
    u8_t            LinkQuality;
    u8_t            TimeStamp[4];
    u8_t            SecurityFailure;
    SecSpec_t       Security;
} PanDescriptor_t;


/******************************************************************************/
/****** Downlink Functions Parameter Set Definitions                     ******/
/******************************************************************************/

// MCPS

typedef struct MCPS_DATA_request_pset
{
    u8_t            SrcAddrMode;
    FullAddr_t      Dst;
    u8_t            MsduLength;
    u8_t            MsduHandle;
    u8_t            TxOptions;
    u8_t            Msdu[MAX_DATA_SIZE];
} MCPS_DATA_request_pset_t;

// MLME

typedef struct MLME_ASSOCIATE_request_pset
{
    u8_t            LogicalChannel;
    FullAddr_t      Dst;
    u8_t            CapabilityInfo;
    SecSpec_t       Security;
} MLME_ASSOCIATE_request_pset_t;

typedef struct MLME_ASSOCIATE_response_pset
{
    u8_t            DeviceAddress[8];
    u8_t            AssocShortAddress[2];
    u8_t            Status;
    SecSpec_t       Security;
} MLME_ASSOCIATE_response_pset_t;

typedef struct MLME_DISASSOCIATE_request_pset
{
    FullAddr_t      DevAddr;
    u8_t            DisassociateReason;
    u8_t            TxIndirect;
    SecSpec_t       Security;
} MLME_DISASSOCIATE_request_pset_t;


typedef struct MLME_GET_request_pset
{
    u8_t            PIBAttribute;
    u8_t            PIBAttributeIndex;
} MLME_GET_request_pset_t;

typedef struct MLME_ORPHAN_response_pset
{
    u8_t            OrphanAddress[8];
    u8_t            ShortAddress[2];
    u8_t            AssociatedMember;
    SecSpec_t       Security;
} MLME_ORPHAN_response_pset_t;

typedef struct MLME_POLL_request_pset
{
    FullAddr_t      CoordAddress;
    u8_t            Interval[2];      /* polling interval in 0.1 seconds res */
                                      /* 0 means poll once */
                                      /* 0xFFFF means stop polling */
    SecSpec_t       Security;
} MLME_POLL_request_pset_t;

typedef struct MLME_RX_ENABLE_request_pset
{
    u8_t            DeferPermit;
    u8_t            RxOnTime[4];
    u8_t            RxOnDuration[4];
} MLME_RX_ENABLE_request_pset_t;


typedef struct MLME_SCAN_request_pset
{
    u8_t            ScanType;
    u8_t            ScanChannels[4];
    u8_t            ScanDuration;
    SecSpec_t       Security;
} MLME_SCAN_request_pset_t;

typedef struct MLME_SET_request_pset
{
    u8_t            PIBAttribute;
    u8_t            PIBAttributeIndex;
    u8_t            PIBAttributeLength;
    u8_t            PIBAttributeValue[MAX_ATTRIBUTE_SIZE];
} MLME_SET_request_pset_t;

typedef struct MLME_START_request_pset
{
    u8_t            PANId[2];
    u8_t            LogicalChannel;
    u8_t            BeaconOrder;
    u8_t            SuperframeOrder;
    u8_t            PANCoordinator;
    u8_t            BatteryLifeExtension;
    u8_t            CoordRealignment;
    SecSpec_t       CoordRealignSecurity;
    SecSpec_t       BeaconSecurity;
} MLME_START_request_pset_t;

// HWME

typedef struct HWME_SET_request_pset
{
    u8_t            HWAttribute;
    u8_t            HWAttributeLength;
    u8_t            HWAttributeValue[MAX_HWME_ATTRIBUTE_SIZE];
} HWME_SET_request_pset_t;

typedef struct HWME_GET_request_pset
{
    u8_t            HWAttribute;
} HWME_GET_request_pset_t;

typedef struct HWME_HAES_request_pset
{
    u8_t            HAESMode;
    u8_t            HAESData[16];
} HWME_HAES_request_pset_t;

// TDME

typedef struct TDME_SETSFR_request_pset
{
    u8_t            SFRPage;
    u8_t            SFRAddress;
    u8_t            SFRValue;
} TDME_SETSFR_request_pset_t;

typedef struct TDME_GETSFR_request_pset
{
    u8_t            SFRPage;
    u8_t            SFRAddress;
} TDME_GETSFR_request_pset_t;

typedef struct TDME_TESTMODE_request_pset
{
    u8_t            TestMode;
} TDME_TESTMODE_request_pset_t;

typedef struct TDME_SET_request_pset
{
    u8_t            TDAttribute;
    u8_t            TDAttributeLength;
    u8_t            TDAttributeValue[MAX_TDME_ATTRIBUTE_SIZE];
} TDME_SET_request_pset_t;

typedef struct TDME_TXPKT_request_pset
{
    u8_t            TestPacketDataType;
    u8_t            TestPacketSequenceNumber;
    u8_t            TestPacketLength;
    u8_t            TestPacketData[128];
} TDME_TXPKT_request_pset_t;

typedef struct TDME_LOTLK_request_pset
{
    u8_t            TestChannel;
    u8_t            TestRxTxb;
} TDME_LOTLK_request_pset_t;


/******************************************************************************/
/****** Uplink Functions Parameter Set Definitions                       ******/
/******************************************************************************/

// MCPS

typedef struct MCPS_DATA_confirm_pset
{
    u8_t            MsduHandle;
    u8_t            Status;
    u8_t            TimeStamp[4];
} MCPS_DATA_confirm_pset_t;

typedef struct MCPS_PURGE_confirm_pset
{
    u8_t            MsduHandle;
    u8_t            Status;
} MCPS_PURGE_confirm_pset_t;

typedef struct MCPS_DATA_indication_pset
{

    FullAddr_t      Src;
    FullAddr_t      Dst;
    u8_t            MsduLength;
    u8_t            MpduLinkQuality;
    u8_t            DSN;
    u8_t            TimeStamp[4];
    u8_t            Msdu[MAX_DATA_SIZE];
} MCPS_DATA_indication_pset_t;

// MLME

typedef struct MLME_ASSOCIATE_indication_pset
{
    u8_t            DeviceAddress[8];
    u8_t            CapabilityInformation;
    SecSpec_t      Security;
} MLME_ASSOCIATE_indication_pset_t;

typedef struct MLME_ASSOCIATE_confirm_pset
{
    u8_t            AssocShortAddress[2];
    u8_t            Status;
    SecSpec_t       Security;
} MLME_ASSOCIATE_confirm_pset_t;

typedef struct MLME_DISASSOCIATE_confirm_pset
{
    u8_t            Status;
    FullAddr_t      Address;
} MLME_DISASSOCIATE_confirm_pset_t;

typedef struct MLME_DISASSOCIATE_indication_pset
{
    u8_t            DevAddr[8];
    u8_t            Reason;
    SecSpec_t       Security;
} MLME_DISASSOCIATE_indication_pset_t;


typedef struct MLME_BEACON_NOTIFY_indication_pset
{
    u8_t            BSN;
    PanDescriptor_t PanDescriptor;    /* variable size and so following
                                         fields have to be dealt with
                                         separately */
    /* u8_t            PendAddrSpec; */
    /* variable        Address List  */
    /* variable        Beacon payload */
}  MLME_BEACON_NOTIFY_indication_pset_t;

typedef struct MLME_GET_confirm_pset
{
    u8_t            Status;
    u8_t            PIBAttribute;
    u8_t            PIBAttributeIndex;
    u8_t            PIBAttributeLength;
    u8_t            PIBAttributeValue[MAX_ATTRIBUTE_SIZE];
} MLME_GET_confirm_pset_t;

#define MLME_GET_CONFIRM_BASE_SIZE  (sizeof(MLME_GET_confirm_pset_t)-MAX_ATTRIBUTE_SIZE)

#define DEFAULT_RESULT_LIST_SIZE    (16)
typedef struct MLME_SCAN_confirm_pset
{
    u8_t            Status;
    u8_t            ScanType;
    u8_t            UnscannedChannels[4];
    u8_t            ResultListSize;
    u8_t            ResultList[DEFAULT_RESULT_LIST_SIZE];
} MLME_SCAN_confirm_pset_t;

typedef struct MLME_COMM_STATUS_indication_pset
{
    u8_t            PANId[2];
    u8_t            SrcAddrMode;
    u8_t            SrcAddr[8];
    u8_t            DstAddrMode;
    u8_t            DstAddr[8];
    u8_t            Status;
    SecSpec_t       Security;
} MLME_COMM_STATUS_indication_pset_t;

typedef struct MLME_ORPHAN_indication_pset
{
    u8_t            OrphanAddr[8];
    SecSpec_t       Security;
} MLME_ORPHAN_indication_pset_t;

typedef struct MLME_SYNC_LOSS_indication_pset
{
    u8_t            LossReason;
    u8_t            PANId[2];
    u8_t            LogicalChannel;
    SecSpec_t       Security;
}  MLME_SYNC_LOSS_indication_pset_t;

// HWME

typedef struct HWME_SET_confirm_pset
{
    u8_t            Status;
    u8_t            HWAttribute;
} HWME_SET_confirm_pset_t;

typedef struct HWME_GET_confirm_pset
{
    u8_t            Status;
    u8_t            HWAttribute;
    u8_t            HWAttributeLength;
    u8_t            HWAttributeValue[MAX_HWME_ATTRIBUTE_SIZE];
} HWME_GET_confirm_pset_t;

typedef struct HWME_HAES_confirm_pset
{
    u8_t            Status;
    u8_t            HAESData[16];
} HWME_HAES_confirm_pset_t;

typedef struct HWME_WAKEUP_indication_pset
{
    u8_t            WakeUpCondition;
} HWME_WAKEUP_indication_pset_t;

// TDME

typedef struct TDME_SETSFR_confirm_pset
{
    u8_t            Status;
    u8_t            SFRPage;
    u8_t            SFRAddress;
} TDME_SETSFR_confirm_pset_t;

typedef struct TDME_GETSFR_confirm_pset
{
    u8_t            Status;
    u8_t            SFRPage;
    u8_t            SFRAddress;
    u8_t            SFRValue;
} TDME_GETSFR_confirm_pset_t;

typedef struct TDME_TESTMODE_confirm_pset
{
    u8_t            Status;
    u8_t            TestMode;
} TDME_TESTMODE_confirm_pset_t;

typedef struct TDME_SET_confirm_pset
{
    u8_t            Status;
    u8_t            TDAttribute;
} TDME_SET_confirm_pset_t;

typedef struct TDME_TXPKT_confirm_pset
{
    u8_t            Status;
    u8_t            TestPacketSequenceNumber;
    u8_t            TestPacketLength;
    u8_t            TestPacketData[128];
} TDME_TXPKT_confirm_pset_t;

typedef struct TDME_RXPKT_indication_pset
{
    u8_t            Status;
    u8_t            TestPacketEDValue;
    u8_t            TestPacketCSValue;
    u8_t            TestPacketFoffsValue;
    u8_t            TestPacketLength;
    u8_t            TestPacketData[128];
} TDME_RXPKT_indication_pset_t;

typedef struct TDME_EDDET_indication_pset
{
    u8_t            TestEDThreshold;
    u8_t            TestEDValue;
    u8_t            TestCSValue;
    u8_t            TestTimeAboveThreshold_us[2];
} TDME_EDDET_indication_pset_t;

typedef struct TDME_ERROR_indication_pset
{
    u8_t            ErrorCode;
} TDME_ERROR_indication_pset_t;

typedef struct TDME_LOTLK_confirm_pset
{
    u8_t            Status;
    u8_t            TestChannel;
    u8_t            TestRxTxb;
    u8_t            TestLOFDACValue;
    u8_t            TestLOAMPValue;
    u8_t            TestLOTXCALValue;
} TDME_LOTLK_confirm_pset_t;


/******************************************************************************/
/****** Security PIB Table Size Definitions                              ******/
/******************************************************************************/
#define KEY_TABLE_SIZE                  (2)
#define LOOKUP_DESC_TABLE_SIZE          (5)
#define KEY_DEVICE_TABLE_SIZE           (5)
#define KEY_USAGE_TABLE_SIZE            (12)
#define SECURITY_LEVEL_TABLE_SIZE       (2)
#define DEVICE_TABLE_SIZE               (5)


/******************************************************************************/
/****** Structures used by Security PIB Attributes                       ******/
/******************************************************************************/

typedef struct M_KeyIdLookupDesc
{
    u8_t      LookupData[9];
    u8_t      LookupDataSizeCode;
} M_KeyIdLookupDesc_t;

typedef struct M_DeviceDescriptor
{
    u8_t      PANId[2];
    u8_t      ShortAddress[2];
    u8_t      ExtAddress[8];
    u8_t      FrameCounter[4];
    u8_t      Exempt;
} M_DeviceDescriptor_t;

typedef struct M_SecurityLevelDescriptor
{
    u8_t    FrameType;
    u8_t    CommandFrameIdentifier;
    u8_t    SecurityMinimum;
    u8_t    DeviceOverrideSecurityMinimum;
} M_SecurityLevelDescriptor_t;

typedef struct M_KeyDeviceDesc
{
    //u8_t      DeviceDescriptorHandle : 6;
    //u8_t      UniqueDevice : 1;
    //u8_t      Blacklisted : 1;
    u8_t      Flags;

/* Masks for KeyDeviceDesc Flags*/
#define KDD_DeviceDescHandleMask        (0x3F)
#define KDD_UniqueDeviceMask            (0x40)
#define KDD_BlacklistedMask             (0x80)
} M_KeyDeviceDesc_t;

typedef struct M_KeyUsageDesc
{
    //u8_t      FrameType : 2;
    //u8_t      CommandFrameIdentifier : 4;
    u8_t      Flags;
/* Masks for KeyUsageDesc Flags*/
#define KUD_FrameTypeMask               (0x03)
#define KUD_CommandFrameIdentifierMask  (0xF0)
#define KUD_CommandFrameIdentifierShift (4)
} M_KeyUsageDesc_t;

typedef struct M_KeyTableEntryFixed_s
{
    u8_t    KeyIdLookupListEntries;
    u8_t    KeyDeviceListEntries;
    u8_t    KeyUsageListEntries;
    u8_t    Key[16];
} M_KeyTableEntryFixed_t;

typedef struct M_KeyDescriptor
{
    M_KeyTableEntryFixed_t    Fixed;
    M_KeyIdLookupDesc_t       KeyIdLookupList[LOOKUP_DESC_TABLE_SIZE];
    M_KeyDeviceDesc_t         KeyDeviceList[KEY_DEVICE_TABLE_SIZE];
    M_KeyUsageDesc_t          KeyUsageList[SECURITY_LEVEL_TABLE_SIZE];
} M_KeyDescriptor_t;


/******************************************************************************/
/****** Message ID Codes in SPI Commands                                 ******/
/******************************************************************************/
// Downstream
#define MCPS_DATA_REQUEST                     (0x00)
#define MCPS_PURGE_REQUEST                    (0x01)
#define MLME_ASSOCIATE_REQUEST                (0x02)
#define MLME_ASSOCIATE_RESPONSE               (0x03)
#define MLME_DISASSOCIATE_REQUEST             (0x04)
#define MLME_GET_REQUEST                      (0x05)
#define MLME_ORPHAN_RESPONSE                  (0x06)
#define MLME_RESET_REQUEST                    (0x07)
#define MLME_RX_ENABLE_REQUEST                (0x08)
#define MLME_SCAN_REQUEST                     (0x09)
#define MLME_SET_REQUEST                      (0x0A)
#define MLME_START_REQUEST                    (0x0B)
#define MLME_SYNC_REQUEST                     (0x0C)
#define MLME_POLL_REQUEST                     (0x0D)
#define HWME_SET_REQUEST                      (0x0E)
#define HWME_GET_REQUEST                      (0x0F)
#define HWME_HAES_REQUEST                     (0x10)
#define TDME_SETSFR_REQUEST                   (0x11)
#define TDME_GETSFR_REQUEST                   (0x12)
#define TDME_TESTMODE_REQUEST                 (0x13)
#define TDME_SET_REQUEST                      (0x14)
#define TDME_TXPKT_REQUEST                    (0x15)
#define TDME_LOTLK_REQUEST                    (0x16)
// Upstream
#define MCPS_DATA_INDICATION                  (0x00)
#define MCPS_DATA_CONFIRM                     (0x01)
#define MCPS_PURGE_CONFIRM                    (0x02)
#define MLME_ASSOCIATE_INDICATION             (0x03)
#define MLME_ASSOCIATE_CONFIRM                (0x04)
#define MLME_DISASSOCIATE_INDICATION          (0x05)
#define MLME_DISASSOCIATE_CONFIRM             (0x06)
#define MLME_BEACON_NOTIFY_INDICATION         (0x07)
#define MLME_GET_CONFIRM                      (0x08)
#define MLME_ORPHAN_INDICATION                (0x09)
#define MLME_RESET_CONFIRM                    (0x0A)
#define MLME_RX_ENABLE_CONFIRM                (0x0B)
#define MLME_SCAN_CONFIRM                     (0x0C)
#define MLME_COMM_STATUS_INDICATION           (0x0D)
#define MLME_SET_CONFIRM                      (0x0E)
#define MLME_START_CONFIRM                    (0x0F)
#define MLME_SYNC_LOSS_INDICATION             (0x10)
#define MLME_POLL_CONFIRM                     (0x11)
#define HWME_SET_CONFIRM                      (0x12)
#define HWME_GET_CONFIRM                      (0x13)
#define HWME_HAES_CONFIRM                     (0x14)
#define HWME_WAKEUP_INDICATION                (0x15)
#define TDME_MESSAGE_INDICATION               (0x16)
#define TDME_SETSFR_CONFIRM                   (0x17)
#define TDME_GETSFR_CONFIRM                   (0x18)
#define TDME_TESTMODE_CONFIRM                 (0x19)
#define TDME_SET_CONFIRM                      (0x1A)
#define TDME_TXPKT_CONFIRM                    (0x1B)
#define TDME_RXPKT_INDICATION                 (0x1C)
#define TDME_EDDET_INDICATION                 (0x1D)
#define TDME_ERROR_INDICATION                 (0x1E)
#define TDME_LOTLK_CONFIRM                    (0x1F)


/******************************************************************************/
/****** SPI Message Format Typedef                                       ******/
/******************************************************************************/

typedef struct MAC_Message_s
{
    u8_t      CommandId;
    u8_t      Length;
    union
    {
        // MAC MCPS / MLME
        MCPS_DATA_request_pset_t                      DataReq;
        MLME_ASSOCIATE_request_pset_t                 AssocReq;
        MLME_ASSOCIATE_response_pset_t                AssocRsp;
        MLME_DISASSOCIATE_request_pset_t              DisassocReq;
        MLME_GET_request_pset_t                       GetReq;
        MLME_ORPHAN_response_pset_t                   OrphanRsp;
        MLME_POLL_request_pset_t                      PollReq;
        MLME_RX_ENABLE_request_pset_t                 RxEnableReq;
        MLME_SCAN_request_pset_t                      ScanReq;
        MLME_SET_request_pset_t                       SetReq;
        MLME_START_request_pset_t                     StartReq;
        MCPS_DATA_confirm_pset_t                      DataCnf;
        MCPS_PURGE_confirm_pset_t                     PurgeCnf;
        MCPS_DATA_indication_pset_t                   DataInd;
        MLME_ASSOCIATE_indication_pset_t              AssocInd;
        MLME_ASSOCIATE_confirm_pset_t                 AssocCnf;
        MLME_DISASSOCIATE_indication_pset_t           DisassocInd;
        MLME_DISASSOCIATE_confirm_pset_t              DisassocCnf;
        MLME_BEACON_NOTIFY_indication_pset_t          BeaconInd;
        MLME_GET_confirm_pset_t                       GetCnf;
        MLME_SCAN_confirm_pset_t                      ScanCnf;
        MLME_COMM_STATUS_indication_pset_t            CommStatusInd;
        MLME_SYNC_LOSS_indication_pset_t              SyncLossInd;
        MLME_ORPHAN_indication_pset_t                 OrphanInd;
        // HWME
        HWME_SET_request_pset_t                       HWMESetReq;
        HWME_GET_request_pset_t                       HWMEGetReq;
        HWME_HAES_request_pset_t                      HWMEHAESReq;
        HWME_SET_confirm_pset_t                       HWMESetCnf;
        HWME_GET_confirm_pset_t                       HWMEGetCnf;
        HWME_HAES_confirm_pset_t                      HWMEHAESCnf;
        HWME_WAKEUP_indication_pset_t                 HWMEWakeupInd;
        // TDME
        TDME_SETSFR_request_pset_t                    TDMESetSFRReq;
        TDME_GETSFR_request_pset_t                    TDMEGetSFRReq;
        TDME_TESTMODE_request_pset_t                  TDMETestModeReq;
        TDME_SET_request_pset_t                       TDMESetReq;
        TDME_TXPKT_request_pset_t                     TDMETxPktReq;
        TDME_LOTLK_request_pset_t                     TDMELOTlkReq;
        TDME_SETSFR_confirm_pset_t                    TDMESetSFRCnf;
        TDME_GETSFR_confirm_pset_t                    TDMEGetSFRCnf;
        TDME_TESTMODE_confirm_pset_t                  TDMETestModeCnf;
        TDME_SET_confirm_pset_t                       TDMESetCnf;
        TDME_TXPKT_confirm_pset_t                     TDMETxPktCnf;
        TDME_RXPKT_indication_pset_t                  TDMERxPktInd;
        TDME_EDDET_indication_pset_t                  TDMEEDDetInd;
        TDME_ERROR_indication_pset_t                  TDMEErrorInd;
        TDME_LOTLK_confirm_pset_t                     TDMELOTlkCnf;
        // Common
        u8_t                                          u8Param;
        u8_t                                          Status;
        u8_t                                          Payload[254];
    }   PData;
} MAC_Message_t;


/******************************************************************************/
/****** SPI Command IDs                                                  ******/
/******************************************************************************/
// Mask to derive the Message ID Code from the Command ID
#define SPI_MID_MASK                          (0x1F)
// Bit indicating a Confirm or Indication from Slave to Master
#define SPI_S2M                               (0x20)
// Bit indicating a Synchronous Message
#define SPI_SYN                               (0x40)


/******************************************************************************/
/****** SPI Command Definitions                                          ******/
/******************************************************************************/
// Idle Mode - No Data
#define SPI_IDLE                           (0xFF)
// NACK - Buffer full or busy - resend Request
#define SPI_NACK                           (0xF0)
// MAC MCPS
#define SPI_MCPS_DATA_REQUEST              (MCPS_DATA_REQUEST)
#define SPI_MCPS_PURGE_REQUEST             (MCPS_PURGE_REQUEST+SPI_SYN)
#define SPI_MCPS_DATA_INDICATION           (MCPS_DATA_INDICATION+SPI_S2M)
#define SPI_MCPS_DATA_CONFIRM              (MCPS_DATA_CONFIRM+SPI_S2M)
#define SPI_MCPS_PURGE_CONFIRM             (MCPS_PURGE_CONFIRM+SPI_S2M+SPI_SYN)
// MAC MLME
#define SPI_MLME_ASSOCIATE_REQUEST         (MLME_ASSOCIATE_REQUEST)
#define SPI_MLME_ASSOCIATE_RESPONSE        (MLME_ASSOCIATE_RESPONSE)
#define SPI_MLME_DISASSOCIATE_REQUEST      (MLME_DISASSOCIATE_REQUEST)
#define SPI_MLME_GET_REQUEST               (MLME_GET_REQUEST+SPI_SYN)
#define SPI_MLME_ORPHAN_RESPONSE           (MLME_ORPHAN_RESPONSE)
#define SPI_MLME_RESET_REQUEST             (MLME_RESET_REQUEST+SPI_SYN)
#define SPI_MLME_RX_ENABLE_REQUEST         (MLME_RX_ENABLE_REQUEST+SPI_SYN)
#define SPI_MLME_SCAN_REQUEST              (MLME_SCAN_REQUEST)
#define SPI_MLME_SET_REQUEST               (MLME_SET_REQUEST+SPI_SYN)
#define SPI_MLME_START_REQUEST             (MLME_START_REQUEST+SPI_SYN)
#define SPI_MLME_SYNC_REQUEST              (MLME_SYNC_REQUEST)
#define SPI_MLME_POLL_REQUEST              (MLME_POLL_REQUEST+SPI_SYN)
#define SPI_MLME_ASSOCIATE_INDICATION      (MLME_ASSOCIATE_INDICATION+SPI_S2M)
#define SPI_MLME_ASSOCIATE_CONFIRM         (MLME_ASSOCIATE_CONFIRM+SPI_S2M)
#define SPI_MLME_DISASSOCIATE_INDICATION   (MLME_DISASSOCIATE_INDICATION+SPI_S2M)
#define SPI_MLME_DISASSOCIATE_CONFIRM      (MLME_DISASSOCIATE_CONFIRM+SPI_S2M)
#define SPI_MLME_BEACON_NOTIFY_INDICATION  (MLME_BEACON_NOTIFY_INDICATION+SPI_S2M)
#define SPI_MLME_GET_CONFIRM               (MLME_GET_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_MLME_ORPHAN_INDICATION         (MLME_ORPHAN_INDICATION+SPI_S2M)
#define SPI_MLME_RESET_CONFIRM             (MLME_RESET_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_MLME_RX_ENABLE_CONFIRM         (MLME_RX_ENABLE_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_MLME_SCAN_CONFIRM              (MLME_SCAN_CONFIRM+SPI_S2M)
#define SPI_MLME_COMM_STATUS_INDICATION    (MLME_COMM_STATUS_INDICATION+SPI_S2M)
#define SPI_MLME_SET_CONFIRM               (MLME_SET_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_MLME_START_CONFIRM             (MLME_START_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_MLME_SYNC_LOSS_INDICATION      (MLME_SYNC_LOSS_INDICATION+SPI_S2M)
#define SPI_MLME_POLL_CONFIRM              (MLME_POLL_CONFIRM+SPI_S2M+SPI_SYN)
// HWME
#define SPI_HWME_SET_REQUEST               (HWME_SET_REQUEST+SPI_SYN)
#define SPI_HWME_GET_REQUEST               (HWME_GET_REQUEST+SPI_SYN)
#define SPI_HWME_HAES_REQUEST              (HWME_HAES_REQUEST+SPI_SYN)
#define SPI_HWME_SET_CONFIRM               (HWME_SET_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_HWME_GET_CONFIRM               (HWME_GET_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_HWME_HAES_CONFIRM              (HWME_HAES_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_HWME_WAKEUP_INDICATION         (HWME_WAKEUP_INDICATION+SPI_S2M)
// TDME
#define SPI_TDME_SETSFR_REQUEST            (TDME_SETSFR_REQUEST+SPI_SYN)
#define SPI_TDME_GETSFR_REQUEST            (TDME_GETSFR_REQUEST+SPI_SYN)
#define SPI_TDME_TESTMODE_REQUEST          (TDME_TESTMODE_REQUEST+SPI_SYN)
#define SPI_TDME_SET_REQUEST               (TDME_SET_REQUEST+SPI_SYN)
#define SPI_TDME_TXPKT_REQUEST             (TDME_TXPKT_REQUEST+SPI_SYN)
#define SPI_TDME_LOTLK_REQUEST             (TDME_LOTLK_REQUEST+SPI_SYN)
#define SPI_TDME_MESSAGE_INDICATION        (TDME_MESSAGE_INDICATION+SPI_S2M)
#define SPI_TDME_SETSFR_CONFIRM            (TDME_SETSFR_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_TDME_GETSFR_CONFIRM            (TDME_GETSFR_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_TDME_TESTMODE_CONFIRM          (TDME_TESTMODE_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_TDME_SET_CONFIRM               (TDME_SET_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_TDME_TXPKT_CONFIRM             (TDME_TXPKT_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_TDME_RXPKT_INDICATION          (TDME_RXPKT_INDICATION+SPI_S2M)
#define SPI_TDME_EDDET_INDICATION          (TDME_EDDET_INDICATION+SPI_S2M)
#define SPI_TDME_ERROR_INDICATION          (TDME_ERROR_INDICATION+SPI_S2M)
#define SPI_TDME_LOTLK_CONFIRM             (TDME_LOTLK_CONFIRM+SPI_S2M+SPI_SYN)


#endif  // MAC_MESSAGES_H
