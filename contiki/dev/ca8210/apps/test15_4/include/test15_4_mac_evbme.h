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

/******************************************************************************/
/******************************************************************************/
/****** Cascoda Ltd. 2014, CA-821X EVB Driver Code                       ******/
/******************************************************************************/
/******************************************************************************/
/****** EvaBoard Management Entity (EVBME) Definitions/Declarations      ******/
/******************************************************************************/
/******************************************************************************/
/****** Revision           Notes                                         ******/
/******************************************************************************/
/****** 1.0  19/07/14  WB  Release Baseline                              ******/
/******************************************************************************/
/******************************************************************************/
#include "dev/ca8210/cascoda/include/cascoda_types.h"

#ifndef TEST15_4_MAC_EVBME_H
#define TEST15_4_MAC_EVBME_H


/******************************************************************************/
/****** EVBME Command ID Codes                                           ******/
/******************************************************************************/
#define EVBME_PIB_TEST_REQUEST          (0x86)
#define EVBME_SCAN_TEST_REQUEST         (0x87)
#define EVBME_SECPIB_TEST_REQUEST       (0x88)
#define EVBME_FFD_INIT_REQUEST          (0x89)
#define EVBME_FFD_START_REQUEST         (0x8A)
#define EVBME_DATA_TEST_REQUEST         (0x8B)
#define EVBME_RFD_ASSOCIATE_REQUEST     (0x8C)
#define EVBME_FFD_AWAIT_ASSOC_REQUEST   (0x8D)
#define EVBME_FFD_AWAIT_ORPHAN_REQUEST  (0x8E)


/******************************************************************************/
/****** EVBME API Functions                                              ******/
/******************************************************************************/
void TEST15_4_MAC_UpStreamDispatch(void);
void TEST15_4_MAC_DownStreamDispatch(void);


#endif // TEST15_4_MAC_EVBME_H
