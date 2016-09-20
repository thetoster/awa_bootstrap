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

#ifndef CASCODA_EVBME_H
#define CASCODA_EVBME_H


/******************************************************************************/
/****** EVBME Command ID Codes                                           ******/
/******************************************************************************/
#define EVBME_SET_REQUEST               (0x5F)
#define EVBME_GUI_CONNECTED      		(0x81)
#define EVBME_GUI_DISCONNECTED  		(0x82)
#define EVBME_MESSAGE_INDICATION        (0xA0)


/******************************************************************************/
/****** EVBME Enumerations                                               ******/
/******************************************************************************/
/* Status */
#define EVBME_SUCCESS          (0x00)
#define EVBME_FAIL             (0x01)
#define EVBME_UNKNOWN          (0x02)
#define EVBME_INVALID          (0x03)
#define EVBME_NO_ACCESS        (0x04)


/******************************************************************************/
/****** EVBME Attribute IDs                                              ******/
/******************************************************************************/
#define EVBME_RESETRF                      (0x00)
#define EVBME_CFGPINS                      (0x01)


/******************************************************************************/
/****** Message Definitions for UpStream (Serial) and  DownStream (SPI)  ******/
/******************************************************************************/
#define SERIAL_RX_CMD_ID  (SerialRxBuffer.Header[SERIAL_CMD_ID])
#define SERIAL_RX_CMD_LEN (SerialRxBuffer.Header[SERIAL_CMD_LEN])
#define SERIAL_RX_DATA    (SerialRxBuffer.Data)

#define SERIAL_TX_CMD_ID  (SerialTxBuffer.Header[SERIAL_CMD_ID])
#define SERIAL_TX_CMD_LEN (SerialTxBuffer.Header[SERIAL_CMD_LEN])
#define SERIAL_TX_DATA    (SerialTxBuffer.Data)

#define SPI_RX_CMD_ID     (SPI_Receive.CommandId)
#define SPI_RX_CMD_LEN    (SPI_Receive.Length)
#define SPI_RX_DATA       (SPI_Receive.PData.Payload)

#define SPI_TX_CMD_ID     (SPI_Transmit.CommandId)
#define SPI_TX_CMD_LEN    (SPI_Transmit.Length)
#define SPI_TX_DATA       (SPI_Transmit.PData.Payload)


/******************************************************************************/
/****** Global Parameters that can by set via EVBME_SET_request          ******/
/******************************************************************************/
extern u8_t EVBME_SetVal[8];


/******************************************************************************/
/****** Global Parameters for Communications                             ******/
/******************************************************************************/
// Up   Stream Command when set
extern u8_t EVBMEIsUpStreamCommand;    // Pass-Through SPI -> EVBME -> SERIAL
// Down Stream Command when set
extern u8_t EVBMEIsDownStreamCommand;  // Pass Through SERIAL -> EVBME -> SPI


/******************************************************************************/
/****** EVBME API Functions                                              ******/
/******************************************************************************/
u8_t EVBMEInitialise(u8_t *version);
u8_t EVBMEHandler(void);
void EVBMEUpStreamDispatch(u8_t *version);
void EVBMESendDownStream(void);
void EVBMESendUpStream(void);
void EVBMEForceUpStream(u8_t cmd);
void EVBMESetIsUpStreamCommand(u8_t SendUpStream);
void EVBMECheckSerialCommand(void);
void EVBMECheckSPICommand(void);
u8_t EVBME_SET_request(u8_t Attribute, u8_t AttributeLength, u8_t *AttributeValue );
u8_t EVBME_ResetRF(u8_t ms);
u8_t EVBME_Connect(u8_t *version);
void EVBME_Disconnect(void);
u8_t EVBME_WaitforResponse(u8_t command, u32_t timeout_ms);
u8_t EVBME_CompareCommand(u8_t command);
void EVBME_PowerDown(u8_t mode, u32_t sleeptime_ms);
void EVBME_Wakeup(u8_t mode);


#endif // CASCODA_EVBME_H
