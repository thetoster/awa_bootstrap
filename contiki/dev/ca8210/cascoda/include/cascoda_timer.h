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

#ifndef CASCODA_TIMER_H
#define CASCODA_TIMER_H

#include <stdbool.h>
#include "dev/ca8210/cascoda/include/cascoda_types.h"


/******************************************************************************/
/****** Configuration settings											 ******/
/******************************************************************************/

#define TIMER_DEBUG_ENABLED false 	//!Enable or disable debug messages
#define GROUPMAP_START_SIZE (8)		//!The initial size of the array used to store created groups


/******************************************************************************/
/****** Handler function signatures										 ******/
/******************************************************************************/

/** Signature for timeout handler function
*/
typedef void (TIMER_TimeoutHandler)(void);


/** Signature for event handler function
	The argument passed to the handler will be the pointer given to the corresponding TIMER_trigger call.
*/
typedef void (TIMER_EventHandler)(const void* params);


/******************************************************************************/
/****** API functions 													 ******/
/******************************************************************************/

/** Obtain a timer group handle for use by your application

	The returned handle should be stored for later use in other timer functions.
	You must only use group handles in your module that you obtained via this function.

	@return The handle of the new timer group
*/
u8_t TIMER_createGroup(void);


/** Begin a wait timer for an event

	The deadline of the event timer will be BSP_currentTime_ms + duration_ms

	@param group the id of the group
	@param eventId the id of the specific event. May not be 0.
	@param duration_ms the duration in milliseconds to wait for before timing out
	@param handler the function to run if the event is triggered, or NULLP to run nothing
	@param timeoutHandler the function to run if the event is not triggered within duration_ms, or NULLP to run nothing
	@return void
*/
void TIMER_await(
	u8_t group,
	u8_t eventId,
	u32_t duration_ms,
	TIMER_EventHandler* handler,
	TIMER_TimeoutHandler* timeoutHandler
);

/** Begin a wait timer for an event, continuing with the deadline of the event currently being handled

	If this function is called within the handler or timeout handler function of an event,
	the deadline of the new event timer will be the deadline of the event currently being handled.
	Otherwise, the deadline will be 0 (i.e. immediate)

	@param group the id of the group for the new event
	@param eventId the id of the specific event to create. May not be 0.
	@param handler the function to run if the event is triggered, or NULLP to run nothing
	@param timeoutHandler the function to run if the event is not triggered within duration_ms, or NULLP to run nothing
	@return void
*/
void TIMER_awaitContinue(
	u8_t group,
	u8_t eventId,
	TIMER_EventHandler* handler,
	TIMER_TimeoutHandler* timeoutHandler
);


/** Trigger a single event

	If eventId is 0, the timer in the given group with the earliest deadline is triggered, regardless of eventId.

	The memory allocated for the triggered timer will not be freed until the next call to TIMER_monitor.

	@param group the event group ID of the event.
	@param eventId the ID of the specific event, or 0x00 to trigger the soonest event in the group
	@param params the parameters of the occuring event instance.
	@retval true if the triggered event was being waited for
	@retval false if the triggered event was not waited for
*/
bool TIMER_trigger(u8_t group, u8_t eventId, const void* params);


/** Check for and execute event await timeouts

	All events in the given group with deadlines earlier than the time at which this function
	is entered are timed out, in order from earliest to latest.

	This function must NOT be called inside itself, i.e. it must not be called from inside a timeout handler.

	@param group the group to monitor, or 0xff for all groups
	@return the number of event timeouts that were performed
*/
u8_t TIMER_monitor( u8_t group );


/** Count how many of the specified event are currently being waited for

	If eventId is 0, all timers with the given group are counted regardless of eventId.

	@param group the group of the event
	@param event the specific event id to count, or 0x00 to count all events in the group
	@return the number of matching events with active timers
*/
u8_t TIMER_waitingFor(u8_t group, u8_t eventId);


/** Stop waiting for an event

	Aborts all timers with the given group and eventId.
	If eventId is 0, all timers with the given group are aborted regardless of eventId.

	The memory allocated for the aborted timer will not be freed until the next call to TIMER_monitor.

	@param group the event group ID to stop waiting for
	@param eventId the event id to stop waiting for, or 0 to stop waiting for entire group
	@return the number of events aborted
*/
u8_t TIMER_abort(u8_t group, u8_t eventId);



#endif //CASCODA_TIMER_H