#ifndef TIMER
#define TIMER

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

/* Set the timer to an amount */
void initTime();

void delay(int ms);

void startTimer();
void stopTimer();

/* Returns the difference between the "start_o" timestamp
 * and the current time. Updates the parameter "start_o" to
 * the current system time automatically
 */
int timeDelta(int * start_o);

/* Returns the time elapsed since t_start
 * WITHOUT updating t_start
 */
int timeSince(int t_start);

#endif
