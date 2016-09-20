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

/*---------------------------------------------------------------------------*/
// PHY and MAC Tests Plug-In for CA8210 Driver
// Can be used with either Cascoda GUI or Terminal
/*---------------------------------------------------------------------------*/

#include <contiki.h>
#include <sys/etimer.h>

#include "net/packetbuf.h"
#include "net/netstack.h"

#include "ca8210-radio.h"
#include "ca8210-conf.h"
#include "dev/ca8210/cascoda/include/cascoda_api.h"
#include "dev/ca8210/cascoda/include/cascoda_evbme.h"
#include "dev/ca8210/apps/test15_4/include/test15_4_phy_evbme.h"
#include "dev/ca8210/apps/test15_4/include/test15_4_mac_evbme.h"
#include "dev/ca8210/apps/test15_4/include/test15_4_phy_tests.h"

#define DEBUGP 0
#if DEBUGP
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


/*---------------------------------------------------------------------------*/
// RADIO Process Thread Declaration
/*---------------------------------------------------------------------------*/
PROCESS(ca8210_test_process, "CA8210 test process");


/*---------------------------------------------------------------------------*/
// Radio Test Handler Process Thread
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ca8210_test_process, ev, data)
{
  PROCESS_BEGIN();

  static struct etimer etm;

  PRINTF("ca8210_test_process: started\n");

  // 1 ms tick
  etimer_set(&etm, CLOCK_SECOND/1000);

  while(1)
   {
   TEST15_4_PHY_Handler();

   PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etm));
   etimer_reset(&etm);
   }
  PROCESS_END();
}


/*---------------------------------------------------------------------------*/
// Test Modes Downstream Dispatch (from SPI)
/*---------------------------------------------------------------------------*/
void ca8210_test_downstreamdispatch(void)
{
 // Insert Application-Specific DownStream Dispatches (from SPI) here
 TEST15_4_PHY_DownStreamDispatch();
 TEST15_4_MAC_DownStreamDispatch();
}


/*---------------------------------------------------------------------------*/
// Serial (UART) Interrupt Handler for Upstream Data from GUI/Terminal
/*---------------------------------------------------------------------------*/
#if defined __USE_UART_PORT3_FOR_15_4_DEBUG__ || __USE_UART_PORT3_FOR_15_4_GUI__
u8_t SerialGetCommand( u8_t c );

int ca8210_uart_15_4_interrupt(unsigned char c)
{

if(SerialGetCommand((u8_t)c))
 {
 // Insert Application-Specific UpStream Dispatches (from Serial) here
 TEST15_4_PHY_UpStreamDispatch();
 TEST15_4_MAC_UpStreamDispatch();

 EVBMEUpStreamDispatch((u8_t *)CA8210_VERSION);
 EVBMESendDownStream();
 }

return(0);
}
#endif


/*---------------------------------------------------------------------------*/
