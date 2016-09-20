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

#include <pic32_irq.h>
#include "string.h"
#include "dev/ca8210/cascoda/include/cascoda_types.h"
#include "dev/ca8210/cascoda/ariadne/include/ariadne.h"

#define __enable_interrupt()  ASM_EN_INT
#define __disable_interrupt() ASM_DIS_INT

/*******************************************************************************
 * Heap definitions - Adjust to make sure spare RAM is all used
 ******************************************************************************/
u8_t     Heap[HEAP_SIZE];

/*******************************************************************************
 * A_MemoryInit()
 ******************************************************************************/
/**
 * \brief Initialise memory allocation
 */
void A_MemoryInit( void )
{
    A_HeapInit( Heap, HEAP_SIZE );
} // End of A_MemoryInit

/*******************************************************************************
 * A_Malloc()
 ******************************************************************************/
/**
 * \brief Allocate a block of memory
 * \param Size - No of bytes to allocate
 * \return pointer to buffer or NULLP
 */
u8_t *A_Malloc( u16_t Size )
{
    u8_t  iQ;
    u8_t  *pMemory = NULLP;

    for ( iQ = 0; iQ < NO_OF_QUEUES; iQ++ )
    {
        if ( Size <= ( ChunkSizes[iQ] - 4 ) )
        {
            pMemory = A_HeapGet( iQ );
            if ( pMemory != NULLP )
            {
                *pMemory = iQ;
                pMemory += 4;
            }
            break;
        }
    }
    return pMemory;
} // End of A_Malloc()

/*******************************************************************************
 * A_Realloc()
 ******************************************************************************/
/**
 * \brief Reallocate a block of memory
 *
 * 'Stupid' implementation that just uses A_Malloc and A_Free to deallocate old pointer,
 * allocate a new one and copy the data across
 *
 * The old pointer will be freed, and the function returns a pointer to the new block.
 * The contents of the block is unchanged up to the lesser of the new and old sizes.
 * If the new size is larger, the contents of the newly allocated portion of the block is unspecified.

 * If size is 0 and ptr is not a null pointer, the block is freed.
 * If ptr is a null pointer, A_Realloc is equivalent to A_Malloc fort
 *
 * \param ptr - pointer to the block previously allocated with A_Malloc or A_Realloc
 * \param ptr - the new size for the block, in bytes
 * @return a pointer to the newly allocated memory, or NULLP if the request fails
 */
u8_t *A_Realloc(u8_t *ptr, u16_t size){
    if (ptr == NULLP){
        return A_Malloc(size);
    } else if (size == 0){
        A_Free(ptr);
        return NULLP;
    }

    u8_t *newPtr = A_Malloc(size);
    if (newPtr == NULLP){
        return NULLP;
    } else {
        memcpy(newPtr, ptr, size); //may go past the end of newPtr, so value of newly allocated portion is unspecified
        A_Free(ptr);
        return newPtr;
    }
} // End of A_Realloc()

/*******************************************************************************
 * A_Free()
 ******************************************************************************/
/**
 * \brief Free a block of memory
 * \param pMemory - pointer to memory
 */
void A_Free( u8_t *pMemory )
{
    if ( pMemory != NULLP )
    {
        pMemory-=4;
        if ( *pMemory > NO_OF_QUEUES )
        {
            //TDME_HWERROR = TDME_ERR_SYSTEM_ERROR;
            //dps("A_Free() system error\n");
            //dp16((u16_t)pMemory);dps("->");dparray(pMemory,10);dpnl();
        }
        else
        {
            if ( A_HeapCheck( *pMemory, (a_chunk_t *)pMemory ) == 0 )
            {
                A_HeapPut( *pMemory, (a_chunk_t *)pMemory );
            }
        }
    }
} // End of A_Free()


/*******************************************************************************
 * A_Memxor_x2x()
 ******************************************************************************/
/**
 * \brief XOR two byte arrays of equal length
 * \param pTarget - target array of the xor
 * \param pOperator - array to xor with
 * \param Length - number of bytes
 * \return pOperator + Length
 */
u8_t *A_Memxor_x2x( u8_t *pTarget, u8_t *pOperator, u8_t Length )
{
    u8_t    i;

    for ( i = 0; i < Length; i++ )
    {
        *pTarget++ ^= *pOperator++;
    }
    return pOperator;
} // End of A_Memxor_x2x()


void A_MemShiftLeft( u8_t *pTarget, u8_t Length )
{
    u8_t i;

    for (i = 0;  i < Length - 1;  ++i)
    {
        pTarget[i] = (pTarget[i] << 1) | ((pTarget[i+1] >> 7) & 1);
    }
    pTarget[Length-1] = pTarget[Length-1] << 1;
}

void A_MemShiftRight( u8_t *pTarget, u8_t Length )
{
    u8_t i;

    for(i = Length - 1; i > 0; --i)
    {
        pTarget[i] = (pTarget[i] >> 1) | ((pTarget[i-1] << 7) & 0x80);
    }
    pTarget[0] = pTarget[0] >> 1;
}

/*******************************************************************************
 * A_QPutTail()
 ******************************************************************************/
/**
 * \brief Put Item on tail of queue
 * \param   Queue - pointer pTo queue
 * \param   Item - pointer pTo item
 */
void A_QPutTail( a_queue_t *pQueue, a_item_t *pItem )
{
    a_item_t  *pTail;

    __disable_interrupt();
    if ( ( pTail = pQueue->pTail ) == NULLP )
    {
        pTail = pItem;
    }
    else
    {
        pItem->pNext = pTail->pNext;
    }
    pTail->pNext = pItem;
    pQueue->pTail = pItem;
    __enable_interrupt();
} // A_QPutTail()

/*******************************************************************************
 * A_QPutHead()
 ******************************************************************************/
/**
 * \brief Put Item on head of queue
 * \param   Queue - pointer pTo queue
 * \param   Item - pointer pTo item
 */
void A_QPutHead( a_queue_t *pQueue, a_item_t *pItem )
{
    a_item_t  *pTail;

    __disable_interrupt();
    if ( ( pTail = pQueue->pTail ) == NULLP )
    {
        pQueue->pTail = pItem->pNext = pItem;
    }
    else
    {
        pItem->pNext = pTail->pNext;
        pTail->pNext = pItem;
    }
    __enable_interrupt();
} // A_QPutHead()

/*******************************************************************************
 * A_QGetHead()
 ******************************************************************************/
/**
 * \brief Get Head of Queue
 * \param   Queue - pointer pTo queue
 * \return  Item - pointer pTo item
 */
a_item_t *A_QGetHead( a_queue_t *pQueue )
{
    a_item_t  *pHead, *pTail;

    __disable_interrupt();
    if ( ( pTail = pQueue->pTail ) == NULLP )
    {
        __enable_interrupt();
        return NULLP;
    }
    if ( ( pHead = pTail->pNext ) == pTail )
    {   /* This was the only item so the queue is now empty */
        pQueue->pTail = NULLP;
    }
    else
    {
        pTail->pNext = pHead->pNext;
    }
    __enable_interrupt();
    return pHead;
} // A_QGetHead()

/*******************************************************************************
 * A_PoolPut()
 ******************************************************************************/
/**
 * \brief Put Item in pool
 * \param   pPool - pointer to list
 * \param   pItem - pointer to item
 */
void A_PoolPut( a_pool_t *pPool, a_item_t *pItem )
{
    pItem->pNext = pPool->pHead;
    pPool->pHead = pItem;
} // A_PoolPut()

/*******************************************************************************
 * A_PoolGet()
 ******************************************************************************/
/**
 * \brief Item from pool
 * \param   pPool - pointer to pool
 * \return  pItem - pointer to item
 */
a_item_t *A_PoolGet( a_pool_t *pPool )
{
    a_item_t *pHead;

    if ( ( pHead = pPool->pHead ) != NULLP )
    {
        pPool->pHead = pHead->pNext;
    }
    return pHead;
} // A_PoolGet()

