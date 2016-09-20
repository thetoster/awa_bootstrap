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

#ifndef CASCODA_SPI_H
#define CASCODA_SPI_H


/******************************************************************************/
/****** Definitions for SPI_Exchange Status                              ******/
/******************************************************************************/
#define SPIST_TX  1       // packet transmitted
#define SPIST_RX  2       // packet received
#define SPIST_ERR 0xF0    // SPI Handling/System/Timeout Error


/******************************************************************************/
/****** Definitions for SPI Handling                                     ******/
/******************************************************************************/
#define SPI_T_TIMEOUT 500     // Timeout for NACKs [ms]
#define SPI_T_BACKOFF   2     // Backoff Time when receiving SPI_NACK [ms]

#define SPI_RX_FIFO_SIZE  2   // Maximum Size of Rx FIFO


/******************************************************************************/
/****** Global Variables for SPI Message Buffers                         ******/
/******************************************************************************/
extern MAC_Message_t   SPI_Transmit, SPI_Receive;


/******************************************************************************/
/****** Function Declarations for cascoda_spi.c                          ******/
/******************************************************************************/
u8_t SPI_Exchange( MAC_Message_t *pTxBuffer, MAC_Message_t *pRxBuffer );
u8_t SPI_Send( void );
u8_t SPI_Push_Rx_Buffer( void );
void SPI_Pop_Rx_Buffer( void );
void SPI_Wait( void );
void SPI_Error(char *message);


#endif // CASCODA_SPI_H
