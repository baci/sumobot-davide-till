/*
	Authors: Davide Berdin, Till Riemer
	
	This file initializes all the registers that the robot needs for moving

*/

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void motorInit(){

	/************** MOTOR TIMERS INITIALIZATION *****************/

	DDRB |= 1<<PB6 | 1<<PB5; //for writing to OC1B,A

	//fast pwm timer 1
	//Clear OC1B,A on compare match (set output to low level)
	TCCR1A |= 1<<COM1A1 | 1<<COM1B1; // | 1<<WGM11;
	TCCR1B |= 1<<WGM13; //| 1<<WGM12;    
	ICR1 = 2999; // set top value

	//start / stop timer/counter -> set prescaler to 8 -> CS11
	TCCR1B |= 1<<CS11;

	TIMSK1|=1<<TOIE0; // Timer/counter1 overflow interrupt enable 
	SREG|=1<<SREG_I; // global enable interrupts
}
