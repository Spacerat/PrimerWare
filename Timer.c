/******************************************************************************
*
* File Name          :  Timer.c
* Description        :  Basic timer functionality.
*
*******************************************************************************/

#include "circle_api.h"

#define TIMER_NOTIMERS 10


static unsigned int timers[TIMER_NOTIMERS]; // Array of ints to hold timer 
											// values.
static bool timerActive[TIMER_NOTIMERS]; // Is the timer enabled?
static unsigned int timerMax[TIMER_NOTIMERS]; // What is the trigger number?

void TIMER_initTimer(unsigned int timerNo, unsigned int timerMaxVal) {
	timers[timerNo] = 0;
	timerActive[timerNo] = 1;
	timerMax[timerNo] = timerMaxVal;
}

bool TIMER_checkTimer(unsigned int timerNo) {
	return timers[timerNo] >= timerMax[timerNo];
}

void TIMER_tickTimers(void) {
	int i;
	
	for(i = 0; i < TIMER_NOTIMERS; i++)
		if (timerActive[i]) timers[i]++;
}

void TIMER_disableTimer(unsigned int timerNo) {
	timerActive[timerNo] = 0;
}

bool TIMER_isEnabled(unsigned int timerNo) {
	return timerActive[timerNo];
}