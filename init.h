/*
	Authors: Davide Berdin, Till Riemer
	
	This file initializes all the registers that the robot needs for moving

*/
#ifndef _INIT
#define _INIT

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define NO_MOVE 		750		// no movement of a motor
#define SLOW_FWD		50		// minimum forward
#define FAST_FWD		250		// maximum forward
#define SLOW_BWD		-50		// min backward
#define FAST_BWD		-250	// max backward
#define LEFT 			OCR1B	// left motor
#define RIGHT			OCR1A	// right motor


struct _State{
	int leftMotor;
	int rightMotor;
	int isStopped;
};

typedef struct _State State;

State *state;

State StateInit();

void delayInit()
{
   OCR2A = 0x1E84;           //Set OCR1A
   //Timer counter control register
   TCCR2A = (1 << CS22)|(0 << CS21)|(1 << CS20) | (1 << WGM21)|(1 << WGM20);   // WGM1=4, prescale at 1024
   TIMSK2 |= (1 << OCIE2A);         //Set bit 6 in TIMSK to enable Timer 2 compare interrupt. 
}

void motorInit(){

	/************** MOTOR TIMERS INITIALIZATION *****************/

	DDRB |= 1<<PB6 | 1<<PB5; //for writing to OC1B,A

	//pwm phase and frequency correct - timer 1
	//Clear OC1B,A on compare match (set output to low level)
	TCCR1A |= 1<<COM1A1 | 1<<COM1B1;
	TCCR1B |= 1<<WGM13;
	ICR1 = 2999; // set top value

	//start / stop timer/counter -> set prescaler to 8 -> CS11
	TCCR1B |= 1<<CS11;

	TIMSK1|=1<<TOIE1; // Timer/counter1 overflow interrupt enable 

	// initial
	LEFT = NO_MOVE;
	RIGHT = NO_MOVE;

	SREG|=1<<SREG_I; // global enable interrupts

	
}

State StateInit(){
	state = (State *)malloc(sizeof(State));

	if(state == NULL){
		state->leftMotor = 0;
		state->rightMotor = 0;
		state->isStopped = 1;
	}

	return *state;
}

#endif
