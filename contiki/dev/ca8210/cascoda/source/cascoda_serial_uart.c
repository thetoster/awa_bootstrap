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
#include "dev/ca8210/ports/include/cascoda_bsp.h"
#include "dev/ca8210/cascoda/include/cascoda_serial.h"

#include <stdio.h>
#include <string.h>


/******************************************************************************/
/****** Definitions for Serial State                                     ******/
/******************************************************************************/
#define SERIAL_INBETWEEN    (0)
#define SERIAL_HEADER       (1)
#define SERIAL_DATA         (2)


/******************************************************************************/
/****** Global Variables for Serial State                                ******/
/******************************************************************************/
u8_t SerialCount;
u8_t SerialRemainder;
u8_t SerialIfState = SERIAL_INBETWEEN;


/******************************************************************************/
/****** Global Variables for Serial Message Buffers                      ******/
/******************************************************************************/
SerialBuffer_t SerialRxBuffer;
SerialBuffer_t SerialTxBuffer;


/******************************************************************************/
/****** Static Variables for Ascii Hex to Binary Conversion              ******/
/******************************************************************************/
#define SERIAL_CONV_FAIL    0xFFFF
u8_t TX_BYTE_COMPLETE = 0;
u8_t cin1, cin2;
u16_t ASCIIHextoBin(u8_t x1, u8_t x2);

#ifdef __USE_UART_PORT3_FOR_15_4_DEBUG__
#ifdef __USE_UART_PORT3_FOR_15_4_GUI__
#error 15_4 DEBUG: Cannot connect to both Terminal and GUI
#endif
#endif


/******************************************************************************/
/******************************************************************************/
/****** SerialGetCommand()                                               ******/
/******************************************************************************/
/****** Brief:  Read in next Command from Serial                         ******/
/******************************************************************************/
/****** Param:  c - character                                            ******/
/******************************************************************************/
/****** Return: 1 if Command ready, 0 if not                             ******/
/******************************************************************************/
/******************************************************************************/
/****** Note: Interrupt driven here, so no low-level function necessary  ******/
u8_t SerialGetCommand( u8_t c )
 {
 u8_t cmod;

#ifdef __USE_UART_PORT3_FOR_15_4_DEBUG__
    u16_t cext;
    printf("%c", c);
    if(TX_BYTE_COMPLETE)
     {
     cin2 = c;
     TX_BYTE_COMPLETE = 0;
     cext = ASCIIHextoBin(cin1, cin2);
     if(cext == SERIAL_CONV_FAIL)
      {
      SerialIfState = SERIAL_INBETWEEN;
      SerialRemainder = 0;
      SerialCount = 0;
	  printf("\n");
      return 0;
	  }
	 else
	  {
      cmod = (u8_t)cext;
      }
     }
    else
     {
     cin1 = c;
     TX_BYTE_COMPLETE = 1;
     return 0;
     }
#else
    cmod = c;
#endif



    if(SerialIfState == SERIAL_INBETWEEN)
     {
     if ( cmod == 0xDE )
      {
      SerialIfState = SERIAL_HEADER;
      SerialRemainder = SERIAL_HDR_LEN;
      SerialCount = 0;
      }
     return 0;
     }
    else if (SerialIfState == SERIAL_HEADER)
     {
     SerialRxBuffer.Header[SerialCount] = cmod;
     SerialCount += 1;
     SerialRemainder -= 1;
     if ( SerialRemainder == 0 )
      {
      SerialRemainder = SerialRxBuffer.Header[SERIAL_CMD_LEN];
      SerialCount = 0;
      if(SerialRemainder == 0)
       {
       SerialIfState = SERIAL_INBETWEEN;		// exit if 0 API packet length !!
       return 1;
       }
      else
       {
       SerialIfState = SERIAL_DATA;
       }
      }
     return 0;
     }
    else if (SerialIfState == SERIAL_DATA)
     {
     SerialRxBuffer.Data[SerialCount] = cmod;
     SerialCount += 1;
     SerialRemainder -= 1;
     if ( SerialRemainder == 0 )
      {
      SerialIfState = SERIAL_INBETWEEN;
      SerialRemainder = 0;
      SerialCount = 0;
#ifdef __USE_UART_PORT3_FOR_15_4_DEBUG__
	  printf("\n");
#endif
      return 1;
      }
     return 0;
     }

return 0;
} // End of SerialGetCommand()


/******************************************************************************/
/******************************************************************************/
/****** EVBME_Message()                                                  ******/
/******************************************************************************/
/****** Brief:  Send EVBME_MESSAGE_Indication Upstream                   ******/
/******************************************************************************/
/****** Param:  Count - Number of Characters                             ******/
/****** Param:  pBuffer - Pointer to Character Buffer                    ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void EVBME_Message( u8_t Count, u8_t *pBuffer )
{
#ifdef __USE_UART_PORT3_FOR_15_4_GUI__
 SerialTxBuffer.Header[SERIAL_CMD_ID] = 0xA0;
 SerialTxBuffer.Header[SERIAL_CMD_LEN] = Count;
 SerialTxBuffer.SOM = 0xDE;
 memcpy( SerialTxBuffer.Data, pBuffer, Count );
 BSP_SerialWriteAll( &SerialTxBuffer.SOM, Count + 1 + SERIAL_HDR_LEN );
#endif
#ifdef __USE_UART_PORT3_FOR_15_4_DEBUG__
 u8_t i;
 printf("CAX-E: ");
 for(i=0;i<Count;++i)
  printf("%c", pBuffer[i]);
 printf("\n");
#endif
} // End of EVBME_Message()


/******************************************************************************/
/******************************************************************************/
/****** MAC_Message()                                                    ******/
/******************************************************************************/
/****** Brief:  Send MCPS or MLME confirm or indication Upstream         ******/
/******************************************************************************/
/****** Param: CommandId - command id of confirm or indication           ******/
/****** Param:  Count - Number of Characters                             ******/
/****** Param:  pBuffer - Pointer to Character Buffer                    ******/
/******************************************************************************/
/****** Return: -                                                        ******/
/******************************************************************************/
/******************************************************************************/
void MAC_Message( u8_t CommandId, u8_t Count, u8_t *pBuffer )
{
#ifdef __USE_UART_PORT3_FOR_15_4_GUI__
  SerialTxBuffer.Header[SERIAL_CMD_ID] = CommandId;
  SerialTxBuffer.Header[SERIAL_CMD_LEN] = Count;
  SerialTxBuffer.SOM = 0xDE;
  memcpy( SerialTxBuffer.Data, pBuffer, Count );
  BSP_SerialWriteAll( &SerialTxBuffer.SOM, Count + 1 + SERIAL_HDR_LEN );
#endif
#ifdef __USE_UART_PORT3_FOR_15_4_DEBUG__
  u8_t i;
  printf("CAX-M: %02X %02X", CommandId, Count);
  for(i=0;i<Count;++i)
   printf(" %02X", pBuffer[i]);
  printf("\n");
#endif
} // End of MAC_Message()


/********************************************************************/
/****** Converts Hex ASCII input to extended unsigned          ******/
/****** u16_t: u8_t + FAIL                                     ******/
/********************************************************************/
u16_t ASCIIHextoBin(u8_t x1, u8_t x2)
 {
 u8_t d1, d2;

 if((x1 >= '0') && (x1 <= '9'))
  d1 = (x1 - '0');
 else if((x1 >= 'A') && (x1 <= 'F'))
  d1 = (x1 - 'A' + 0x0A);
 else if((x1 >= 'a') && (x1 <= 'f'))
  d1 = (x1 - 'a' + 0x0A);
 else
  return(SERIAL_CONV_FAIL);

 if((x2 >= '0') && (x2 <= '9'))
  d2 = (x2 - '0');
 else if((x2 >= 'A') && (x2 <= 'F'))
  d2 = (x2 - 'A' + 0x0A);
 else if((x2 >= 'a') && (x2 <= 'f'))
  d2 = (x2 - 'a' + 0x0A);
 else
  return(SERIAL_CONV_FAIL);

 return(d1*0x10+d2);
 }

