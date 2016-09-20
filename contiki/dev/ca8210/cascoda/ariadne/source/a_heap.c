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

/*******************************************************************************
 * Declarations for heap
 ******************************************************************************/

a_chunk_t     *pHeap;
a_chunk_t     *HeapQueues[NO_OF_QUEUES];
u16_t          ChunkSizes[NO_OF_QUEUES];

/*******************************************************************************
 * Macro to find a heap buddy
 ******************************************************************************/
#define BUDDY(b,q)  (pHeap+((b-pHeap)^(1<<q)))

/*******************************************************************************
 * A_HeapInit()
 ******************************************************************************/
/**
 * \brief Initialize Heap queues
 */
void A_HeapInit( u8_t *pMemory, u16_t MemorySize )
{
    u8_t          iQ;
    u16_t         ChunkSize;
    a_chunk_t     **ppChain;

    for ( iQ = 0, ChunkSize = MIN_BLOCK_SIZE;
          iQ < NO_OF_QUEUES;
          iQ++, ChunkSize *= 2 )
    {
        ChunkSizes[iQ] = ChunkSize;
    }

    pHeap = (a_chunk_t *)pMemory;
    memset( HeapQueues, 0, sizeof(HeapQueues) );

    for ( iQ = NO_OF_QUEUES - 1; iQ != 0; iQ-- )
    {
        ChunkSize = ChunkSizes[iQ];
        ppChain = HeapQueues + iQ;
        while ( MemorySize > ChunkSize )
        {
            *ppChain = (a_chunk_t *)pMemory;
            ppChain = &((a_chunk_t *)pMemory)->pNext;
            pMemory += ChunkSize;
            MemorySize -= ChunkSize;
        }
        *ppChain = NULLP;
    }
} // End of A_HeapInit()

/*******************************************************************************
 * A_HeapPut()
 ******************************************************************************/
/**
 * \brief Put memory on heap
 * \param   iQ - index to heap queue for this size
 * \param   pMemory - pointer to item
 */
void A_HeapPut( u8_t iQ, a_chunk_t *pMemory )
{
    a_chunk_t           *pThis;
    static a_chunk_t    *pBuddy;
    static a_chunk_t    **ppPrev;


    for ( ppPrev = HeapQueues + iQ;
        ( pThis = *ppPrev ) != NULLP;
        iQ++, ppPrev = HeapQueues + iQ )
    {
        // If this is the last queue look for the buffer otherwise look for its buddy
        // (Looking for the buffer wont succeed but we will find correct queue position)
        pBuddy = pMemory;
        if ( iQ < ( NO_OF_QUEUES - 1 ) )
        {
            pBuddy = BUDDY( pMemory, iQ );
        }

        while ( pBuddy > pThis )
        {
            ppPrev = &pThis->pNext;
            pThis = pThis->pNext;
            if ( pThis == NULLP )
            {
                break;
            }
        }
        if ( pBuddy != pThis )
        {   // So we reached the queue end OR pThis is bigger and
            // since the queue is ordered, pLookFor isn't there
            break;
        }
        // pThis is the one we are looking for so it must be the buddy
        // unchain the buddy (pThis)
        *ppPrev = pThis->pNext;
        // set pMemory to the smallest of pMemory and its buddy to merge them
        if ( pThis < pMemory )
        {
            pMemory = pThis;
        }
        // continue round again to free enlarged pMemory
    }
    // put pBuff on queue after pPrev
    pMemory->pNext = *ppPrev;
    *ppPrev = pMemory;
} // End of A_HeapPut()

/*******************************************************************************
 * A_HeapGet()
 ******************************************************************************/
/**
 * \brief Get memory from heap
 * \param   iQStart - index to heap queue for this size
 * \return  pMemory - pointer to item
 */
u8_t *A_HeapGet( u8_t iQStart )
{
    u8_t    iQ;
    static a_chunk_t    *pResult, *pNext;
    static a_chunk_t    *pBuddy;
    static a_chunk_t    **ppPrev;

    pResult = NULLP;
    // If there are no buffers on iQStart loop round looking
    // at queues of larger blocks until one is found
    for ( iQ = iQStart; iQ < NO_OF_QUEUES; iQ++ )
    {
        if ( ( pResult = HeapQueues[iQ]) != NULLP )
        {
            HeapQueues[iQ] = pResult->pNext;
            break;
        }
    }
    if ( pResult != NULLP )
    {
        while ( iQ != iQStart )
        {   // If the block was bigger than we needed, split it up
            iQ--;
            pBuddy = BUDDY( pResult, iQ );
            for ( ppPrev = HeapQueues + iQ, pNext = *ppPrev;
                  pNext > pBuddy;
                  pNext = pNext->pNext )
            {
                ppPrev = &pNext->pNext;
            }
            // Free the split off buddy onto iQ
            pBuddy->pNext = *ppPrev;
            *ppPrev = pBuddy;
            // Go round until the allocated block is the right size
        }
    }
    return pResult->Buffer;

} // End of A_HeapGet()

/*******************************************************************************
 * A_HeapCheck()
 ******************************************************************************/
/**
 * \brief Check parameters for HeapPut() for debug purposes
 * \param   iQ - index to heap queue for this size
 * \param   pMemory - pointer to item
 */
u8_t A_HeapCheck( u8_t iQ, a_chunk_t *pMemory )
{
    a_chunk_t           *pThis;
    u8_t                Result = 0;

    /* First check address is within Heap */
    if ( ( pMemory->Buffer < pHeap->Buffer )
        || ( pMemory->Buffer > ( pHeap->Buffer + ( HEAP_SIZE - ChunkSizes[iQ] ) ) ) )
    {
        // Address wrong */
        Result = 1;
    }
    /* Then check alignment */
    else if ( ( ( pMemory->Buffer - pHeap->Buffer ) % ChunkSizes[iQ] ) != 0 )
    {
        /* Alignment wrong */
        Result = 2;
    }
    /* Last check if already on heap */
    else
    {
        for ( iQ = 0; iQ < NO_OF_QUEUES; iQ++ )
        {
            for ( pThis = HeapQueues[iQ]; pThis != NULLP; pThis = pThis->pNext )
            {
                if ( pThis == pMemory )
                {
                    Result = 3;
                    break;
                }
            }
        }
    }
    if ( Result != 0 )
    {
        Result*= 10;       /* Instruction to break on */
    }

    return Result;
} // End of HeapCheck()
