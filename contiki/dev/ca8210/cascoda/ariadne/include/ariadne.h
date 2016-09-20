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

#ifndef ARIADNE_H
#define ARIADNE_H

/*******************************************************************************
 * Limit definitions
 ******************************************************************************/
#define HEAP_SIZE             (5000)
#define MIN_BLOCK_SIZE        (8)
#define NO_OF_QUEUES          (8)       // Gives maximum 8 * (2^8) = 2048
extern u16_t ChunkSizes[NO_OF_QUEUES];

/*******************************************************************************
 * Structures
 ******************************************************************************/

typedef struct a_item_s
{
    struct a_item_s   *pNext;
} a_item_t;

typedef union a_chunk_s
{
    union a_chunk_s   *pNext;
    u8_t              Buffer[MIN_BLOCK_SIZE];
} a_chunk_t;

/* A QUEUE is an ordered circle of items with the queue pointer
   pointing to the tail item ( and the tail item points to the head).
   In an empty queue the tail pointer is NULLP. Actions are GetHead
   and PutTail */
typedef struct a_queue_s
{
    a_item_t          *pTail;
} a_queue_t;

/* A POOL is an unordered chain of items with the pool pointer
   pointing to the head. The pointer in the tail item is NULLP.
   Actions are GetHead and PutHead */
typedef struct a_pool_s
{
    a_item_t                  *pHead;
} a_pool_t;

/* A CHAIN is an ordered chain of items with the chain pointer
   pointing to the head. The pointer in the tail item is NULLP.
   Actions are bespoke. */
typedef struct a_chain_s
{
    a_item_t                  *pHead;
} a_chain_t;


/*******************************************************************************
 * Macro Procedure definitions
 ******************************************************************************/
#define A_Minimum(a,b)        ((a<b)?a:b)
#define A_Maximum(a,b)        ((a<b)?b:a)
#define A_LSByte(a)           ((u8_t)(((u16_t)(a))&0xFF))
#define A_MSByte(a)           ((u8_t)((((u16_t)(a))>>8)&0xFF))
#define A_GetLE16(x)          (((u16_t)(x)[1]<<8)+(x)[0])
#define A_GetLE24(x)          (((u32_t)(x)[2]<<16)+((u32_t)(x)[1]<<8)+(x)[0])
#define A_GetLE32(x)          (((u32_t)(x)[3]<<24)+((u32_t)(x)[2]<<16)+((u32_t)(x)[1]<<8)+(x)[0])
#define A_PutLE16(x,y)        {(y)[0]=((x)&0xff);(y)[1]=((x)>>8);}
#define A_PutLE24(x,y)        {(y)[0]=((x)&0xff);(y)[1]=(((x)>>8)&0xff);(y)[2]=(((x)>>16)&0xff);}
#define A_PutLE32(x,y)        {(y)[0]=((x)&0xff);(y)[1]=(((x)>>8)&0xff);(y)[2]=(((x)>>16)&0xff);(y)[3]=(((x)>>24)&0xff);}
#define A_GetBE16(x)          (((u16_t)(x)[0]<<8)+(x)[1])
#define A_PutBE16(x,y)        {(y)[1]=((x)&0xff);(y)[0]=((x)>>8);}
#define A_GetBE32(x)          (((u32_t)(x)[0]<<24)+((u32_t)(x)[1]<<16)+((u32_t)(x)[2]<<8)+(x)[3])
#define A_PutBE32(x,y)        {(y)[3]=((x)&0xff);(y)[2]=(((x)>>8)&0xff);(y)[1]=(((x)>>16)&0xff);(y)[0]=(((x)>>24)&0xff);}

#define A_QEmpty(q)           (q.pTail==NULLP)

/*******************************************************************************
 * Procedure definitions
 ******************************************************************************/

/*******************************************************************************
 * Procedure definitions in a_memory.c
 ******************************************************************************/

u8_t              *A_Memxor_x2x( u8_t *pTarget, u8_t *pOperator, u8_t Length );
void              A_MemShiftLeft( u8_t *pTarget, u8_t Length );
void              A_MemShiftRight( u8_t *pTarget, u8_t Length );
void              A_QPutTail( a_queue_t *pQueue, a_item_t *pItem );
void              A_QPutHead( a_queue_t *pQueue, a_item_t *pItem );
a_item_t          *A_QGetHead( a_queue_t *pQueue );
void              A_PoolPut( a_pool_t *pPool, a_item_t *pItem );
a_item_t          *A_PoolGet( a_pool_t *pPool );
void              A_HeapInit( u8_t *pMemory, u16_t MemorySize );
void              A_HeapPut( u8_t iQ, a_chunk_t *pMemory );
u8_t              *A_HeapGet( u8_t iQStart );
u8_t              A_HeapCheck( u8_t iQ, a_chunk_t *pMemory );

void              A_MemoryInit( void );
u8_t              *A_Malloc( u16_t Size );
u8_t              *A_Realloc( u8_t *pMemory, u16_t Size );
void              A_Free( u8_t *pMemory );

/*******************************************************************************
 * Procedure definitions in a_trace.c
 ******************************************************************************/
void              A_TraceInit( void );
void              A_Trace( u8_t Type, u8_t Length, u8_t *pBuffer );

#endif  // ARIADNE_H






