/******************************************************************************
*
* File Name          :  Timer.c
* Description        :  Basic timer functionality.
*
*******************************************************************************/

#include "circle_api.h"

#define TIMER_NOTIMERS 10 + 1 // Add 1!

static unsigned int timers[TIMER_NOTIMERS]; // Array of ints to hold timer 
											// values.
static bool timerActive[TIMER_NOTIMERS]; // Is the timer enabled?
static unsigned int timerMax[TIMER_NOTIMERS]; // What is the trigger number?

__attribute__((section(".rodata"))) void TIMER_initTimer(unsigned int timerNo, unsigned int timerMaxVal) {
	timers[timerNo] = 0;
	timerActive[timerNo] = 1;
	timerMax[timerNo] = timerMaxVal;
}

__attribute__((section(".rodata"))) bool TIMER_checkTimer(unsigned int timerNo) {
	return timers[timerNo] >= timerMax[timerNo];
}

__attribute__((section(".rodata"))) unsigned int TIMER_ticksLeft(unsigned int timerNo) {
	return TIMER_checkTimer(timerNo) ? 0 : timerMax[timerNo] - timers[timerNo];
}

__attribute__((section(".rodata"))) void TIMER_tickTimers(void) {
	int i;
	
	for(i = 0; i < TIMER_NOTIMERS; i++)
		if (timerActive[i]) timers[i]++;
}

__attribute__((section(".rodata"))) void TIMER_disableTimer(unsigned int timerNo) {
	timerActive[timerNo] = 0;
}

__attribute__((section(".rodata"))) bool TIMER_isEnabled(unsigned int timerNo) {
	return timerActive[timerNo];
}

void TIMER_drawTicker(unsigned int timerNo) {
	LCD_FillRect( 0, 5, SCREEN_WIDTH, 5, RGB_BLACK);
	LCD_FillRect( 0, 0, SCREEN_WIDTH, 4, RGB_GREEN);
	LCD_FillRect( 0, 0, SCREEN_WIDTH * ((float)timers[timerNo] / timerMax[timerNo]), 4, RGB_RED);
}
