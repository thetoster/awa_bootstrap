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

#ifndef CASCODA_SERIAL_H
#define CASCODA_SERIAL_H

#define SERIAL_HDR_LEN          (2)
#define SERIAL_CMD_ID           (0)
#define SERIAL_CMD_LEN          (1)


/******************************************************************************/
/****** Typedef for Serial Buffer                                        ******/
/******************************************************************************/
typedef struct SerialBuffer_s
{
    u8_t    SOM;
    u8_t    Header[SERIAL_HDR_LEN];
    u8_t    Data[200];
} SerialBuffer_t;


/******************************************************************************/
/****** Global Variables for Serial Message Buffers                      ******/
/******************************************************************************/
extern SerialBuffer_t SerialTxBuffer, SerialRxBuffer;


/******************************************************************************/
/****** Functions in cascoda_serial_uart.c / cascoda_serial_usb.c        ******/
/******************************************************************************/
u8_t SerialGetCommand(  u8_t c  );
void EVBME_Message( u8_t Count, u8_t *pBuffer );
void MAC_Message( u8_t CommandId, u8_t Count, u8_t *pBuffer );


#endif // CASCODA_SERIAL_H
