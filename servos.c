#include <avr/io.h>
#include <stdlib.h>
#include "servos.h"

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

	if(!state){
		state->leftMotor = 0;
		state->rightMotor = 0;
		state->isStopped = 1;
	}

	return *state;
}
