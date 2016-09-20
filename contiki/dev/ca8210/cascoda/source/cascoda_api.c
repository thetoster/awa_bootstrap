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
#include "dev/ca8210/cascoda/include/cascoda_types.h"
#include "dev/ca8210/cascoda/include/mac_messages.h"
#include "dev/ca8210/cascoda/include/cascoda_spi.h"
#include "dev/ca8210/cascoda/include/cascoda_api.h"
#include "ca8210-conf.h"


/******************************************************************************/
/****** Shortform References to Requests and Responses in SPI_Transmit   ******/
/******************************************************************************/
#define SIMPLEREQ   (SPI_Transmit.PData)
#define DATAREQ     (SPI_Transmit.PData.DataReq)
#define ASSOCREQ    (SPI_Transmit.PData.AssocReq)
#define ASSOCRSP    (SPI_Transmit.PData.AssocRsp)
#define DISASSOCREQ (SPI_Transmit.PData.DisassocReq)
#define GETREQ      (SPI_Transmit.PData.GetReq)
#define ORPHANRSP   (SPI_Transmit.PData.OrphanRsp)
#define POLLREQ     (SPI_Transmit.PData.PollReq)
#define RXENABLEREQ (SPI_Transmit.PData.RxEnableReq)
#define SCANREQ     (SPI_Transmit.PData.ScanReq)
#define SETREQ      (SPI_Transmit.PData.SetReq)
#define STARTREQ    (SPI_Transmit.PData.StartReq)


/******************************************************************************/
/****** Shortform References to Requests and Responses in SPI_Receive    ******/
/******************************************************************************/
#define SIMPLECNF   (SPI_Receive.PData)
#define GETCNF      (SPI_Receive.PData.GetCnf)


/******************************************************************************/
/****** MAC Workarounds for V1.1 and MPW silicon (V0.x)                  ******/
/******************************************************************************/
u8_t MAC_Workarounds = 0;
u8_t MAC_MPW         = 0;


/******************************************************************************/
/******************************************************************************/
/****** MCPS_DATA_request()                                              ******/
/******************************************************************************/
/****** Brief:  MCPS_DATA_request (Send Data) according to API Spec      ******/
/******************************************************************************/
/****** Param:  SrcAddrMode - Source Addressing Mode                     ******/
/****** Param:  DstAddrMode - Destination Addressing Mode                ******/
/****** Param:  DstPANId - Destination PAN ID                            ******/
/****** Param:  pDstAddr - Pointer to Destination Address                ******/
/****** Param:  MsduLength - Length of Data                              ******/
/****** Param:  pMsdu - Pointer to Data                                  ******/
/****** Param:  MsduHandle - Handle of Data                              ******/
/****** Param:  TxOptions - Tx Options Bit Field                         ******/
/****** Param:  pSecurity - Pointer to Security Structure or NULLP       ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
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
    SecSpec_t   *pSecurity
)
{
    SecSpec_t   *pSec;

    SPI_Transmit.CommandId = SPI_MCPS_DATA_REQUEST;
    DATAREQ.SrcAddrMode = SrcAddrMode;
    DATAREQ.Dst.AddressMode = DstAddrMode;
    if ( DstAddrMode != MAC_MODE_NO_ADDR )
    {
        DATAREQ.Dst.PANId[0] = LS_BYTE( DstPANId );
        DATAREQ.Dst.PANId[1] = MS_BYTE( DstPANId );
        if ( DstAddrMode == MAC_MODE_SHORT_ADDR )
        {
            DATAREQ.Dst.Address[0] = LS_BYTE( pDstAddr->ShortAddress );
            DATAREQ.Dst.Address[1] = MS_BYTE( pDstAddr->ShortAddress );
        }
        else
        {   // MAC_MODE_LONG_ADDR
            memcpy( DATAREQ.Dst.Address, pDstAddr->IEEEAddress, 8 );
        }
    }
    DATAREQ.MsduLength = MsduLength;
    DATAREQ.MsduHandle = MsduHandle;
    DATAREQ.TxOptions = TxOptions;
    memcpy( DATAREQ.Msdu, pMsdu, MsduLength );
    pSec = (SecSpec_t *)( DATAREQ.Msdu + MsduLength );
    SPI_Transmit.Length = sizeof( MCPS_DATA_request_pset_t ) - MAX_DATA_SIZE
                     + MsduLength;
    if ( ( pSecurity == NULLP ) || ( pSecurity->SecurityLevel == 0 ) )
    {
        pSec->SecurityLevel = 0;
        SPI_Transmit.Length += 1;
    }
    else
    {
        *pSec = *pSecurity;
        SPI_Transmit.Length += sizeof(SecSpec_t);
    }

    if(SPI_Send())
        return MAC_SYSTEM_ERROR;

    return(MAC_SUCCESS);
} // End of MCPS_DATA_request()


/******************************************************************************/
/******************************************************************************/
/****** MCPS_PURGE_request_sync()                                        ******/
/******************************************************************************/
/****** Brief:  MCPS_PURGE_request/confirm according to API Spec         ******/
/******************************************************************************/
/****** Param:  MsduHandle - Handle of Data                              ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t MCPS_PURGE_request_sync(
    u8_t        *MsduHandle
)
{
    SPI_Transmit.CommandId = SPI_MCPS_PURGE_REQUEST;
    SPI_Transmit.Length = 1;
    SIMPLEREQ.u8Param = *MsduHandle;

    CA8210_IRQN_DISABLE();
    if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
    CA8210_IRQN_ENABLE();

    if ( SPI_Receive.CommandId != SPI_MCPS_PURGE_CONFIRM )
        return MAC_SYSTEM_ERROR;

    *MsduHandle = SPI_Receive.PData.PurgeCnf.MsduHandle;

    SPI_Receive.CommandId = SPI_IDLE;
    return SPI_Receive.PData.PurgeCnf.Status;
} // End of MCPS_PURGE_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** MLME_ASSOCIATE_request()                                         ******/
/******************************************************************************/
/****** Brief:  MLME_ASSOCIATE_request according to API Spec             ******/
/******************************************************************************/
/****** Param:  LogicalChannel - Channel Number                          ******/
/****** Param:  DstAddrMode - Destination Addressing Mode                ******/
/****** Param:  DstPANId - Destination PAN ID                            ******/
/****** Param:  pDstAddr - Pointer to Destination Address                ******/
/****** Param:  CapabilityInfo - Bitmap of operational Capabilities      ******/
/****** Param:  pSecurity - Pointer to Security Structure or NULLP       ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_ASSOCIATE_request
(
    u8_t         LogicalChannel,
    u8_t         DstAddrMode,
    u16_t        DstPANId,
    MacAddr_t   *pDstAddr,
    u8_t         CapabilityInfo,
    SecSpec_t   *pSecurity
)
{
    TDME_ChannelInit(LogicalChannel);
    SPI_Transmit.CommandId = SPI_MLME_ASSOCIATE_REQUEST;
    SPI_Transmit.Length = sizeof( MLME_ASSOCIATE_request_pset_t );
    ASSOCREQ.LogicalChannel = LogicalChannel;
    ASSOCREQ.Dst.AddressMode = DstAddrMode;
    ASSOCREQ.Dst.PANId[0] = LS_BYTE(DstPANId);
    ASSOCREQ.Dst.PANId[1] = MS_BYTE(DstPANId);
    switch( DstAddrMode )
    {
    case MAC_MODE_SHORT_ADDR:
        ASSOCREQ.Dst.Address[0] = LS_BYTE(pDstAddr->ShortAddress);
        ASSOCREQ.Dst.Address[1] = MS_BYTE(pDstAddr->ShortAddress);
        break;
    case MAC_MODE_LONG_ADDR:
        memcpy( ASSOCREQ.Dst.Address, pDstAddr->IEEEAddress, 8 );
        break;
    default:
        memset( ASSOCREQ.Dst.Address, 0 , 8 );
        break;
    }
    ASSOCREQ.CapabilityInfo = CapabilityInfo;
    if ( ( pSecurity == NULLP ) || ( pSecurity->SecurityLevel == 0 ) )
    {
        ASSOCREQ.Security.SecurityLevel = 0;
        SPI_Transmit.Length -= sizeof(SecSpec_t) - 1;
    }
    else
    {
        ASSOCREQ.Security = *pSecurity;
    }

    if(SPI_Send())
        return MAC_SYSTEM_ERROR;

    return(MAC_SUCCESS);
} // End of MLME_ASSOCIATE_request()


/******************************************************************************/
/******************************************************************************/
/****** MLME_ASSOCIATE_response()                                        ******/
/******************************************************************************/
/****** Brief:  MLME_ASSOCIATE_response according to API Spec            ******/
/******************************************************************************/
/****** Param:  pDeviceAddress - Pointer to IEEE Address                 ******/
/****** Param:  AssocShortAddress - Short Address given to Device        ******/
/****** Param:  Status - Status                                          ******/
/****** Param:  pSecurity - Pointer to Security Structure or NULLP       ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_ASSOCIATE_response
(
    u8_t        *pDeviceAddress,
    u16_t        AssocShortAddress,
    u8_t         Status,
    SecSpec_t   *pSecurity
)
{
    SPI_Transmit.CommandId = SPI_MLME_ASSOCIATE_RESPONSE;
    SPI_Transmit.Length = sizeof( MLME_ASSOCIATE_response_pset_t );
    memcpy(ASSOCRSP.DeviceAddress, pDeviceAddress, 8 );
    ASSOCRSP.AssocShortAddress[0] = LS_BYTE(AssocShortAddress);
    ASSOCRSP.AssocShortAddress[1] = MS_BYTE(AssocShortAddress);
    ASSOCRSP.Status = Status;
    if ( ( pSecurity == NULLP ) || ( pSecurity->SecurityLevel == 0 ) )
    {
        ASSOCRSP.Security.SecurityLevel = 0;
        SPI_Transmit.Length -= sizeof(SecSpec_t) - 1;
    }
    else
    {
        ASSOCRSP.Security = *pSecurity;
    }

    if(SPI_Send())
        return MAC_SYSTEM_ERROR;

    return(MAC_SUCCESS);
} // End of MLME_ASSOCIATE_response()


/******************************************************************************/
/******************************************************************************/
/****** MLME_DISASSOCIATE_request()                                      ******/
/******************************************************************************/
/****** Brief:  MLME_DISASSOCIATE_request according to API Spec          ******/
/******************************************************************************/
/****** Param:  DevAddr - Device Address                                 ******/
/****** Param:  DisassociateReason - Reason for Disassociation           ******/
/****** Param:  TxIndirect - TxIndirect Flag                             ******/
/****** Param:  pSecurity - Pointer to Security Structure or NULLP       ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_DISASSOCIATE_request
(
    FullAddr_t   DevAddr,
    u8_t         DisassociateReason,
    u8_t         TxIndirect,
    SecSpec_t   *pSecurity
)
{
    SPI_Transmit.CommandId = SPI_MLME_DISASSOCIATE_REQUEST;
    SPI_Transmit.Length = sizeof( MLME_DISASSOCIATE_request_pset_t );

    SPI_Transmit.PData.DisassocReq.DevAddr            = DevAddr;
    SPI_Transmit.PData.DisassocReq.DisassociateReason = DisassociateReason;
    SPI_Transmit.PData.DisassocReq.TxIndirect         = TxIndirect;

    if ( ( pSecurity == NULLP ) || ( pSecurity->SecurityLevel == 0 ) )
    {
        SPI_Transmit.PData.DisassocReq.Security.SecurityLevel = 0;
        SPI_Transmit.Length -= sizeof(SecSpec_t) - 1;
    }
    else
    {
        SPI_Transmit.PData.DisassocReq.Security = *pSecurity;
    }

    if(SPI_Send())
        return MAC_SYSTEM_ERROR;

    return(MAC_SUCCESS);
} // End of MLME_DISASSOCIATE_request()


/******************************************************************************/
/******************************************************************************/
/****** MLME_GET_request_sync()                                          ******/
/******************************************************************************/
/****** Brief:  MLME_GET_request/confirm according to API Spec           ******/
/******************************************************************************/
/****** Param:  PIBAttribute - Attribute Number                          ******/
/****** Param:  PIBAttributeIndex - Index within Attribute if an Array   ******/
/****** Param:  pPIBAttributeLength - Pointer to Attribute Length        ******/
/****** Param:  pPIBAttributeValue - Pointer to Attribute Value          ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_GET_request_sync
(
    u8_t         PIBAttribute,
    u8_t         PIBAttributeIndex,
    u8_t        *pPIBAttributeLength,
    void        *pPIBAttributeValue
)
{

    if(PIBAttribute == phyTransmitPower)
     {
     GETCNF.Status = TDME_GetTxPower(((u8_t*)pPIBAttributeValue));
     *pPIBAttributeLength = 1;
     }
    else
     {
     SPI_Transmit.CommandId = SPI_MLME_GET_REQUEST;
     SPI_Transmit.Length = sizeof( MLME_GET_request_pset_t );
     GETREQ.PIBAttribute = PIBAttribute;
     GETREQ.PIBAttributeIndex = PIBAttributeIndex;

     CA8210_IRQN_DISABLE();
     if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
     CA8210_IRQN_ENABLE();

     if ( SPI_Receive.CommandId != SPI_MLME_GET_CONFIRM )
         return MAC_SYSTEM_ERROR;

     if ( GETCNF.Status == MAC_SUCCESS )
      {
         *pPIBAttributeLength = GETCNF.PIBAttributeLength;
         memcpy( pPIBAttributeValue, GETCNF.PIBAttributeValue, GETCNF.PIBAttributeLength );
      }
     }

    SPI_Receive.CommandId = SPI_IDLE;
    return GETCNF.Status;
} // End of MLME_GET_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** MLME_ORPHAN_response()                                           ******/
/******************************************************************************/
/****** Brief:  MLME_ORPHAN_response according to API Spec               ******/
/******************************************************************************/
/****** Param:  pOrphanAddress - Pointer to Orphan IEEE Address          ******/
/****** Param:  ShortAddress - Short Address for Orphan                  ******/
/****** Param:  AssociatedMember - TRUE if associated                    ******/
/****** Param:  pSecurity - Pointer to Security Structure or NULLP       ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_ORPHAN_response
(
    u8_t        *pOrphanAddress,
    u16_t        ShortAddress,
    u8_t         AssociatedMember,
    SecSpec_t   *pSecurity
)
{
    SPI_Transmit.CommandId = SPI_MLME_ORPHAN_RESPONSE;
    SPI_Transmit.Length = sizeof( MLME_ORPHAN_response_pset_t );
    memcpy(ORPHANRSP.OrphanAddress, pOrphanAddress, 8 );
    ORPHANRSP.ShortAddress[0] = LS_BYTE(ShortAddress);
    ORPHANRSP.ShortAddress[1] = MS_BYTE(ShortAddress);
    ORPHANRSP.AssociatedMember = AssociatedMember;
    if ( ( pSecurity == NULLP ) || ( pSecurity->SecurityLevel == 0 ) )
    {
        ORPHANRSP.Security.SecurityLevel = 0;
        SPI_Transmit.Length -= sizeof(SecSpec_t) - 1;
    }
    else
    {
        ORPHANRSP.Security = *pSecurity;
    }

    if(SPI_Send())
        return MAC_SYSTEM_ERROR;

    return(MAC_SUCCESS);
} // End of MLME_ORPHAN_response()


/******************************************************************************/
/******************************************************************************/
/****** MLME_RESET_request_sync()                                        ******/
/******************************************************************************/
/****** Brief:  MLME_RESET_request/confirm according to API Spec         ******/
/******************************************************************************/
/****** Param:  SetDefaultPIB - Set defaults in PIB                      ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_RESET_request_sync
(
    u8_t         SetDefaultPIB
)
{
u8_t status;

    SPI_Transmit.CommandId = SPI_MLME_RESET_REQUEST;
    SPI_Transmit.Length = 1;
    SIMPLEREQ.u8Param = SetDefaultPIB;

    CA8210_IRQN_DISABLE();
    if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
    CA8210_IRQN_ENABLE();

    if ( SPI_Receive.CommandId != SPI_MLME_RESET_CONFIRM )
        return MAC_SYSTEM_ERROR;

    status = SIMPLECNF.Status;

    // reset COORD Bit for Channel Filtering as Coordinator
    if(MAC_Workarounds && SetDefaultPIB && (!status))
     status = TDME_SETSFR_request_sync(0, 0xD8, 0);

    SPI_Receive.CommandId = SPI_IDLE;
    return status;
} // End of MLME_RESET_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** MLME_RX_ENABLE_request_sync()                                    ******/
/******************************************************************************/
/****** Brief:  MLME_RX_ENABLE_request/confirm according to API Spec     ******/
/******************************************************************************/
/****** Param:  DeferPermit - Defer Permit Flag                          ******/
/****** Param:  RxOnTime - Receiver On Time                              ******/
/****** Param:  RxOnDuration - Receiver On Duration                      ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_RX_ENABLE_request_sync
(
    u8_t         DeferPermit,
    u32_t        RxOnTime,
    u32_t        RxOnDuration
)
{
    SPI_Transmit.CommandId = SPI_MLME_RX_ENABLE_REQUEST;
    SPI_Transmit.Length = sizeof( MLME_RX_ENABLE_request_pset_t );;

    SPI_Transmit.PData.RxEnableReq.DeferPermit     = DeferPermit;
    SPI_Transmit.PData.RxEnableReq.RxOnTime[0]     = LS0_BYTE(RxOnTime);
    SPI_Transmit.PData.RxEnableReq.RxOnTime[1]     = LS1_BYTE(RxOnTime);
    SPI_Transmit.PData.RxEnableReq.RxOnTime[2]     = LS2_BYTE(RxOnTime);
    SPI_Transmit.PData.RxEnableReq.RxOnTime[3]     = LS3_BYTE(RxOnTime);
    SPI_Transmit.PData.RxEnableReq.RxOnDuration[0] = LS0_BYTE(RxOnDuration);
    SPI_Transmit.PData.RxEnableReq.RxOnDuration[1] = LS1_BYTE(RxOnDuration);
    SPI_Transmit.PData.RxEnableReq.RxOnDuration[2] = LS2_BYTE(RxOnDuration);
    SPI_Transmit.PData.RxEnableReq.RxOnDuration[3] = LS3_BYTE(RxOnDuration);

    CA8210_IRQN_DISABLE();
    if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
    CA8210_IRQN_ENABLE();

    if ( SPI_Receive.CommandId != SPI_MLME_RX_ENABLE_CONFIRM )
    {
        return MAC_SYSTEM_ERROR;
    }

    SPI_Receive.CommandId = SPI_IDLE;
    return SIMPLECNF.Status;
} // End of MLME_RX_ENABLE_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** MLME_SCAN_request()                                              ******/
/******************************************************************************/
/****** Brief:  MLME_SCAN_request according to API Spec                  ******/
/******************************************************************************/
/****** Param:  ScanType - Scan Type (Energy, Active or Orphan)          ******/
/****** Param:  ScanChannels -  Channel Bit mask (32 Bit)                ******/
/****** Param:  ScanDuration - Time to scan for                          ******/
/****** Param:  pSecurity - Pointer to Security Structure or NULLP       ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_SCAN_request
(
    u8_t         ScanType,
    u32_t        ScanChannels,
    u8_t         ScanDuration,
    SecSpec_t   *pSecurity
)
{
    SPI_Transmit.CommandId = SPI_MLME_SCAN_REQUEST;
    SPI_Transmit.Length = sizeof( MLME_SCAN_request_pset_t );
    SCANREQ.ScanType = ScanType;
    SCANREQ.ScanChannels[0] = LS0_BYTE(ScanChannels);
    SCANREQ.ScanChannels[1] = LS1_BYTE(ScanChannels);
    SCANREQ.ScanChannels[2] = LS2_BYTE(ScanChannels);
    SCANREQ.ScanChannels[3] = LS3_BYTE(ScanChannels);
    SCANREQ.ScanDuration = ScanDuration;
    if ( ( pSecurity == NULLP ) || ( pSecurity->SecurityLevel == 0 ) )
    {
        ASSOCREQ.Security.SecurityLevel = 0;
        SPI_Transmit.Length -= sizeof(SecSpec_t) - 1;
    }
    else
    {
        ASSOCREQ.Security = *pSecurity;
    }

    if(SPI_Send())
        return MAC_SYSTEM_ERROR;

    return(MAC_SUCCESS);
} // End of MLME_SCAN_request()


/******************************************************************************/
/******************************************************************************/
/****** MLME_SET_request_sync()                                          ******/
/******************************************************************************/
/****** Brief:  MLME_SET_request/confirm according to API Spec           ******/
/******************************************************************************/
/****** Param:  PIBAttribute - Attribute Number                          ******/
/****** Param:  PIBAttributeIndex - Index within Attribute if an Array   ******/
/****** Param:  PIBAttributeLength - Attribute Length                    ******/
/****** Param:  pPIBAttributeValue - Pointer to Attribute Value          ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_SET_request_sync
(
    u8_t         PIBAttribute,
    u8_t         PIBAttributeIndex,
    u8_t         PIBAttributeLength,
    const void   *pPIBAttributeValue
)
{

    // pre-check the validity of PIBAttribute values that are not checked in MAC
    if(TDME_CheckPIBAttribute(PIBAttribute, PIBAttributeLength, pPIBAttributeValue))
     return(MAC_INVALID_PARAMETER);

    if(PIBAttribute == phyCurrentChannel)
	 TDME_ChannelInit(*((u8_t*)pPIBAttributeValue));

    if(PIBAttribute == phyTransmitPower)
     return(TDME_SetTxPower(*((u8_t*)pPIBAttributeValue)));

    SPI_Transmit.CommandId = SPI_MLME_SET_REQUEST;
    SPI_Transmit.Length = sizeof(MLME_SET_request_pset_t) - MAX_ATTRIBUTE_SIZE + PIBAttributeLength;
    SETREQ.PIBAttribute = PIBAttribute;
    SETREQ.PIBAttributeIndex = PIBAttributeIndex;
    SETREQ.PIBAttributeLength = PIBAttributeLength;
    memcpy( SETREQ.PIBAttributeValue, pPIBAttributeValue, PIBAttributeLength );

    CA8210_IRQN_DISABLE();
    if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
    CA8210_IRQN_ENABLE();

    if ( SPI_Receive.CommandId != SPI_MLME_SET_CONFIRM )
        return MAC_SYSTEM_ERROR;

     SPI_Receive.CommandId = SPI_IDLE;
     return SIMPLECNF.Status;
} // End of MLME_SET_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** MLME_START_request_sync()                                        ******/
/******************************************************************************/
/****** Brief:  MLME_START_request/confirm according to API Spec         ******/
/******************************************************************************/
/****** Param:  PANId - PAN Identifier                                   ******/
/****** Param:  LogicalChannel - Channel Number                          ******/
/****** Param:  BeaconOrder - Beacon Order                               ******/
/****** Param:  SuperframeOrder - Superframe Order                       ******/
/****** Param:  PANCoordinator - 1 if Coordinator                        ******/
/****** Param:  BatteryLifeExtension - 1 if battery save mode            ******/
/****** Param:  CoordRealignment - 1 if a Coordinator Realignment        ******/
/****** Param:  pSecurity - Pointer to Security Structure or NULLP       ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
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
    SecSpec_t   *pBeaconSecurity
)
{
    SecSpec_t   *pBS;

    TDME_ChannelInit(LogicalChannel);
    SPI_Transmit.CommandId = SPI_MLME_START_REQUEST;
    SPI_Transmit.Length = sizeof(MLME_START_request_pset_t);
    STARTREQ.PANId[0] = LS_BYTE(PANId);
    STARTREQ.PANId[1] = MS_BYTE(PANId);
    STARTREQ.LogicalChannel = LogicalChannel;
    STARTREQ.BeaconOrder = BeaconOrder;
    STARTREQ.SuperframeOrder = SuperframeOrder;
    STARTREQ.PANCoordinator = PANCoordinator;
    STARTREQ.BatteryLifeExtension = BatteryLifeExtension;
    STARTREQ.CoordRealignment = CoordRealignment;
    if ( ( pCoordRealignSecurity == NULLP ) || ( pCoordRealignSecurity->SecurityLevel == 0 ) )
    {
        STARTREQ.CoordRealignSecurity.SecurityLevel = 0;
        SPI_Transmit.Length -= sizeof(SecSpec_t) - 1;
        pBS = (SecSpec_t *)(&STARTREQ.CoordRealignSecurity.SecurityLevel + 1);
    }
    else
    {
        STARTREQ.CoordRealignSecurity = *pCoordRealignSecurity;
        pBS = &STARTREQ.BeaconSecurity;
    }
    if ( ( pBeaconSecurity == NULLP ) || ( pBeaconSecurity->SecurityLevel == 0 ) )
    {
        pBS->SecurityLevel = 0;
        SPI_Transmit.Length -= sizeof(SecSpec_t) - 1;
    }
    else
    {
        *pBS = *pBeaconSecurity;
    }

    CA8210_IRQN_DISABLE();
    if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
    CA8210_IRQN_ENABLE();

    if ( SPI_Receive.CommandId != SPI_MLME_START_CONFIRM )
        return MAC_SYSTEM_ERROR;

    SPI_Receive.CommandId = SPI_IDLE;
    return SIMPLECNF.Status;
} // End of MLME_START_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** MLME_POLL_request_sync()                                         ******/
/******************************************************************************/
/****** Brief:  MLME_POLL_request/confirm according to API Spec          ******/
/******************************************************************************/
/****** Param:  CoordAddress - Coordinator Address                       ******/
/****** Param:  Interval - Polling Interval in 0.1 Seconds Resolution    ******/
/****** Param:  pSecurity - Pointer to Security Structure or NULLP       ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_POLL_request_sync
(
    FullAddr_t   CoordAddress,
    u8_t         Interval[2],    /* polling interval in 0.1 seconds res */
                                 /* 0 means poll once */
                                 /* 0xFFFF means stop polling */
    SecSpec_t   *pSecurity
)
{
    SPI_Transmit.CommandId = SPI_MLME_POLL_REQUEST;
    SPI_Transmit.Length = sizeof( MLME_POLL_request_pset_t );
    POLLREQ.CoordAddress = CoordAddress;
    POLLREQ.Interval[0] = Interval[0];
    POLLREQ.Interval[1] = Interval[1];
    if ( ( pSecurity == NULLP ) || ( pSecurity->SecurityLevel == 0 ) )
    {
        POLLREQ.Security.SecurityLevel = 0;
        SPI_Transmit.Length -= sizeof(SecSpec_t) - 1;
    }
    else
    {
        POLLREQ.Security = *pSecurity;
    }

    CA8210_IRQN_DISABLE();
    if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
    CA8210_IRQN_ENABLE();

    if ( SPI_Receive.CommandId != SPI_MLME_POLL_CONFIRM )
        return MAC_SYSTEM_ERROR;

    SPI_Receive.CommandId = SPI_IDLE;
    return SIMPLECNF.Status;
} // End of MLME_POLL_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** MCPS_DATA_indication()                                           ******/
/******************************************************************************/
/****** Brief:  MCPS_DATA_indication according to API Spec               ******/
/******************************************************************************/
/****** Param:  Src - Source Address                                     ******/
/****** Param:  Dst - Destination Address                                ******/
/****** Param:  MsduLength - MSDU Data Length                            ******/
/****** Param:  MpduLinkQuality - Link Quality Indication                ******/
/****** Param:  DSN - Data Sequence Number                               ******/
/****** Param:  pTimeStamp - Pointer to Timestamp                        ******/
/****** Param:  pMsdu - Pointer to MSDU Data                             ******/
/****** Param:  pSecurity - Pointer to Security Structure                ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
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
    SecSpec_t   *pSecurity
)
{
    SecSpec_t     *psec;

    if(SPI_Receive.CommandId != SPI_MCPS_DATA_INDICATION)
        return(MAC_SYSTEM_ERROR);

    *Src             = SPI_Receive.PData.DataInd.Src;
    *Dst             = SPI_Receive.PData.DataInd.Dst;
    *MsduLength      = SPI_Receive.PData.DataInd.MsduLength;
    *MpduLinkQuality = SPI_Receive.PData.DataInd.MpduLinkQuality;
    *DSN             = SPI_Receive.PData.DataInd.DSN;
    *pTimeStamp      = GETLE32( SPI_Receive.PData.DataInd.TimeStamp );
    memcpy(pMsdu, SPI_Receive.PData.DataInd.Msdu, *MsduLength);

    psec = (SecSpec_t *)( SPI_Receive.PData.DataInd.Msdu + *MsduLength );

    if(psec->SecurityLevel == 0)
     memcpy(pSecurity, psec,  1);
    else
     memcpy(pSecurity, psec, 11);

    SPI_Receive.CommandId = SPI_IDLE;
    return(MAC_SUCCESS);
} // End of MCPS_DATA_indication()


/******************************************************************************/
/******************************************************************************/
/****** MCPS_DATA_confirm()                                              ******/
/******************************************************************************/
/****** Brief:  MCPS_DATA_confirm according to API Spec                  ******/
/******************************************************************************/
/****** Param:  MsduHandle - Handle of Data                              ******/
/****** Param:  pTimeStamp - Pointer to Timestamp                        ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t MCPS_DATA_confirm
(
    u8_t        *MsduHandle,
    u32_t       *pTimeStamp
)
 {

    if(SPI_Receive.CommandId != SPI_MCPS_DATA_CONFIRM)
     return(MAC_SYSTEM_ERROR);

    *MsduHandle = SPI_Receive.PData.DataCnf.MsduHandle;
    *pTimeStamp = GETLE32( SPI_Receive.PData.DataCnf.TimeStamp );

    if((MAC_Workarounds) && (SPI_Receive.PData.DataCnf.Status == MAC_SYSTEM_ERROR))  // direct transmission - change malloc fail message
     SPI_Receive.PData.DataCnf.Status = MAC_TRANSACTION_OVERFLOW;

    SPI_Receive.CommandId = SPI_IDLE;
    return(SPI_Receive.PData.DataCnf.Status);
 } // End of MCPS_DATA_confirm()


/******************************************************************************/
/******************************************************************************/
/****** MLME_ASSOCIATE_indication()                                      ******/
/******************************************************************************/
/****** Brief:  MLME_ASSOCIATE_indication according to API Spec          ******/
/******************************************************************************/
/****** Param:  pDeviceAddress - Pointer to Device Address               ******/
/****** Param:  CapabilityInfo - Bitmap of operational Capabilities      ******/
/****** Param:  pSecurity - Pointer to Security Structure                ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_ASSOCIATE_indication
(
    u8_t        *pDeviceAddress,
    u8_t        *CapabilityInfo,
    SecSpec_t   *pSecurity
)
 {

    if(SPI_Receive.CommandId != SPI_MLME_ASSOCIATE_INDICATION)
     return(MAC_SYSTEM_ERROR);

    memcpy(pDeviceAddress, SPI_Receive.PData.AssocInd.DeviceAddress, 8);
    *CapabilityInfo = SPI_Receive.PData.AssocInd.CapabilityInformation;

    if(SPI_Receive.PData.AssocInd.Security.SecurityLevel == 0)
     memcpy(pSecurity, SPI_Receive.PData.Payload+9,  1);
    else
     memcpy(pSecurity, SPI_Receive.PData.Payload+9, 11);

    SPI_Receive.CommandId = SPI_IDLE;
    return(MAC_SUCCESS);
 } // End of MLME_ASSOCIATE_indication()


/******************************************************************************/
/******************************************************************************/
/****** MLME_ASSOCIATE_confirm()                                         ******/
/******************************************************************************/
/****** Brief:  MLME_ASSOCIATE_confirm according to API Spec             ******/
/******************************************************************************/
/****** Param:  pAssocShortAddress - Pointer to associated Short Address ******/
/****** Param:  pSecurity - Pointer to Security Structure                ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_ASSOCIATE_confirm
(
    u8_t        *pAssocShortAddress,
    SecSpec_t   *pSecurity
)
 {

    if(SPI_Receive.CommandId != SPI_MLME_ASSOCIATE_CONFIRM)
     return(MAC_SYSTEM_ERROR);

    memcpy(pAssocShortAddress, SPI_Receive.PData.AssocCnf.AssocShortAddress, 2);

    if(SPI_Receive.PData.AssocCnf.Security.SecurityLevel == 0)
     memcpy(pSecurity, SPI_Receive.PData.Payload+3,  1);
    else
     memcpy(pSecurity, SPI_Receive.PData.Payload+3, 11);

    SPI_Receive.CommandId = SPI_IDLE;
    return(SPI_Receive.PData.AssocCnf.Status);
 } // End of MLME_ASSOCIATE_confirm()


/******************************************************************************/
/******************************************************************************/
/****** MLME_DISASSOCIATE_indication()                                   ******/
/******************************************************************************/
/****** Brief:  MLME_DISASSOCIATE_indication according to API Spec       ******/
/******************************************************************************/
/****** Param:  pDevAddr - Pointer to Device Address                     ******/
/****** Param:  Reason - Reason for Disassociation                       ******/
/****** Param:  pSecurity - Pointer to Security Structure                ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_DISASSOCIATE_indication
(
    u8_t        *pDevAddr,
    u8_t        *Reason,
    SecSpec_t   *pSecurity
)
 {

    if(SPI_Receive.CommandId != SPI_MLME_DISASSOCIATE_INDICATION)
     return(MAC_SYSTEM_ERROR);

    memcpy(pDevAddr, SPI_Receive.PData.DisassocInd.DevAddr, 8);

    *Reason = SPI_Receive.PData.DisassocInd.Reason;

    if(SPI_Receive.PData.DisassocInd.Security.SecurityLevel == 0)
     memcpy(pSecurity, SPI_Receive.PData.Payload+9,  1);
    else
     memcpy(pSecurity, SPI_Receive.PData.Payload+9, 11);

    SPI_Receive.CommandId = SPI_IDLE;
    return(MAC_SUCCESS);
 } // End of MLME_DISASSOCIATE_indication()


/******************************************************************************/
/******************************************************************************/
/****** MLME_DISASSOCIATE_confirm()                                      ******/
/******************************************************************************/
/****** Brief:  MLME_DISASSOCIATE_confirm according to API Spec          ******/
/******************************************************************************/
/****** Param:  Address - Address                                        ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_DISASSOCIATE_confirm
(
    FullAddr_t  *Address
)
 {

    if(SPI_Receive.CommandId != SPI_MLME_DISASSOCIATE_CONFIRM)
     return(MAC_SYSTEM_ERROR);

    *Address = SPI_Receive.PData.DisassocCnf.Address;

    SPI_Receive.CommandId = SPI_IDLE;
    return(SPI_Receive.PData.DisassocCnf.Status);
 } // End of MLME_DISASSOCIATE_confirm()


/******************************************************************************/
/******************************************************************************/
/****** MLME_BEACON_NOTIFY_indication()                                  ******/
/******************************************************************************/
/****** Brief:  MLME_BEACON_NOTIFY_indication according to API Spec      ******/
/******************************************************************************/
/****** Param:  BSN - Beacon Sequence Number                             ******/
/****** Param:  pPanDescriptor - Pointer to Pan Descriptor               ******/
/****** Param:  PendAddrSpec - Pending Address Specification             ******/
/****** Param:  pAddressListShort - Pointer to List of Short Addresses   ******/
/****** Param:  pAddressListLong - Pointer to List of Long Addresses     ******/
/****** Param:  SduLength - Beacon Payload Length                        ******/
/****** Param:  Sdu - Pointer to Beacon Payload                          ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_BEACON_NOTIFY_indication
(
    u8_t            *BSN,
    PanDescriptor_t *pPanDescriptor,
    u8_t            *PendAddrSpec,
    u8_t            *pAddressListShort,
    u8_t            *pAddressListLong,
    u8_t            *SduLength,
    u8_t            *pSdu
)
 {
    u8_t ptr;
    u8_t nladd, nsadd;

    if(SPI_Receive.CommandId != SPI_MLME_BEACON_NOTIFY_INDICATION)
     return(MAC_SYSTEM_ERROR);

    *BSN = SPI_Receive.PData.BeaconInd.BSN;

    if(SPI_Receive.PData.BeaconInd.PanDescriptor.Security.SecurityLevel == 0)
     {
     memcpy(pPanDescriptor, SPI_Receive.PData.Payload+1, 22);
     ptr = 23;
     }
    else
     {
     memcpy(pPanDescriptor, SPI_Receive.PData.Payload+1, 32);
     ptr = 33;
     }

    *PendAddrSpec = SPI_Receive.PData.Payload[ptr];

    nsadd =  SPI_Receive.PData.Payload[ptr]       & 0x7; // nr of short add.
    nladd = (SPI_Receive.PData.Payload[ptr] >> 4) & 0x7; // nr of long  add.
    ++ ptr;

    if(nsadd > 0)
     {
     memcpy(pAddressListShort, SPI_Receive.PData.Payload+ptr, 2*nsadd);
     ptr += 2*nsadd;
     }

    if(nladd > 0)
     {
     memcpy(pAddressListLong,  SPI_Receive.PData.Payload+ptr, 8*nladd);
     ptr += 8*nladd;
     }

    *SduLength = SPI_Receive.PData.Payload[ptr++];

    memcpy(pSdu,  SPI_Receive.PData.Payload+ptr, *SduLength);

    SPI_Receive.CommandId = SPI_IDLE;
    return(MAC_SUCCESS);
 } // End of MLME_BEACON_NOTIFY_indication()



/******************************************************************************/
/******************************************************************************/
/****** MLME_ORPHAN_indication()                                         ******/
/******************************************************************************/
/****** Brief:  MLME_ORPHAN_indication according to API Spec             ******/
/******************************************************************************/
/****** Param:  pOrphanAddr - Pointer to Orphan Address                  ******/
/****** Param:  pSecurity - Pointer to Security Structure                ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_ORPHAN_indication
(
    u8_t        *pOrphanAddr,
    SecSpec_t   *pSecurity
)
 {

    if(SPI_Receive.CommandId != SPI_MLME_ORPHAN_INDICATION)
     return(MAC_SYSTEM_ERROR);

    memcpy(pOrphanAddr, SPI_Receive.PData.OrphanInd.OrphanAddr, 8);

    if(SPI_Receive.PData.OrphanInd.Security.SecurityLevel == 0)
     memcpy(pSecurity, SPI_Receive.PData.Payload+8,  1);
    else
     memcpy(pSecurity, SPI_Receive.PData.Payload+8, 11);

    SPI_Receive.CommandId = SPI_IDLE;
    return(MAC_SUCCESS);
 } // End of MLME_ORPHAN_indication()


/******************************************************************************/
/******************************************************************************/
/****** MLME_SCAN_confirm()                                              ******/
/******************************************************************************/
/****** Brief:  MLME_SCAN_confirm according to API Spec                  ******/
/******************************************************************************/
/****** Param:  ScanType - Scan Type (Energy, Active or Orphan)          ******/
/****** Param:  pUnscannedChannels - Pointer to List of unscanned Chan.  ******/
/****** Param:  ResultListSize - Result List Size                        ******/
/****** Param:  pResultList - Pointer to Result List                     ******/
/*****          either EnergyDetectList, PanDescriptorList or NULLP      ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
/****** Note: For ScanType ENERGY_DETECT the ResultList contains the     ******/
/******       EnergyDetectList, and it's length is therefore             ******/
/******       ResultListSize Bytes in Size.                              ******/
/******       For ACTIVE_SCAN and PASSIVE_SCAN the ResultList contains   ******/
/******       the PanDescriptorList, which can be variable in Size, as   ******/
/******       each PanDescriptor can be 32 or 22 bytes due to the        ******/
/******       Security Field being either present or omitted. In this    ******/
/******       case, the Function simply copies DEFAULT_RESULT_LIST_SIZE  ******/
/******       Bytes of Data, and correct Partitioning has to be          ******/
/******       performed further upstream.                                ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_SCAN_confirm
(
    u8_t        *ScanType,
    u8_t        *pUnscannedChannels,
    u8_t        *ResultListSize,
    u8_t        *pResultList
)
 {

    if(SPI_Receive.CommandId != SPI_MLME_SCAN_CONFIRM)
     return(MAC_SYSTEM_ERROR);

    *ScanType = SPI_Receive.PData.ScanCnf.ScanType;
    memcpy(pUnscannedChannels, SPI_Receive.PData.ScanCnf.UnscannedChannels, 4);
    *ResultListSize = SPI_Receive.PData.ScanCnf.ResultListSize;

    if(*ScanType == ENERGY_DETECT) // EnergyDetectList
     {
     memcpy(pResultList, SPI_Receive.PData.ScanCnf.ResultList, *ResultListSize);
     }
    else if((*ScanType == ACTIVE_SCAN) || (*ScanType == PASSIVE_SCAN))
     {
     memcpy(pResultList, SPI_Receive.PData.ScanCnf.ResultList, DEFAULT_RESULT_LIST_SIZE);
     }
    else
     {
     pResultList = NULLP;
     }

    SPI_Receive.CommandId = SPI_IDLE;
    return(SPI_Receive.PData.ScanCnf.Status);
 } // End of MLME_SCAN_confirm()


/******************************************************************************/
/******************************************************************************/
/****** MLME_COMM_STATUS_indication()                                    ******/
/******************************************************************************/
/****** Brief:  MLME_COMM_STATUS_indication according to API Spec        ******/
/******************************************************************************/
/****** Param:  pPANId - Pointer to PANId                                ******/
/****** Param:  SrcAddrMode - Source Addressing Mode                     ******/
/****** Param:  pSrcAddr - Pointer to Source Address                     ******/
/****** Param:  DstAddrMode - Destination Addressing Mode                ******/
/****** Param:  pDstAddr - Pointer to Destination Address                ******/
/******************************************************************************/
/****** Return: Comm Status                                              ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_COMM_STATUS_indication
(
    u8_t           *pPANId,
    u8_t           *SrcAddrMode,
    u8_t           *pSrcAddr,
    u8_t           *DstAddrMode,
    u8_t           *pDstAddr,
    SecSpec_t      *pSecurity
)
 {

    if(SPI_Receive.CommandId != SPI_MLME_COMM_STATUS_INDICATION)
     return(MAC_SYSTEM_ERROR);

    memcpy(pPANId,   SPI_Receive.PData.CommStatusInd.PANId, 2);
    *SrcAddrMode   = SPI_Receive.PData.CommStatusInd.SrcAddrMode;
    memcpy(pSrcAddr, SPI_Receive.PData.CommStatusInd.SrcAddr, 8);
    *DstAddrMode   = SPI_Receive.PData.CommStatusInd.DstAddrMode;
    memcpy(pDstAddr, SPI_Receive.PData.CommStatusInd.DstAddr, 8);

    if(SPI_Receive.PData.CommStatusInd.Security.SecurityLevel == 0)
     memcpy(pSecurity, SPI_Receive.PData.Payload+21,  1);
    else
     memcpy(pSecurity, SPI_Receive.PData.Payload+21, 11);

    SPI_Receive.CommandId = SPI_IDLE;
    return(SPI_Receive.PData.CommStatusInd.Status);
 } // End of MLME_COMM_STATUS_indication()


/******************************************************************************/
/******************************************************************************/
/****** MLME_SYNC_LOSS_indication()                                      ******/
/******************************************************************************/
/****** Brief:  MLME_SYNC_LOSS_indication according to API Spec          ******/
/******************************************************************************/
/****** Param:  LossReason - Reason for Loss                             ******/
/****** Param:  pPANId - Pointer to PANId                                ******/
/****** Param:  LogicalChannel - Channel Number                          ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t MLME_SYNC_LOSS_indication
(
    u8_t        *LossReason,
    u8_t        *pPANId,
    u8_t        *LogicalChannel,
    SecSpec_t   *pSecurity
)
 {

    if(SPI_Receive.CommandId != SPI_MLME_SYNC_LOSS_INDICATION)
     return(MAC_SYSTEM_ERROR);

    *LossReason     = SPI_Receive.PData.SyncLossInd.LossReason;
    memcpy(pPANId,    SPI_Receive.PData.SyncLossInd.PANId, 2);
    *LogicalChannel = SPI_Receive.PData.SyncLossInd.LogicalChannel;

    if(SPI_Receive.PData.SyncLossInd.Security.SecurityLevel == 0)
     memcpy(pSecurity, SPI_Receive.PData.Payload+4,  1);
    else
     memcpy(pSecurity, SPI_Receive.PData.Payload+4, 11);

    SPI_Receive.CommandId = SPI_IDLE;
    return(MAC_SUCCESS);
 } // End of MLME_SYNC_LOSS_indication()


/******************************************************************************/
/******************************************************************************/
/****** HWME_SET_request_sync()                                          ******/
/******************************************************************************/
/****** Brief:  HWME_SET_request/confirm according to API Spec           ******/
/******************************************************************************/
/****** Param:  HWAttribute - Attribute Number                           ******/
/****** Param:  HWAttributeLength - Attribute Length                     ******/
/****** Param:  pHWAttributeValue - Pointer to Attribute Value           ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t HWME_SET_request_sync
(
    u8_t         HWAttribute,
    u8_t         HWAttributeLength,
    u8_t        *pHWAttributeValue
)
 {

 SPI_Transmit.CommandId                          = SPI_HWME_SET_REQUEST;
 SPI_Transmit.Length                             = 2 + HWAttributeLength;

 SPI_Transmit.PData.HWMESetReq.HWAttribute       = HWAttribute;
 SPI_Transmit.PData.HWMESetReq.HWAttributeLength = HWAttributeLength;
 memcpy(SPI_Transmit.PData.HWMESetReq.HWAttributeValue, pHWAttributeValue, HWAttributeLength);

 CA8210_IRQN_DISABLE();
 if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
 CA8210_IRQN_ENABLE();

 if(SPI_Receive.CommandId != SPI_HWME_SET_CONFIRM)
  return(MAC_SYSTEM_ERROR);

 SPI_Receive.CommandId = SPI_IDLE;
 return(SPI_Receive.PData.HWMESetCnf.Status);
 } // End of HWME_SET_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** HWME_GET_request_sync()                                          ******/
/******************************************************************************/
/****** Brief:  HWME_GET_request/confirm according to API Spec           ******/
/******************************************************************************/
/****** Param:  HWAttribute - Attribute Number                           ******/
/****** Param:  HWAttributeLength - Attribute Length                     ******/
/****** Param:  pHWAttributeValue - Pointer to Attribute Value           ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t HWME_GET_request_sync
(
    u8_t         HWAttribute,
    u8_t        *HWAttributeLength,
    u8_t        *pHWAttributeValue
)
 {

 SPI_Transmit.CommandId = SPI_HWME_GET_REQUEST;
 SPI_Transmit.Length = 1;

 SPI_Transmit.PData.HWMEGetReq.HWAttribute = HWAttribute;

 CA8210_IRQN_DISABLE();
 if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
 CA8210_IRQN_ENABLE();

 if(SPI_Receive.CommandId != SPI_HWME_GET_CONFIRM)
  return(MAC_SYSTEM_ERROR);

 if(SPI_Receive.PData.HWMEGetCnf.Status == MAC_SUCCESS)
  {
  *HWAttributeLength = SPI_Receive.PData.HWMEGetCnf.HWAttributeLength;
  memcpy(pHWAttributeValue, SPI_Receive.PData.HWMEGetCnf.HWAttributeValue, *HWAttributeLength);
  }

 SPI_Receive.CommandId = SPI_IDLE;
 return(SPI_Receive.PData.HWMEGetCnf.Status);
 } // End of HWME_GET_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** HWME_HAES_request_sync()                                         ******/
/******************************************************************************/
/****** Brief:  HWME_HAES_request/confirm according to API Spec          ******/
/******************************************************************************/
/****** Param:  HAESMode - AES Mode (Encrypt/Decrypt)                    ******/
/****** Param:  pHAESData - Pointer to AES Input/Output Data             ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t HWME_HAES_request_sync
(
    u8_t         HAESMode,
    u8_t        *pHAESData
)
 {

 SPI_Transmit.CommandId = SPI_HWME_HAES_REQUEST;
 SPI_Transmit.Length = 17;

 SPI_Transmit.PData.HWMEHAESReq.HAESMode = HAESMode;
 memcpy(SPI_Transmit.PData.HWMEHAESReq.HAESData, pHAESData, 16);

 CA8210_IRQN_DISABLE();
 if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
 CA8210_IRQN_ENABLE();

 if(SPI_Receive.CommandId != SPI_HWME_HAES_CONFIRM)
  return(MAC_SYSTEM_ERROR);

 if(SPI_Receive.PData.HWMEHAESCnf.Status == MAC_SUCCESS)
  {
  memcpy(pHAESData, SPI_Receive.PData.HWMEHAESCnf.HAESData, 16);
  }

 SPI_Receive.CommandId = SPI_IDLE;
 return(SPI_Receive.PData.HWMEHAESCnf.Status);
 } // End of HWME_HAES_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** HWME_WAKEUP_indication()                                         ******/
/******************************************************************************/
/****** Brief:  HWME_WAKEUP_indication according to API Spec             ******/
/******************************************************************************/
/****** Param:  WakeUpCondition - Wakeup Condition                       ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t HWME_WAKEUP_indication
(
    u8_t        *WakeUpCondition
)
 {

 if(SPI_Receive.CommandId != SPI_HWME_WAKEUP_INDICATION)
  return(MAC_SYSTEM_ERROR);

 *WakeUpCondition = SPI_Receive.PData.HWMEWakeupInd.WakeUpCondition;

 SPI_Receive.CommandId = SPI_IDLE;
 return(MAC_SUCCESS);
 } // End of HWME_WAKEUP_indication()


/******************************************************************************/
/******************************************************************************/
/****** TDME_SETSFR_request_sync()                                       ******/
/******************************************************************************/
/****** Brief:  TDME_SETSFR_request/confirm according to API Spec        ******/
/******************************************************************************/
/****** Param:  SFRPage - SFR Page                                       ******/
/****** Param:  SFRAddress - SFR Address                                 ******/
/****** Param:  SFRValue - SFR Value                                     ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t TDME_SETSFR_request_sync
(
    u8_t         SFRPage,
    u8_t         SFRAddress,
    u8_t         SFRValue
)
 {

 SPI_Transmit.CommandId                          = SPI_TDME_SETSFR_REQUEST;
 SPI_Transmit.Length                             = 3;

 SPI_Transmit.PData.TDMESetSFRReq.SFRPage    = SFRPage;
 SPI_Transmit.PData.TDMESetSFRReq.SFRAddress = SFRAddress;
 SPI_Transmit.PData.TDMESetSFRReq.SFRValue   = SFRValue;

 CA8210_IRQN_DISABLE();
 if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
 CA8210_IRQN_ENABLE();

 if(SPI_Receive.CommandId != SPI_TDME_SETSFR_CONFIRM)
  return(MAC_SYSTEM_ERROR);

 SPI_Receive.CommandId = SPI_IDLE;
 return(SPI_Receive.PData.TDMESetSFRCnf.Status);
 } // End of TDME_SETSFR_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** TDME_GETSFR_request_sync()                                       ******/
/******************************************************************************/
/****** Brief:  TDME_GETSFR_request/confirm according to API Spec        ******/
/******************************************************************************/
/****** Param:  SFRPage - SFR Page                                       ******/
/****** Param:  SFRAddress - SFR Address                                 ******/
/****** Param:  SFRValue - SFR Value                                     ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t TDME_GETSFR_request_sync
(
    u8_t         SFRPage,
    u8_t         SFRAddress,
    u8_t        *SFRValue
)
 {

 SPI_Transmit.CommandId                          = SPI_TDME_GETSFR_REQUEST;
 SPI_Transmit.Length                             = 2;

 SPI_Transmit.PData.TDMEGetSFRReq.SFRPage    = SFRPage;
 SPI_Transmit.PData.TDMEGetSFRReq.SFRAddress = SFRAddress;

 CA8210_IRQN_DISABLE();
 if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
 CA8210_IRQN_ENABLE();

 if(SPI_Receive.CommandId != SPI_TDME_GETSFR_CONFIRM)
  return(MAC_SYSTEM_ERROR);

 *SFRValue = SPI_Receive.PData.TDMEGetSFRCnf.SFRValue;

 SPI_Receive.CommandId = SPI_IDLE;
 return(SPI_Receive.PData.TDMEGetSFRCnf.Status);
 } // End of TDME_GETSFR_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** TDME_TESTMODE_request_sync()                                     ******/
/******************************************************************************/
/****** Brief:  TDME_TESTMODE_request/confirm according to API Spec      ******/
/******************************************************************************/
/****** Param:  TestMode - Test Mode to be set                           ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t TDME_TESTMODE_request_sync
(
    u8_t         TestMode
)
 {

 SPI_Transmit.CommandId = SPI_TDME_TESTMODE_REQUEST;
 SPI_Transmit.Length = 1;
 SPI_Transmit.PData.TDMETestModeReq.TestMode = TestMode;

 CA8210_IRQN_DISABLE();
 if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
 CA8210_IRQN_ENABLE();

 if(SPI_Receive.CommandId != SPI_TDME_TESTMODE_CONFIRM)
  return(MAC_SYSTEM_ERROR);

 SPI_Receive.CommandId = SPI_IDLE;
 return(SPI_Receive.PData.TDMETestModeCnf.Status);
 } // End of TDME_TESTMODE_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** TDME_SET_request_sync()                                          ******/
/******************************************************************************/
/****** Brief:  TDME_SET_request/confirm according to API Spec           ******/
/******************************************************************************/
/****** Param:  TestAttribute - Test Attribute Number                    ******/
/****** Param:  TestAttributeLength - Test Attribute Length              ******/
/****** Param:  pTestAttributeValue - Pointer to Test Attribute Value    ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t TDME_SET_request_sync
(
    u8_t         TestAttribute,
    u8_t         TestAttributeLength,
    void        *pTestAttributeValue
)
 {

 if(TestAttribute == TDME_CHANNEL)
  TDME_ChannelInit(*((u8_t*)pTestAttributeValue));
 SPI_Transmit.CommandId                          = SPI_TDME_SET_REQUEST;
 SPI_Transmit.Length                             = 2 + TestAttributeLength;
 SPI_Transmit.PData.TDMESetReq.TDAttribute       = TestAttribute;
 SPI_Transmit.PData.TDMESetReq.TDAttributeLength = TestAttributeLength;
 memcpy(SPI_Transmit.PData.TDMESetReq.TDAttributeValue, pTestAttributeValue, TestAttributeLength);

 CA8210_IRQN_DISABLE();
 if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
 CA8210_IRQN_ENABLE();

 if(SPI_Receive.CommandId != SPI_TDME_SET_CONFIRM)
  return(MAC_SYSTEM_ERROR);

 SPI_Receive.CommandId = SPI_IDLE;
 return(SPI_Receive.PData.TDMESetCnf.Status);
 } // End of TDME_SET_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** TDME_TXPKT_request_sync()                                        ******/
/******************************************************************************/
/****** Brief:  TDME_TXPKT_request/confirm according to API Spec         ******/
/******************************************************************************/
/****** Param:  TestPacketDataType - Test Packet Data Type               ******/
/****** Param:  TestPacketSequenceNumber - Pointer to Sequence Number    ******/
/****** Param:  TestPacketLength - Pointer to Test Packet Length         ******/
/****** Param:  pTestPacketData - Pointer to Test Packet Data or NULLP   ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t TDME_TXPKT_request_sync
(
    u8_t         TestPacketDataType,
    u8_t        *TestPacketSequenceNumber,
    u8_t        *TestPacketLength,
    void        *pTestPacketData
)
 {

 SPI_Transmit.CommandId = SPI_TDME_TXPKT_REQUEST;
 if(TestPacketDataType == TDME_TXD_APPENDED)
  SPI_Transmit.Length = 3 + *TestPacketLength;
 else
  SPI_Transmit.Length = 3;

 SPI_Transmit.PData.TDMETxPktReq.TestPacketDataType       = TestPacketDataType;
 SPI_Transmit.PData.TDMETxPktReq.TestPacketSequenceNumber = *TestPacketSequenceNumber;
 SPI_Transmit.PData.TDMETxPktReq.TestPacketLength         = *TestPacketLength;

 if(TestPacketDataType == TDME_TXD_APPENDED)
  memcpy(SPI_Transmit.PData.TDMETxPktReq.TestPacketData, pTestPacketData, *TestPacketLength);

 CA8210_IRQN_DISABLE();
 if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
 CA8210_IRQN_ENABLE();

 if(SPI_Receive.CommandId != SPI_TDME_TXPKT_CONFIRM)
  return(MAC_SYSTEM_ERROR);

 if(SPI_Receive.PData.TDMETxPktCnf.Status == TDME_SUCCESS)
  {
  *TestPacketLength         = SPI_Receive.PData.TDMETxPktCnf.TestPacketLength;
  *TestPacketSequenceNumber = SPI_Receive.PData.TDMETxPktCnf.TestPacketSequenceNumber;
  memcpy(pTestPacketData, SPI_Receive.PData.TDMETxPktCnf.TestPacketData, *TestPacketLength);
  }

 SPI_Receive.CommandId = SPI_IDLE;
 return(SPI_Receive.PData.TDMETxPktCnf.Status);
 } // End of TDME_TXPKT_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** TDME_LOTLK_request_sync()                                        ******/
/******************************************************************************/
/****** Brief:  TDME_LOTLK_request/confirm according to API Spec         ******/
/******************************************************************************/
/****** Param:  TestChannel - Pointer to Channel                         ******/
/****** Param:  TestRxTxb - Pointer to LO Mode (Rx when 1, Tx when 0)    ******/
/****** Param:  TestLOFDACValue - Pointer LOFDAC Value                   ******/
/****** Param:  TestLOAMPValue - Pointer LOAMP Value                     ******/
/****** Param:  TestLOTXCALValue - Pointer LOTXCAL Value                 ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t TDME_LOTLK_request_sync
(
    u8_t        *TestChannel,
    u8_t        *TestRxTxb,
    u8_t        *TestLOFDACValue,
    u8_t        *TestLOAMPValue,
    u8_t        *TestLOTXCALValue
)
 {

 SPI_Transmit.CommandId = SPI_TDME_LOTLK_REQUEST;
 SPI_Transmit.Length    = 2;

 SPI_Transmit.PData.TDMELOTlkReq.TestChannel = *TestChannel;
 SPI_Transmit.PData.TDMELOTlkReq.TestRxTxb   = *TestRxTxb;

 CA8210_IRQN_DISABLE();
 if(SPI_Send())
        {
        CA8210_IRQN_ENABLE();
        return MAC_SYSTEM_ERROR;
	    }
 CA8210_IRQN_ENABLE();

 if(SPI_Receive.CommandId != SPI_TDME_LOTLK_CONFIRM)
  return(MAC_SYSTEM_ERROR);

 if(SPI_Receive.PData.TDMELOTlkCnf.Status == TDME_SUCCESS)
  {
  *TestChannel         = SPI_Receive.PData.TDMELOTlkCnf.TestChannel;
  *TestRxTxb           = SPI_Receive.PData.TDMELOTlkCnf.TestRxTxb;
  *TestLOFDACValue     = SPI_Receive.PData.TDMELOTlkCnf.TestLOFDACValue;
  *TestLOAMPValue      = SPI_Receive.PData.TDMELOTlkCnf.TestLOAMPValue;
  *TestLOTXCALValue    = SPI_Receive.PData.TDMELOTlkCnf.TestLOTXCALValue;
  }

 SPI_Receive.CommandId = SPI_IDLE;
 return(SPI_Receive.PData.TDMELOTlkCnf.Status);
 } // End of TDME_LOTLK_request_sync()


/******************************************************************************/
/******************************************************************************/
/****** TDME_RXPKT_indication()                                          ******/
/******************************************************************************/
/****** Brief:  TDME_RXPKT_indication according to API Spec              ******/
/******************************************************************************/
/****** Param:  TestPacketEDValue - Pointer to received Packet ED Value  ******/
/****** Param:  TestPacketCSValue - Pointer to received Packet CS Value  ******/
/****** Param:  TestPacketFoffsValue - Pointer to Frequency Offset Value ******/
/****** Param:  TestPacketLength - Pointer to Packet Payload Length      ******/
/****** Param:  TestPacketData - Pointer to received Packet Payload Data ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t TDME_RXPKT_indication
(
    u8_t        *TestPacketEDValue,
    u8_t        *TestPacketCSValue,
    u8_t        *TestPacketFoffsValue,
    u8_t        *TestPacketLength,
    void        *TestPacketData
)
 {

 if(SPI_Receive.CommandId != SPI_TDME_RXPKT_INDICATION)
  return(MAC_SYSTEM_ERROR);

 *TestPacketEDValue         = SPI_Receive.PData.TDMERxPktInd.TestPacketEDValue;
 *TestPacketCSValue         = SPI_Receive.PData.TDMERxPktInd.TestPacketCSValue;
 *TestPacketFoffsValue      = SPI_Receive.PData.TDMERxPktInd.TestPacketFoffsValue;
 *TestPacketLength          = SPI_Receive.PData.TDMERxPktInd.TestPacketLength;

 memcpy(TestPacketData, SPI_Receive.PData.TDMERxPktInd.TestPacketData, *TestPacketLength);

 SPI_Receive.CommandId = SPI_IDLE;
 return(SPI_Receive.PData.TDMERxPktInd.Status);
 } // End of TDME_RXPKT_indication()


/******************************************************************************/
/******************************************************************************/
/****** TDME_EDDET_indication()                                          ******/
/******************************************************************************/
/****** Brief:  TDME_EDDET_indication according to API Spec              ******/
/******************************************************************************/
/****** Param:  TestEDThreshold - Pointer to ED Threshold Setting        ******/
/****** Param:  TestEDValue - Pointer to received ED Value               ******/
/****** Param:  TestCSValue - Pointer to received CS Value               ******/
/****** Param:  TestTimeAboveThreshold_us - Time above Threshold [us]    ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t TDME_EDDET_indication
(
    u8_t        *TestEDThreshold,
    u8_t        *TestEDValue,
    u8_t        *TestCSValue,
    void        *TestTimeAboveThreshold_us
)
 {

 if(SPI_Receive.CommandId != SPI_TDME_EDDET_INDICATION)
  return(MAC_SYSTEM_ERROR);

 *TestEDThreshold         = SPI_Receive.PData.TDMEEDDetInd.TestEDThreshold;
 *TestEDValue             = SPI_Receive.PData.TDMEEDDetInd.TestEDValue;
 *TestCSValue             = SPI_Receive.PData.TDMEEDDetInd.TestCSValue;

 memcpy(TestTimeAboveThreshold_us, SPI_Receive.PData.TDMEEDDetInd.TestTimeAboveThreshold_us, 2);

 SPI_Receive.CommandId = SPI_IDLE;
 return(TDME_SUCCESS);
 } // End of TDME_EDDET_indication()


/******************************************************************************/
/******************************************************************************/
/****** TDME_ERROR_indication()                                          ******/
/******************************************************************************/
/****** Brief:  TDME_ERROR_indication according to API Spec              ******/
/******************************************************************************/
/****** Param:  ErrorCode - Error Code                                   ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t TDME_ERROR_indication
(
    u8_t        *ErrorCode
)
 {

 if(SPI_Receive.CommandId != SPI_TDME_ERROR_INDICATION)
  return(MAC_SYSTEM_ERROR);

 *ErrorCode = SPI_Receive.PData.TDMEErrorInd.ErrorCode;

 SPI_Receive.CommandId = SPI_IDLE;
 return(TDME_SUCCESS);
 } // End of TDME_ERROR_indication()


/******************************************************************************/
/******************************************************************************/
/****** TDME_ChipInit()                                                  ******/
/******************************************************************************/
/****** Brief:  TDME Chip Register Default Initialisation Macro          ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t TDME_ChipInit(void)
 {
 u8_t status;

 if((status = TDME_SETSFR_request_sync(1, 0xE1, 0x29)))  // LNA Gain Settings
  return(status);
 if((status = TDME_SETSFR_request_sync(1, 0xE2, 0x54)))
  return(status);
 if((status = TDME_SETSFR_request_sync(1, 0xE3, 0x6C)))
  return(status);
 if((status = TDME_SETSFR_request_sync(1, 0xE4, 0x7A)))
  return(status);
 if((status = TDME_SETSFR_request_sync(1, 0xE5, 0x84)))
  return(status);
 if((status = TDME_SETSFR_request_sync(1, 0xE6, 0x8B)))
  return(status);
 if((status = TDME_SETSFR_request_sync(1, 0xE7, 0x92)))
  return(status);
 if((status = TDME_SETSFR_request_sync(1, 0xE9, 0x96)))
  return(status);
 if((status = TDME_SETSFR_request_sync(1, 0xD3, 0x5B))) // Preamble Timing Config
  return(status);
 if((status = TDME_SETSFR_request_sync(1, 0xD1, 0x5A))) // Preamble Threshold High
  return(status);
 if((status = TDME_SETSFR_request_sync(0, 0xFE, 0x3F))) // Tx Output Power 8 dBm
  return(status);

 return(status);
 } // End of TDME_ChipInit()


/******************************************************************************/
/******************************************************************************/
/****** TDME_ChannelInit()                                               ******/
/******************************************************************************/
/****** Brief:  TDME Channel Register Default Initialisation Macro (Tx)  ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: Status                                                   ******/
/******************************************************************************/
/******************************************************************************/
u8_t TDME_ChannelInit(u8_t channel)
 {
 u8_t status;
 u8_t txcalval;

 if(     channel >= 25)
  txcalval = 0xA7;
 else if(channel >= 23)
  txcalval = 0xA8;
 else if(channel >= 22)
  txcalval = 0xA9;
 else if(channel >= 20)
  txcalval = 0xAA;
 else if(channel >= 17)
  txcalval = 0xAB;
 else if(channel >= 16)
  txcalval = 0xAC;
 else if(channel >= 14)
  txcalval = 0xAD;
 else if(channel >= 12)
  txcalval = 0xAE;
 else
  txcalval = 0xAF;

 status = TDME_SETSFR_request_sync(1, 0xBF, txcalval);  // LO Tx Cal

 return(status);
 } // End of TDME_ChannelInit()


/******************************************************************************/
/******************************************************************************/
/****** TDME_CheckPIBAttribute()                                         ******/
/******************************************************************************/
/****** Brief:  Checks Attribute Values that are not checked in MAC      ******/
/******************************************************************************/
/****** Param:  PIBAttribute - Attribute Number                          ******/
/****** Param:  PIBAttributeLength - Attribute Length                    ******/
/****** Param:  pPIBAttributeValue - Pointer to Attribute Value          ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t TDME_CheckPIBAttribute
(
    u8_t         PIBAttribute,
    u8_t         PIBAttributeLength,
    const void   *pPIBAttributeValue
)
 {
 u8_t status;
 u8_t value;

 status = MAC_SUCCESS;
 value  = *((u8_t*)pPIBAttributeValue);

 switch(PIBAttribute)
  {

  // PHY

  case phyTransmitPower:
   if(value > 0x3F)
    status = MAC_INVALID_PARAMETER;
   break;
  case phyCCAMode:
   if(value > 0x03)
    status = MAC_INVALID_PARAMETER;
   break;

  // MAC

  case macBattLifeExtPeriods:
   if((value < 6) || (value > 41))
    status = MAC_INVALID_PARAMETER;
   break;
  case macBeaconPayload:
   if(PIBAttributeLength > aMaxBeaconPayloadLength)
    status = MAC_INVALID_PARAMETER;
   break;
  case macBeaconPayloadLength:
   if(value > aMaxBeaconPayloadLength)
    status = MAC_INVALID_PARAMETER;
   break;
  case macBeaconOrder:
   if(value > 15)
    status = MAC_INVALID_PARAMETER;
   break;
  case macMaxBE:
   if((value < 3) || (value > 8))
    status = MAC_INVALID_PARAMETER;
   break;
  case macMaxCSMABackoffs:
   if(value > 5)
    status = MAC_INVALID_PARAMETER;
   break;
  case macMaxFrameRetries:
   if(value > 7)
    status = MAC_INVALID_PARAMETER;
   break;
  case macMinBE:
   if(value > 8)
    status = MAC_INVALID_PARAMETER;
   break;
  case macResponseWaitTime:
   if((value < 2) || (value > 64))
    status = MAC_INVALID_PARAMETER;
   break;
  case macSuperframeOrder:
   if(value > 15)
    status = MAC_INVALID_PARAMETER;
   break;

  // boolean
  case macAssociatedPANCoord:
  case macAssociationPermit:
  case macAutoRequest:
  case macBattLifeExt:
  case macGTSPermit:
  case macPromiscuousMode:
  case macRxOnWhenIdle:
  case macSecurityEnabled:
   if(value > 1)
    status = MAC_INVALID_PARAMETER;
   break;

  // MAC SEC
  case macAutoRequestSecurityLevel:
   if(value > 7)
    status = MAC_INVALID_PARAMETER;
   break;
  case macAutoRequestKeyIdMode:
   if(value > 3)
    status = MAC_INVALID_PARAMETER;
   break;

  default:
   break;
  }

return(status);
}

/******************************************************************************/
/******************************************************************************/
/****** TDME_SetTxPower()                                                ******/
/******************************************************************************/
/****** Brief:  Sets the tx power for MLME_SET phyTransmitPower          ******/
/******************************************************************************/
/****** Param:  txp - Transmit Power                                     ******/
/******************************************************************************/
/****** Normalised to 802.15.4 Definition (6-bit, signed):               ******/
/****** Bit 7-6: not used                                                ******/
/****** Bit 5-0: tx power (-32 - +31 dB)                                 ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t TDME_SetTxPower(u8_t txp)
 {
 u8_t status;
 i8_t txp_val;
 u8_t txp_ext;
 u8_t paib;

 /* extend from 6 to 8 bit */
 txp_ext = 0x3F & txp;
 if(txp_ext & 0x20)
  txp_ext += 0xC0;
 txp_val = (i8_t)txp_ext;

 if(MAC_MPW)
  {

  if(txp_val > 0)
   paib = 0xD3; /* 8 dBm: ptrim = 5, itrim = +3 => +4 dBm */
  else
   paib = 0x73; /* 0 dBm: ptrim = 7, itrim = +3 => -6 dBm */

  status = TDME_SETSFR_request_sync(0, 0xB1, paib); // write PACFG

  }
 else
  {

  // Look-Up Table for Setting Current and Frequency Trim values for desired Output Power
  if(     txp_val  >  8)   {  paib = 0x3F; }
  else if(txp_val ==  8)   {  paib = 0x32; }
  else if(txp_val ==  7)   {  paib = 0x22; }
  else if(txp_val ==  6)   {  paib = 0x18; }
  else if(txp_val ==  5)   {  paib = 0x10; }
  else if(txp_val ==  4)   {  paib = 0x0C; }
  else if(txp_val ==  3)   {  paib = 0x08; }
  else if(txp_val ==  2)   {  paib = 0x05; }
  else if(txp_val ==  1)   {  paib = 0x03; }
  else if(txp_val ==  0)   {  paib = 0x01; }
  else         /*  <  0 */ {  paib = 0x00; }

  status = TDME_SETSFR_request_sync(0, 0xFE, paib); // write PACFGIB

  }

 return(status);
 }


/******************************************************************************/
/******************************************************************************/
/****** TDME_GetTxPower()                                                ******/
/******************************************************************************/
/****** Brief:  returns the tx power for MLME_GET phyTransmitPower       ******/
/******************************************************************************/
/****** Param:  *txp - Transmit Power                                    ******/
/******************************************************************************/
/****** Normalised to 802.15.4 Definition (6-bit, signed):               ******/
/****** Bit 7-6: not used                                                ******/
/****** Bit 5-0: tx power (-32 - +31 dB)                                 ******/
/******************************************************************************/
/****** Return: Confirm Status                                           ******/
/******************************************************************************/
/******************************************************************************/
u8_t  TDME_GetTxPower(u8_t *txp)
 {
 u8_t status;
 u8_t paib;
 i8_t txp_val;

 if(MAC_MPW)
  {

  status = TDME_GETSFR_request_sync(0, 0xB1, &paib); // read PACFG

  if(paib & 0x80)  // BOOST?
   txp_val =  4;    /* +4 dBm */
  else
   txp_val = -6;    /* -6 dBm */

  /* limit to 6 bit */
  *txp = (u8_t)(txp_val) & 0x3F;

  *txp += (0x01 << 6);  /* tolerance +-3 dB */

  }
 else
  {

  status = TDME_GETSFR_request_sync(0, 0xFE, &paib); // read PACFGIB

  if(     paib  >= 0x32)  txp_val =  8;
  else if(paib  >= 0x22)  txp_val =  7;
  else if(paib  >= 0x18)  txp_val =  6;
  else if(paib  >= 0x10)  txp_val =  5;
  else if(paib  >= 0x0C)  txp_val =  4;
  else if(paib  >= 0x08)  txp_val =  3;
  else if(paib  >= 0x05)  txp_val =  2;
  else if(paib  >= 0x03)  txp_val =  1;
  else if(paib  >  0x00)  txp_val =  0;
  else                    txp_val = -1;

  /* limit to 6 bit */
  *txp = (u8_t)(txp_val) & 0x3F;

  //                      /* tolerance +-1 dB */
  // txp += (0x01 << 6);  /* tolerance +-3 dB */
  // txp += (0x02 << 6);  /* tolerance +-6 dB */

  }

 return(status);
 }

