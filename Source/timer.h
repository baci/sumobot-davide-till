/*
	Authors: Davide Berdin, Till Riemer
*/
#ifndef TIMER
#define TIMER

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


void initTimer();
void delay(int ms);
void startTimer();
void stopTimer();

#endif
