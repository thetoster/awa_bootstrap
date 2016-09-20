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

#include "string.h"
#include "dev/ca8210/cascoda/include/cascoda_types.h"
#include "dev/ca8210/cascoda/ariadne/include/ariadne.h"

typedef struct trace_s
{
    a_item_t            Link;
    u8_t                Type;
    u8_t                Length;
    u8_t                Buffer[];
} trace_t;


a_queue_t               Trace;


/******************************************************************************/
/******************************************************************************/
/****** A_TraceInit()                                                    ******/
/******************************************************************************/
/****** Brief:  Empty trace queue                                        ******/
/******************************************************************************/
/****** Param:  None                                                     ******/
/******************************************************************************/
/****** Return: None                                                     ******/
/******************************************************************************/
/******************************************************************************/

void A_TraceInit( void )
{
    Trace.pTail = NULLP;
} // End of A_TraceInit()

/******************************************************************************/
/******************************************************************************/
/****** A_Trace()                                                        ******/
/******************************************************************************/
/****** Brief:  Add item to trace queue                                  ******/
/******************************************************************************/
/****** Param:  Type - Code identifying item                             ******/
/****** Param:  Length - length of data in buffer                        ******/
/****** Param:  pBuffer - pointer to trace data                          ******/
/******************************************************************************/
/****** Return: None                                                     ******/
/******************************************************************************/
/******************************************************************************/

void A_Trace( u8_t Type, u8_t Length, u8_t *pBuffer )
{
    trace_t     *pTraceBuffer;

    pTraceBuffer = (trace_t *)A_Malloc( Length + sizeof(trace_t) );
    pTraceBuffer->Type = Type;
    pTraceBuffer->Length = Length;
    memcpy( pTraceBuffer->Buffer, pBuffer, Length );
    A_QPutTail( &Trace, &pTraceBuffer->Link );
} // End of A_Trace()






