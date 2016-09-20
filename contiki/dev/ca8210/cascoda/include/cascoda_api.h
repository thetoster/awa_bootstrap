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

#include "dev/ca8210/cascoda/include/cascoda_types.h"
#include "dev/ca8210/cascoda/include/mac_messages.h"

#ifndef CASCODA_API_H
#define CASCODA_API_H


/******************************************************************************/
/****** MAC MCPS/MLME Downlink                                           ******/
/******************************************************************************/

u8_t MCPS_DATA_request
(
    u8_t         SrcAddrMode,
    u8_t         DstAddrMode,
    u16_t        DstPANId,
    MacAddr_t   *pDstAddr,
    u8_t         MsduLength,
    u8_t        *pMsdu,
    u8_t         MsduHandle,
    u8_t         TxOptions,
    SecSpec_t   *pSecurity);

u8_t MCPS_PURGE_request_sync(
    u8_t        *MsduHandle);

u8_t MLME_ASSOCIATE_request
(
    u8_t         LogicalChannel,
    u8_t         DstAddrMode,
    u16_t        DstPANId,
    MacAddr_t   *pDstAddr,
    u8_t         CapabilityInfo,
    SecSpec_t   *pSecurity);

u8_t MLME_ASSOCIATE_response
(
    u8_t        *pDeviceAddress,
    u16_t        AssocShortAddress,
    u8_t         Status,
    SecSpec_t   *pSecurity);

u8_t MLME_DISASSOCIATE_request
(
    FullAddr_t   DevAddr,
    u8_t         DisassociateReason,
    u8_t         TxIndirect,
    SecSpec_t   *pSecurity);

u8_t MLME_GET_request_sync
(
    u8_t         PIBAttribute,
    u8_t         PIBAttributeIndex,
    u8_t        *pPIBAttributeLength,
    void        *pPIBAttributeValue);

u8_t MLME_ORPHAN_response
(
    u8_t        *pOrphanAddress,
    u16_t        ShortAddress,
    u8_t         AssociatedMember,
    SecSpec_t   *pSecurity);

u8_t MLME_RESET_request_sync
(
    u8_t         SetDefaultPIB);

u8_t MLME_RX_ENABLE_request_sync
(
    u8_t         DeferPermit,
    u32_t        RxOnTime,
    u32_t        RxOnDuration);

u8_t MLME_SCAN_request
(
    u8_t         ScanType,
    u32_t        ScanChannels,
    u8_t         ScanDuration,
    SecSpec_t   *pSecurity);

u8_t MLME_SET_request_sync
(
    u8_t         PIBAttribute,
    u8_t         PIBAttributeIndex,
    u8_t         PIBAttributeLength,
    const void   *pPIBAttributeValue);

u8_t MLME_START_request_sync
(
    u16_t        PANId,
    u8_t         LogicalChannel,
    u8_t         BeaconOrder,
    u8_t         SuperframeOrder,
    u8_t         PANCoordinator,
    u8_t         BatteryLifeExtension,
    u8_t         CoordRealignment,
    SecSpec_t   *pCoordRealignSecurity,
    SecSpec_t   *pBeaconSecurity);

u8_t MLME_POLL_request_sync
(
    FullAddr_t   CoordAddress,
    u8_t         Interval[2],    /* polling interval in 0.1 seconds res */
                                 /* 0 means poll once */
                                 /* 0xFFFF means stop polling */
    SecSpec_t   *pSecurity);


/******************************************************************************/
/****** MAC MCPS/MLME Uplink                                             ******/
/******************************************************************************/

u8_t MCPS_DATA_indication
(
    FullAddr_t  *Src,
    FullAddr_t  *Dst,
    u8_t        *MsduLength,
    u8_t        *MpduLinkQuality,
    u8_t        *DSN,
    u32_t       *pTimeStamp,
    u8_t        *pMsdu,
    SecSpec_t   *pSecurity);

u8_t MCPS_DATA_confirm
(
    u8_t        *MsduHandle,
    u32_t       *pTimeStamp);

u8_t MLME_ASSOCIATE_indication
(
    u8_t        *pDeviceAddress,
    u8_t        *CapabilityInfo,
    SecSpec_t   *pSecurity);

u8_t MLME_ASSOCIATE_confirm
(
    u8_t        *pAssocShortAddress,
    SecSpec_t   *pSecurity);

u8_t MLME_DISASSOCIATE_indication
(
    u8_t        *pDevAddr,
    u8_t        *Reason,
    SecSpec_t   *pSecurity);

u8_t MLME_DISASSOCIATE_confirm
(
    FullAddr_t  *Address);

u8_t MLME_BEACON_NOTIFY_indication
(
    u8_t            *BSN,
    PanDescriptor_t *pPanDescriptor,
    u8_t            *PendAddrSpec,
    u8_t            *pAddressListShort,
    u8_t            *pAddressListLong,
    u8_t            *SduLength,
    u8_t            *pSdu);

u8_t MLME_ORPHAN_indication
(
    u8_t        *pOrphanAddr,
    SecSpec_t   *pSecurity);

u8_t MLME_SCAN_confirm
(
    u8_t        *ScanType,
    u8_t        *pUnscannedChannels,
    u8_t        *ResultListSize,
    u8_t        *pResultList);

u8_t MLME_COMM_STATUS_indication
(
    u8_t        *pPANId,
    u8_t        *SrcAddrMode,
    u8_t        *pSrcAddr,
    u8_t        *DstAddrMode,
    u8_t        *pDstAddr,
    SecSpec_t   *pSecurity);

u8_t MLME_SYNC_LOSS_indication
(
    u8_t        *LossReason,
    u8_t        *pPANId,
    u8_t        *LogicalChannel,
    SecSpec_t   *pSecurity);


/******************************************************************************/
/****** HWME Downlink                                                    ******/
/******************************************************************************/

u8_t HWME_SET_request_sync
(
    u8_t         HWAttribute,
    u8_t         HWAttributeLength,
    u8_t        *pHWAttributeValue);

u8_t HWME_GET_request_sync
(
    u8_t         HWAttribute,
    u8_t        *HWAttributeLength,
    u8_t        *pHWAttributeValue);

u8_t HWME_HAES_request_sync
(
    u8_t         HAESMode,
    u8_t        *pHAESData);


/******************************************************************************/
/****** HWME Uplink                                                      ******/
/******************************************************************************/

u8_t HWME_WAKEUP_indication
(
    u8_t        *WakeUpCondition);


/******************************************************************************/
/****** TDME Downlink                                                    ******/
/******************************************************************************/

u8_t TDME_SETSFR_request_sync
(
    u8_t         SFRPage,
    u8_t         SFRAddress,
    u8_t         SFRValue);

u8_t TDME_GETSFR_request_sync
(
    u8_t         SFRPage,
    u8_t         SFRAddress,
    u8_t        *SFRValue);

u8_t TDME_TESTMODE_request_sync
(
    u8_t         TestMode);

u8_t TDME_SET_request_sync
(
    u8_t         TestAttribute,
    u8_t         TestAttributeLength,
    void        *pTestAttributeValue);

u8_t TDME_TXPKT_request_sync
(
    u8_t         TestPacketDataType,
    u8_t        *TestPacketSequenceNumber,
    u8_t        *TestPacketLength,
    void        *pTestPacketData);

u8_t TDME_LOTLK_request_sync
(
    u8_t        *TestChannel,
    u8_t        *TestRxTxb,
    u8_t        *TestLOFDACValue,
    u8_t        *TestLOAMPValue,
    u8_t        *TestLOTXCALValue);


/******************************************************************************/
/****** TDME Uplink                                                      ******/
/******************************************************************************/

u8_t TDME_RXPKT_indication
(
    u8_t        *TestPacketEDValue,
    u8_t        *TestPacketCSValue,
    u8_t        *TestPacketFoffsValue,
    u8_t        *TestPacketLength,
    void        *TestPacketData);

u8_t TDME_EDDET_indication
(
    u8_t        *TestEDThreshold,
    u8_t        *TestEDValue,
    u8_t        *TestCSValue,
    void        *TestTimeAboveThreshold_us);

u8_t TDME_ERROR_indication
(
    u8_t        *ErrorCode);


/******************************************************************************/
/****** TDME Register Default Initialisation and Checking Functions      ******/
/******************************************************************************/
u8_t TDME_ChipInit(void);
u8_t TDME_ChannelInit(u8_t channel);
u8_t TDME_CheckPIBAttribute
(
    u8_t         PIBAttribute,
    u8_t         PIBAttributeLength,
    const void   *pPIBAttributeValue);

u8_t TDME_SetTxPower(u8_t txp);
u8_t TDME_GetTxPower(u8_t *txp);

/******************************************************************************/
/****** MAC Workarounds for V1.1 and MPW silicon (V0.x)                  ******/
/******************************************************************************/
extern u8_t MAC_Workarounds;
extern u8_t MAC_MPW;


#endif // CASCODA_API_H
