#ifndef TIMER_H_GUARD
#define TIMER_H_GUARD
void TIMER_initTimer(unsigned int timerNo, unsigned int timerMax);
bool TIMER_checkTimer(unsigned int timerNo);
void TIMER_tickTimers(void);
void TIMER_disableTimer(unsigned int timerNo);
bool TIMER_isEnabled(unsigned int timerNo);
#endif