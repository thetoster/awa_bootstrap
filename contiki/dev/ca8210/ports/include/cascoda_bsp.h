/*
 * Copyright (c) 2016, Cascoda Limited and/or its
 * affiliated group companies.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/******************************************************************************/
/****** Board Support Package (BSP)                                      ******/
/****** Micro: Microchip        32MX470F512H                             ******/
/****** Board: MikroElektronika IMGTech Klickerboard                     ******/
/******************************************************************************/
/******************************************************************************/
/****** Revision           Notes                                         ******/
/******************************************************************************/
/****** 1.0  19/07/14  WB  Release Baseline                              ******/
/****** 1.0u 25/03/15  HM  Modified for HID connectivity                 ******/
/******************************************************************************/
/******************************************************************************/
// If __USE_BREAKOUT_BOARD__ is defined this indicates the old clicker boards
// with the CA-8210 on  a separate clicker-board connected to the mikrobus.
// Pin Numbers are for the 32MX470F512H 64-Pin QFP Package.
/******************************************************************************/
//  CA-8210                BREAKOUT=0              BREAKOUT=1
/******************************************************************************/
//  MOSI   (23)            RG8 SPI2 MOSI2  (6)     RD4 SPI1 MOSI1 (52)
//  MISO   (24)            RG7 SPI2 MISO2  (5)     RD3 SPI1 MISO1 (51)
//  SCLK   (21)            RG6 SPI2 SCK2   (4)     RD2 SPI1 SCK1  (50)
//  SSB    (22)            RE3            (63)     RE5             (1)
//  NIRQ   (20)            RD1            (49)     RD0            (46)
//  NRESET (27)            RE2            (62)     RD6            (54)

#ifndef CASCODA_BSP_H
#define CASCODA_BSP_H

#include "dev/ca8210/cascoda/include/cascoda_types.h"
#include <stdint.h>

#ifdef __USE_BREAKOUT_BOARD__
#define CA8210_SSB_PORT      E
#define CA8210_SSB_PIN       5
#define CA8210_NIRQ_PORT     D
#define CA8210_NIRQ_PIN      0
#define CA8210_NRESET_PORT   D
#define CA8210_NRESET_PIN    6
#define SPIBUF               SPI1BUF
#define SPISTATbits          SPI1STATbits
#else
#define CA8210_SSB_PORT      E
#define CA8210_SSB_PIN       3
#define CA8210_NIRQ_PORT     D
#define CA8210_NIRQ_PIN      1
#define CA8210_NRESET_PORT   E
#define CA8210_NRESET_PIN    2
#define SPIBUF               SPI2BUF
#define SPISTATbits          SPI2STATbits
#endif

#define CA8210_SPI_BAUDRATE  2000000  /* 2 MHz */
#define CA8210_SPI_SETUP     (SPI_MASTER|SPI_CLOCK_IDLE_HIGH|SPI_SDO_ON_CLOCK_TO_IDLE)

#define AbsoluteTicks        clock_time()


/******************************************************************************/
/****** Global Variables defined in cascoda_bsp_*.c                      ******/
/******************************************************************************/


/******************************************************************************/
/****** Function Declarations for cascoda_bsp_*.c                        ******/
/******************************************************************************/
void  BSP_StartTimerUs( void );
void  BSP_StopTimerUs( void );
u32_t BSP_GetTimerUs( void );
u32_t BSP_ReadAbsoluteTime( void );
void  BSP_ShowTitle( u8_t *pString );
void  BSP_ShowTime( void );
void  BSP_WaitTicks( u32_t Ticks);
void  BSP_ResetRF(u8_t ms);
u8_t  BSP_SenseRFIRQ( void );
void  BSP_SetRFSSBHigh( void );
void  BSP_SetRFSSBLow( void );
void  BSP_SerialWriteAll( u8_t *pBuffer, u32_t BufferSize);
u32_t BSP_SerialRead( u8_t *pBuffer, u32_t BufferSize);
void  BSP_SPIInit( void );
u8_t  BSP_SPIExchangeByte( u8_t OutByte );
void  BSP_Initialise( void );


#endif // CASCODA_BSP_H
