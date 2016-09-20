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

#include "dev/ca8210/cascoda/include/cascoda_timer.h"

#include "dev/ca8210/cascoda/ariadne/include/ariadne.h"
#include "dev/ca8210/ports/include/cascoda_bsp.h"
#include "dev/ca8210/cascoda/include/cascoda_debug.h"
#include "dev/ca8210/cascoda/include/cascoda_linked_list.h"

typedef struct {
	u32_t deadline;
	u8_t group;
	u8_t eventId;
	TIMER_EventHandler* handler;
	TIMER_TimeoutHandler* timeoutHandler;
	bool markedForDeath;
} Timer;

static LLIST_Node_t* deadlines = NULLP; //head of deadlines stack (value is u32_t)
static LLIST_Node_t* timers = NULLP; //head of timers list (value is Timer struct)
static u8_t numGroups = 0; //next group number that will be assigned by TIMER_createGroup
static u8_t* groupMap = NULLP; //mapping from group num to count of timers running for that group
static u8_t groupMapArraySize = GROUPMAP_START_SIZE/2; //current size of groupMap array

//LLIST callbacks
static LLIST_Compare_func compareTimers;
static LLIST_Iter_func findTimeout;
static LLIST_Iter_func findTriggered;
static LLIST_Iter_func findCount;
static LLIST_Iter_func abortTimer;

//used to pass values to LLIST callbacks. If only C had lambdas :(
static u8_t g_group;
static u8_t g_eventId;
static u8_t g_groupRemaining;
static u32_t g_currentTime;

//helper functions
void await(
	u8_t group,
	u8_t eventId,
	u32_t deadline,
	TIMER_EventHandler* handler,
	TIMER_TimeoutHandler* timeoutHandler
);
static void runHandler(LLIST_Node_t* node, bool isTimeout, const void* params);
static inline u32_t getDeadline( u32_t duration_ms );
static void outOfRangeDebugMsg( u8_t group );
static void dpTimer(Timer* timer);



u8_t TIMER_createGroup(void){
	if (numGroups == 0xff){
		dps("TIMER: FATAL ERROR! CANNOT CREATE NEW GROUP, ALREADY HAVE 254 OF THEM");
		return 0xff;
	}

	if (numGroups == groupMapArraySize || groupMap == NULLP){
		groupMapArraySize *= 2;
		groupMap = A_Realloc(groupMap, sizeof(u8_t) * groupMapArraySize);
		if (TIMER_DEBUG_ENABLED){
			dps("TIMER: GroupMap array resized to 0x"); dp8(groupMapArraySize); dps(" bytes\n");
		}
	}

	if (TIMER_DEBUG_ENABLED){
		dps("TIMER: Group 0x"); dp8(numGroups); dps(" created\n");
	}

	groupMap[numGroups] = 0;
	return numGroups++; //return and then increment
}

void TIMER_await(
	u8_t group,
	u8_t eventId,
	u32_t duration_ms,
	TIMER_EventHandler* handler,
	TIMER_TimeoutHandler* timeoutHandler
	){

	u32_t deadline = getDeadline(duration_ms);

	await(group, eventId, deadline, handler, timeoutHandler);
}

void TIMER_awaitContinue(
	u8_t group,
	u8_t eventId,
	TIMER_EventHandler* handler,
	TIMER_TimeoutHandler* timeoutHandler
	){

	u32_t deadline;
	if (deadlines == NULLP){
		deadline = 0;
		if (TIMER_DEBUG_ENABLED) dps("TIMER: warning! Called awaitContinue outside of an event handler\n");
	} else {
		deadline = *((u32_t*)deadlines->value);
		if (TIMER_DEBUG_ENABLED){
			dps("Continuing await with deadline");
			dp32(deadline);
			dpFlush();
		}
	}

	await(group, eventId, deadline, handler, timeoutHandler);
}

void await(
	u8_t group,
	u8_t eventId,
	u32_t deadline,
	TIMER_EventHandler* handler,
	TIMER_TimeoutHandler* timeoutHandler
){
	if (group >= numGroups){
		outOfRangeDebugMsg(group);
		return;
	}
	if (eventId == 0){
		dps("TIMER: eventId 0x00 not allowed\n"); //used to signify "any event" in trigger, abort etc
		return;
	}
	if (groupMap[group] == 0xff){
		dps("TIMER: ERROR cannot create new timer. Reached limit of 0xff timers on one group\n");
		return;
	}

	//create new entry
	LLIST_Node_t *newTimer = LLIST_new(sizeof(Timer));
	if (newTimer == NULLP){
		dps("TIMER: FATAL ERROR OUT OF MEMORY\n");
		return;
	}
	((Timer *)newTimer->value)->deadline = deadline;
	((Timer *)newTimer->value)->group = group;
	((Timer *)newTimer->value)->eventId = eventId;
	((Timer *)newTimer->value)->handler = handler;
	((Timer *)newTimer->value)->timeoutHandler = timeoutHandler;
	((Timer *)newTimer->value)->markedForDeath = false;

	//insert into list
	if (timers != NULLP){
		LLIST_moveSort(newTimer, timers, compareTimers);
		//update head pointer
		if (timers->prev != NULLP){
			timers = timers->prev;
		}
	} else { //timers == nullp
		timers = newTimer;
	}

	//update groupMap
	groupMap[group] ++;

	if (TIMER_DEBUG_ENABLED){
		dps("TIMER: Awaiting "); dpTimer((Timer*)newTimer->value); dpFlush();
	}
}

/**
	@return positive if the first > the second
	@return 0 if they are equal
	@return negative if the first < the second
*/
static i8_t compareTimers(const LLIST_Node_t * first, const LLIST_Node_t * second){
	if (((Timer*)first->value)->deadline > ((Timer *)second->value)->deadline){
		return +1;
	} else if (((Timer*)first->value)->deadline < ((Timer *)second->value)->deadline){
		return -1;
	} else {
		return 0;
	}
}


bool TIMER_trigger(u8_t group, u8_t eventId, const void *params){
	if (group >= numGroups){
		outOfRangeDebugMsg(group);
		return false;
	}

	if (groupMap[group] > 0){
		g_group = group;
		g_eventId = eventId;
		LLIST_Node_t* node = LLIST_find(timers, findTriggered);
		if (node != NULLP){
			if (TIMER_DEBUG_ENABLED){
				dps("TIMER: Triggered "); dpTimer((Timer*)node->value); dpFlush();
			}
			runHandler(node, false, params);
			return true;
		}
	}

	if (TIMER_DEBUG_ENABLED){
		dps("TIMER: Cant trigger evnt0x"); dp8(eventId); dps(" grp0x"); dp8(group); dpFlush();
	}
	return false;
}

static i8_t findTriggered(LLIST_Node_t* node){
	Timer* timer = (Timer*)node->value;
	if (timer->markedForDeath){
		return 0;
	}

	if (timer->group == g_group && (g_eventId == 0 || g_eventId == timer->eventId)){
		return 1;
	} else {
		return 0;
	}
}


u8_t TIMER_monitor( u8_t group ){
	g_currentTime = BSP_ReadAbsoluteTime();
	g_group = group;

	//run timeoutHandlers for all functions past their deadline
	u8_t num = LLIST_foreach(timers, findTimeout);

	//delete all timers that are marked for death
	//could include triggered timers, not just timeouted ones. So must traverse whole list
	LLIST_Node_t* node = timers;
	while (node != NULLP){
		LLIST_Node_t* next = node->next;
		if (((Timer*)node->value)->markedForDeath){
			if (TIMER_DEBUG_ENABLED){
				dps("TIMER: Deleting "); dpTimer((Timer*)node->value); dpFlush();
			}
			LLIST_delete(node);
			if (node == timers) {
				timers = next;
			}
		}
		node = next;
	}

	return num;
}

static i8_t findTimeout(LLIST_Node_t* node){
	Timer* timer = (Timer*)node->value;
	if (timer->markedForDeath){
		return 0;
	}
	if (g_group != 0xff && timer->group != g_group){
		return 0;
	}
	if ( timer->deadline <= g_currentTime ){
		if (TIMER_DEBUG_ENABLED){
			dps("TIMER: Timeout  "); dpTimer(timer); dpFlush();
		}
		runHandler(node, true, NULLP);
		return 1;
	} else {
		return -1; //abort iteration
	}
}




u8_t TIMER_waitingFor(u8_t group, u8_t eventId){
	if (group >= numGroups){
		outOfRangeDebugMsg(group);
		return 0;
	}

	u8_t result = 0;

	if (eventId == 0){
		result = groupMap[group];
	} else {
		g_groupRemaining = groupMap[group];
		g_group = group;
		g_eventId = eventId;
		result = LLIST_foreach(timers, findCount);
	}

	/*
	if (TIMER_DEBUG_ENABLED){
		dps("TIMER: Already waiting evnt0x"); dp8(result); dps(" grp0x"); dp8(group); dpFlush();
	}
	*/

	return result;
}

static i8_t findCount(LLIST_Node_t* node){
	if (g_groupRemaining == 0) {
		return -1;
	}
	Timer* timer = (Timer*)node->value;
	if (timer->markedForDeath){
		return 0;
	}
	if (timer->group == g_group){
		g_groupRemaining --;
		if (((Timer*)node->value)->eventId == g_eventId){
			return 1;
		}
	}
	return 0;
}



u8_t TIMER_abort(u8_t group, u8_t eventId){
	if (group >= numGroups){
		outOfRangeDebugMsg(group);
		return 0;
	}

	if (TIMER_DEBUG_ENABLED){
	//	dps("TIMER: Abort evnt0x"); dp8(eventId); dps(" grp0x"); dp8(group); dpFlush();
	}

	g_groupRemaining = groupMap[group];
	g_group = group;
	g_eventId = eventId;
	return LLIST_foreach(timers, abortTimer);
}

static i8_t abortTimer(LLIST_Node_t* node){
	if (g_groupRemaining == 0) {
		return -1;
	}
	Timer* timer = (Timer*)node->value;
	if (timer->markedForDeath){
		return 0;
	}
	if (timer->group == g_group){
		g_groupRemaining --;
		if (g_eventId == 0 || timer->eventId == g_eventId){
			groupMap[g_group] --;
			timer->markedForDeath = true;
			if (TIMER_DEBUG_ENABLED){
				dps("TIMER: aborting "); dpTimer(timer); dpFlush();
			}
			return 1;
		}
	}
	return 0;
}


/** Run a trigger or timeout handler of a timer
	Note that this may happen within the iteration of timer_Monitor, nested within any timer's
	trigger or timeout function. So we can't delete timers here.
*/
static void runHandler(LLIST_Node_t* node, bool isTimeout, const void* params){
	Timer* timer = (Timer*)node->value;

	//decrement groupMap counter
	groupMap[timer->group] --;

	//mark timer as already handled (exclude from future list iters)
	timer->markedForDeath = true;


	//if its not a timeout and the handler is null, we don't have any handler to run
	if (isTimeout || timer->handler != NULLP){

		//push deadline onto top of deadlines list, for awaitContinue
		LLIST_Node_t* newDeadline = LLIST_new(sizeof(u32_t));
		LLIST_moveBefore(newDeadline, deadlines);
		deadlines = newDeadline;
		*(u32_t*)newDeadline->value = timer->deadline;

		//run handler
		if (isTimeout){
			timer->timeoutHandler();
		} else {
			timer->handler(params);
		}

		//pop off deadlines stack
		deadlines = deadlines->next;
		LLIST_delete(newDeadline);
	}
	//don't delete the timer yet; we might be inside TIMER_Monitor()
}

/**
	@param duration_ms the time in milliseconds between now and the deadline
	@return the absolute time of the deadline in milliseconds
*/
static inline u32_t getDeadline(u32_t duration_ms){
	u32_t result = BSP_ReadAbsoluteTime() + duration_ms;
	if (result < duration_ms){ //overflow check
		result = 0xffffffff;
	}
	return result;
}

static void outOfRangeDebugMsg( u8_t group ){
	if (TIMER_DEBUG_ENABLED){
		dps("TIMER: FATAL ERROR! group Id 0x");
		dp8(group);
		dps(" is out of range (less than numGroups)\n");
	}
}

static void dpTimer(Timer* timer){
	dps("evnt0x ");
	dp8(timer->eventId);
	dps(" grp0x");
	dp8(timer->group);
	dps(" dead0x");
	dp32(timer->deadline);
}