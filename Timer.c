/******************************************************************************
*
* File Name          :  Timer.c
* Description        :  Basic timer functionality.
*
*******************************************************************************/

#include "circle_api.h"

#define TIMER_NOTIMERS 10

static unsigned int timers[TIMER_NOTIMERS]; // Array of ints to hold timer values.
// Is the timer enabled?
static bool timerActive[TIMER_NOTIMERS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned int timerMax[TIMER_NOTIMERS]; // What is the trigger number?

void TIMER_initTimer(unsigned int timerNo, unsigned int timerMax) {
	timers[timerNo] = 0;
	timerActive[timerNo] = 1;
	timerMax[timerNo] = timerMax;
}

bool TIMER_checkTimer(unsigned int timerNo) {
	return timers[timerNo] >= timerMax[timerNo];
}

void TIMER_tickTimers(void) {
	int i;
	
	for(i = 0; i < TIMER_NOTIMERS; i++)
		if (timerActive[i]) timers[i]++;
}

void TIMER_diableTimer(unsigned int timerNo) {
	timerActive[timerNo] = 0;
}