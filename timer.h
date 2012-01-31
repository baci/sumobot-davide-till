#ifndef TIMER_H
#define TIMER_H

/* initializes Timer/Counter 0 for timing */
void InitTimer();

/* returns number of ms since Timer initialized */
unsigned long TimerMsCur();

/* returns true if the specified number of milliseconds
   has passed since the start time (else returns false) */
char TimerCheck(unsigned long msStart, unsigned int msWait);

/* pauses for the specified number of milliseconds */
void delay(unsigned int ms);

#endif
