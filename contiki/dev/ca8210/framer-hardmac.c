/*
 * Copyright (c) 2009, Swedish Institute of Computer Science.
 * Copyright (c) 2016, Cascoda Limited
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
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/**
 * \file
 *         MAC framer for nullmac
 * \author
 *         Niclas Finne <nfi@sics.se>
 *         Joakim Eriksson <joakime@sics.se>
 */

/*---------------------------------------------------------------------------*/
// Adapted from framer-nullmac.
// The Radio contains a full 802.15.4 MAC according to IEEE802.15.4 2006.
// However, the SICSLOWPAN network layer is using the NETSTACK_FRAMER.length()
// function to calculate fragment size. This needs to be the actual value.
/*---------------------------------------------------------------------------*/

#include "dev/ca8210/framer-hardmac.h"
#include "net/packetbuf.h"

#define DEBUG 0

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINTADDR(addr) PRINTF(" %02x%02x:%02x%02x:%02x%02x:%02x%02x ", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7])
#else
#define PRINTF(...)
#define PRINTADDR(addr)
#endif

struct nullmac_hdr {
  linkaddr_t receiver;
  linkaddr_t sender;
};

/*---------------------------------------------------------------------------*/
static int
hdr_length(void)
{
int hdrlen;

hdrlen    =  2;     // fcf
hdrlen   +=  1;     // dsn
hdrlen   +=  2;     // panid - hard-coded, so panid compression should always be true

if(LINKADDR_SIZE == 2)
 hdrlen  +=  4;   // 2 * 16-bit shortaddr
else
 hdrlen  += 16;   // 2 * 64-bit ieeeaddr

#if LLSEC802154_SECURITY_LEVEL

if(packetbuf_attr(PACKETBUF_ATTR_SECURITY_LEVEL))
 {
 hdrlen  +=  5;   // 1 byte security control + 4 bytes frame counter
 #if LLSEC802154_USES_EXPLICIT_KEYS
 int key_id_mode;
 key_id_mode = packetbuf_attr(PACKETBUF_ATTR_KEY_ID_MODE);
 if(key_id_mode      ==  FRAME802154_1_BYTE_KEY_ID_MODE)
  hdrlen +=  1;   // key from macDefaultKeySource, 1 byte index
 else if(key_id_mode ==  FRAME802154_5_BYTE_KEY_ID_MODE)
  hdrlen +=  5;   // key explicitly, 4 bytes key source + 1 byte index
 else if(key_id_mode ==  FRAME802154_9_BYTE_KEY_ID_MODE)
  hdrlen +=  9;   // key explicitly, 8 bytes key source + 1 byte index
 else
  hdrlen +=  0;   // key implicitly
 }

#endif // LLSEC802154_USES_EXPLICIT_KEYS

#endif // LLSEC802154_SECURITY_LEVEL

return hdrlen;
}

/*---------------------------------------------------------------------------*/
static int
create(void)
{
  struct nullmac_hdr *hdr;

  if(packetbuf_hdralloc(sizeof(struct nullmac_hdr))) {
    hdr = packetbuf_hdrptr();
    linkaddr_copy(&(hdr->sender), &linkaddr_node_addr);
    linkaddr_copy(&(hdr->receiver), packetbuf_addr(PACKETBUF_ADDR_RECEIVER));
    return sizeof(struct nullmac_hdr);
  }
  PRINTF("PNULLMAC-UT: too large header: %u\n", sizeof(struct nullmac_hdr));
  return FRAMER_FAILED;
}
/*---------------------------------------------------------------------------*/
static int
parse(void)
{
  struct nullmac_hdr *hdr;
  hdr = packetbuf_dataptr();
  if(packetbuf_hdrreduce(sizeof(struct nullmac_hdr))) {
    packetbuf_set_addr(PACKETBUF_ADDR_SENDER, &(hdr->sender));
    packetbuf_set_addr(PACKETBUF_ADDR_RECEIVER, &(hdr->receiver));

    PRINTF("PNULLMAC-IN: ");
    PRINTADDR(packetbuf_addr(PACKETBUF_ADDR_SENDER));
    PRINTADDR(packetbuf_addr(PACKETBUF_ADDR_RECEIVER));
    PRINTF("%u (%u)\n", packetbuf_datalen(), sizeof(struct nullmac_hdr));

    return sizeof(struct nullmac_hdr);
  }
  return FRAMER_FAILED;
}
/*---------------------------------------------------------------------------*/
const struct framer framer_hardmac = {
  hdr_length,
  create,
  framer_canonical_create_and_secure,
  parse
};
