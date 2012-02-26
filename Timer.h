#ifndef TIMER_H_GUARD
#define TIMER_H_GUARD

#define TIME_SECOND 100

void TIMER_initTimer(unsigned int timerNo, unsigned int timerMax);

bool TIMER_checkTimer(unsigned int timerNo);
unsigned int TIMER_ticksLeft(unsigned int timerNo);
void TIMER_tickTimers(void);
void TIMER_disableTimer(unsigned int timerNo);
bool TIMER_isEnabled(unsigned int timerNo);
#endif