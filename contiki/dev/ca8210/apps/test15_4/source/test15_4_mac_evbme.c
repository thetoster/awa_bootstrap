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
#include <string.h>
#include "dev/ca8210/cascoda/include/cascoda_types.h"
#include "dev/ca8210/cascoda/include/mac_messages.h"
#include "dev/ca8210/cascoda/include/cascoda_debug.h"
#include "dev/ca8210/cascoda/include/cascoda_serial.h"
#include "dev/ca8210/cascoda/include/cascoda_spi.h"
#include "dev/ca8210/cascoda/include/cascoda_api.h"
#include "dev/ca8210/cascoda/include/cascoda_evbme.h"
#include "dev/ca8210/apps/test15_4/include/test15_4_mac_evbme.h"
#include "dev/ca8210/apps/test15_4/include/test15_4_mac_tests.h"


/******************************************************************************/
/****** Global Variables                                                 ******/
/******************************************************************************/
u8_t MAC_AWAITING_ASSOC_INDICATION  = 0;
u8_t MAC_AWAITING_ORPHAN_INDICATION = 0;


/******************************************************************************/
/******************************************************************************/
/****** TEST15_4_MAC_UpStreamDispatch()                                  ******/
/******************************************************************************/
/****** Brief:  Dispatch Branch for EVBME Request (UpStream, Serial)     ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void TEST15_4_MAC_UpStreamDispatch(void)
{

if((SERIAL_RX_CMD_ID >= EVBME_PIB_TEST_REQUEST) &&
   (SERIAL_RX_CMD_ID <= EVBME_FFD_AWAIT_ORPHAN_REQUEST))
 {

    EVBMEIsDownStreamCommand = 0;

    switch ( SERIAL_RX_CMD_ID )
    {
    // MAC Tests
    case EVBME_PIB_TEST_REQUEST:
        PIBTest();
        break;
    case EVBME_SCAN_TEST_REQUEST:
        ScanTest();
        break;
    case EVBME_SECPIB_TEST_REQUEST:
        SecPIBTest();
        break;
    case EVBME_DATA_TEST_REQUEST:
        if ( DataTest( SERIAL_RX_DATA[0], (SERIAL_RX_DATA[1])? TXOPT_INDIRECT : 0,
                       SERIAL_RX_DATA[2] + ( SERIAL_RX_DATA[3] << 8 ), SERIAL_RX_DATA[4] ) != 0 )
         dps("DATA TEST FAIL\n");
        break;
    case EVBME_RFD_ASSOCIATE_REQUEST:
        if ( AssocTest( SERIAL_RX_DATA[0], SERIAL_RX_DATA[1] ) != 0 )
         dps("ASSOC TEST FAIL\n");
        break;
    // MAC Functionality
    case EVBME_FFD_INIT_REQUEST:
        if ( InitialiseDevice( SERIAL_RX_DATA[0], SERIAL_RX_DATA[1] + (SERIAL_RX_DATA[2]<<8),
                               SERIAL_RX_DATA[3] + (SERIAL_RX_DATA[4]<<8), SERIAL_RX_DATA + 5 ) != 0 )
         dps("INITIALISE FAIL\n");
        break;
    case EVBME_FFD_START_REQUEST:
        if ( StartDevice( SERIAL_RX_DATA[0], SERIAL_RX_DATA + 1 ) != 0 )
         dps("START DEVICE FAIL\n");
        break;
    case EVBME_FFD_AWAIT_ASSOC_REQUEST:
        SetupAwaitAssoc( SERIAL_RX_DATA, SERIAL_RX_DATA[8] + (SERIAL_RX_DATA[9]<<8), SERIAL_RX_DATA[10] );
        MAC_AWAITING_ASSOC_INDICATION = 1;
        break;
    case EVBME_FFD_AWAIT_ORPHAN_REQUEST:
        SetupAwaitOrphan( SERIAL_RX_DATA, SERIAL_RX_DATA[8] + (SERIAL_RX_DATA[9]<<8) );
        MAC_AWAITING_ORPHAN_INDICATION = 1;
        break;
    }

 }

} // End of TEST15_4_MAC_UpStreamDispatch()


/******************************************************************************/
/******************************************************************************/
/****** TEST15_4_MAC_DownStreamDispatch()                                ******/
/******************************************************************************/
/****** Brief:  Dispatch Branch for MAC Commands (DownStream, SPI)       ******/
/******************************************************************************/
/****** Param:  -                                                        ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void TEST15_4_MAC_DownStreamDispatch(void)
{

    switch( SPI_RX_CMD_ID )
    {
    case SPI_MCPS_DATA_INDICATION:
        break;
    case SPI_MCPS_DATA_CONFIRM:
        break;
    case SPI_MCPS_PURGE_CONFIRM:
        break;
    case SPI_MLME_ASSOCIATE_INDICATION:
     if(MAC_AWAITING_ASSOC_INDICATION)
         {
         if ( memcmp( SPI_Receive.PData.AssocInd.DeviceAddress, AssocDeviceAddress, 8 )
           && memcmp( AssocDeviceAddress, AllZeroes, 8 ) )
          {
            break;
          }
         EVBMEForceUpStream(SPI_MLME_ASSOCIATE_INDICATION);
         Security.SecurityLevel = 0;
         MLME_ASSOCIATE_response( SPI_Receive.PData.AssocInd.DeviceAddress,
                                 StoredAssocShortAddress,
                                 StoredAssocStatus, &Security );
         MAC_AWAITING_ASSOC_INDICATION = 0;
         }
        break;
    case SPI_MLME_ASSOCIATE_CONFIRM:
        break;
    case SPI_MLME_DISASSOCIATE_INDICATION:
        break;
    case SPI_MLME_DISASSOCIATE_CONFIRM:
        break;
    case SPI_MLME_BEACON_NOTIFY_INDICATION:
        break;
    case SPI_MLME_GET_CONFIRM:
        break;
    case SPI_MLME_ORPHAN_INDICATION:
     if(MAC_AWAITING_ORPHAN_INDICATION)
         {
         if ( memcmp( SPI_Receive.PData.OrphanInd.OrphanAddr, OrphanDeviceAddress, 8 )
           && memcmp( OrphanDeviceAddress, AllZeroes, 8 ) )
          {
            break;
          }
         EVBMEForceUpStream(SPI_MLME_ORPHAN_INDICATION);
         MLME_ORPHAN_response(SPI_Receive.PData.OrphanInd.OrphanAddr,
                             StoredOrphanShortAddress, 1,
                             &SPI_Receive.PData.OrphanInd.Security);
         MAC_AWAITING_ORPHAN_INDICATION = 0;
         }
        break;
    case SPI_MLME_RESET_CONFIRM:
        break;
    case SPI_MLME_RX_ENABLE_CONFIRM:
        break;
    case SPI_MLME_SCAN_CONFIRM:
        break;
    case SPI_MLME_COMM_STATUS_INDICATION:
        break;
    case SPI_MLME_SET_CONFIRM:
        break;
    case SPI_MLME_START_CONFIRM:
        break;
    case SPI_MLME_SYNC_LOSS_INDICATION:
        break;
    case SPI_MLME_POLL_CONFIRM:
        break;
    case SPI_HWME_SET_CONFIRM:
        break;
    case SPI_HWME_GET_CONFIRM:
        break;
    case SPI_HWME_HAES_CONFIRM:
        break;
    case SPI_HWME_WAKEUP_INDICATION:
        break;
    case SPI_TDME_MESSAGE_INDICATION:
       break;
    }

} // End of TEST15_4_MAC_DownStreamDispatch()
