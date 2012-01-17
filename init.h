/*
	Authors: Davide Berdin, Till Riemer
	
	This file initializes all the registers that the robot needs for moving

*/

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

void motorInit();

void motorInit(){

	/************** MOTOR TIMERS INITIALIZATION *****************/

	DDRB |= 1<<PB8 | 1<<PB9; //for writing to OC1A,B,C

	//fast pwm timer 3
	//Clear OC3A,B on compare match (set output to low level)
	TCCR3A |= 1<<COM3A1 | 1<<COM3B1 | 1<<WGM31;
	TCCR3B |= 1<<WGM33 | 1<<WGM32;

	//fast pwm timer 1
	//Clear OC1A,B,C on compare match (set output to low level)
	TCCR1A |= 1<<COM1A1 | 1<<COM1B1 | 1<<COM1C1 | 1<<WGM11;
	TCCR1B |= 1<<WGM13 | 1<<WGM12;    
	ICR3 = 40000;
	ICR1 = 40000;

	//start / stop timer/counter
	TCCR3B |= 1<<CS31;
	TCCR1B |= 1<<CS11;

	//interrupt on overflow
	TIMSK|=1<<TOIE0;
	SREG|=1<<SREG_I;
}
