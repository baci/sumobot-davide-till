/*
	Authors: Davide Berdin, Till Riemer
	
	This file initializes all the registers that the robot needs for moving

*/

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void motorInit();
void delayInit();

struct _State{
	int leftMotor;
	int rightMotor;
	int isStopped;
};

struct _Emitter{
	int leftOut;
	int rightOut;
	int durationOut;
};

typedef struct _Detector{
	int leftIn;
	int backIn;
	int rightIn;
	int durationIn;
};

typedef struct _State State;
typedef struct _Emitter Emitter;
typedef struct _Detector Detector;

State *state;
Emitter *emitter;
Detector *detector;

State StateInit();
Emitter EmitterInit();
Detector DetectorInit();

void delayInit()
{
   OCR2A = 0x1E84;               					//Set OCR1A
                   					//Timer counter control register
   TCCR2A = (1 << CS22)|(0 << CS21)|(1 << CS20) | (1 << WGM21)|(1 << WGM20);
   		 	// WGM1=4, prescale at 1024
   TIMSK2 |= (1 << OCIE2A);         					//Set bit 6 in TIMSK to enable Timer 2 compare interrupt. 
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
	OCR1A = 750;
	OCR1B = 750;

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


Emitter EmitterInit()
{
	emitter = (Emitter *)malloc(sizeof(Emitter));
	
	if(!emitter){
		emitter->leftOut = 0;
		emitter->rightOut = 0;
		emitter->durationOut = 0;
	}

	DDRD |= 1<<PD0 | 1<<PD1; // initialize output left/right
	
	return *emitter;
}

Detector DetectorInit()
{
	detector = (Detector *)malloc(sizeof(Detector));

	if(!detector){
		detector->leftIn = 0;
		detector->backIn = 0;
		detector->rightIn = 0;
		detector->durationIn = 0;
	}
	
	DDRB  &= ~(1<<PB0) & ~(1<<PB1); 	// initialize input left/right
	DDRD  &= ~(1<<PD2); 			// -//- back
	PORTB |= 1<<PB0 | 1<<PB1;
	PORTD |= 1<<PD2;
	
	return *detector;
}
