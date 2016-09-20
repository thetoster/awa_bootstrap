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

#ifndef CASCODA_DEBUG_H
#define CASCODA_DEBUG_H


/******************************************************************************/
/****** Function Declarations for cascoda_debug.c                        ******/
/******************************************************************************/
void dpFlush( void );
void dp8( u8_t Out );
void dp16( u16_t Out );
void dp32( u32_t Out );
void dps( char *pString );
void dptime( void );
void dparray( u8_t *pArray, u8_t Length );
void dparrayle( u8_t *pArray, u8_t Length );
void dpc( u8_t Out );
void dpnl( void );
void dpDec( u16_t Out );
void dp_u8_2h(u8_t x);
void dp_u8_3u(u8_t d);
void dp_u8_u(u8_t d);
void dp_s8_s(i8_t d);
void dp_u16_5u(u16_t d);
void dp_u16_u(u16_t d);
void dp_s16_s(i16_t d);
void dp_u32_10u(u32_t d);
void dp_u32_u(u32_t d);
void dpslow( char *pString );

#endif // CASCODA_DEBUG_H
