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

#ifndef CA8210_RADIO_H
#define CA8210_RADIO_H

#include "contiki.h"
#include "dev/radio.h"


/*---------------------------------------------------------------------------*/
// CA8210 Function Declarations
/*---------------------------------------------------------------------------*/
int  ca8210_init(void);
int  ca8210_init_pib(int channel, unsigned panid, unsigned addr, const uint8_t *ieee_addr);
void ca8210_reverse_bytes(uint8_t *u8buf, uint8_t n);
void ca8210_get_random(uint8_t *num, int count);
int  ca8210_get_txpower(void);
int  ca8210_interrupt(void);
void ca8210_test_downstreamdispatch(void);


/*---------------------------------------------------------------------------*/
// CA8210 NETSTACK_RADIO Driver
/*---------------------------------------------------------------------------*/
extern const struct radio_driver ca8210_driver;


/*---------------------------------------------------------------------------*/
// CA8210 Common Definitions
/*---------------------------------------------------------------------------*/
#define CA8210_MIN_CHANNEL 11
#define CA8210_MAX_CHANNEL 26

#define CA8210_TXPOWER_MIN  0
#define CA8210_TXPOWER_MAX  8

#define CA8210_BROADCAST_ADDRESS 0xFFFF

#define CA8210_VERSION  "CONTIKI-1.0"

#define CA8210_LOWPOWER_MODE  0x24   // standby - wake-up by GPIO

// minimum interval time [ms] between transmissions (0 turns timing check off)
#define CA8210_T_MIN_TX  40

// switch endian-ness of addressing
#define CA8210_REVERSE_ADDR_BYTEORDER 1


#endif /* CA8210_RADIO_H */
