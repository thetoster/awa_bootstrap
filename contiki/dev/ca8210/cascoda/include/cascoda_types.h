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

#ifndef CASCODA_TYPES_H
#define CASCODA_TYPES_H


/******************************************************************************/
/****** Typedefs                                                         ******/
/******************************************************************************/
typedef  unsigned       char   u8_t;
typedef    signed       char   i8_t;
typedef  unsigned      short  u16_t;
typedef    signed      short  i16_t;
typedef  unsigned       long  u32_t;
typedef    signed       long  i32_t;
typedef  unsigned  long long  u64_t;
typedef    signed  long long  i64_t;


/******************************************************************************/
/****** Defines                                                          ******/
/******************************************************************************/
#define NULLP   0


/******************************************************************************/
/****** Macros                                                           ******/
/******************************************************************************/
#define LS_BYTE(x)  ((u8_t)((x)&0xFF))
#define MS_BYTE(x)  ((u8_t)(((x)>>8)&0xFF))
#define LS0_BYTE(x) ((u8_t)((x)&0xFF))
#define LS1_BYTE(x) ((u8_t)(((x)>>8)&0xFF))
#define LS2_BYTE(x) ((u8_t)(((x)>>16)&0xFF))
#define LS3_BYTE(x) ((u8_t)(((x)>>24)&0xFF))

#define GETLE16(x)      (((u16_t)(x)[1]<<8)+(x)[0])
#define GETLE32(x)      (((u32_t)(x)[3]<<24)+((u32_t)(x)[2]<<16)+((u32_t)(x)[1]<<8)+(x)[0])
#define PUTLE16(x,y)    {(y)[0]=((x)&0xff);(y)[1]=((x)>>8);}
#define PUTLE32(x,y)    {(y)[0]=((x)&0xff);(y)[1]=(((x)>>8)&0xff);(y)[2]=(((x)>>16)&0xff);(y)[3]=(((x)>>24)&0xff);}


#endif // CASCODA_TYPES_H
