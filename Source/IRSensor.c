/*
	Authors: Davide Berdin, Till Riemer
*/
#include "IRSensor.h"

/* initialize emittors */
void initEmittors(){
	cli();	

    DDRB |= 1<<PB4; // OC0A as output for IR LEDs
	TCCR0A = 0;
    TCCR0A |= 1<<WGM01 | 1<<COM0A0; // CTC mode and set OC0A to toggle on compare match
    OCR0A = SENSITIVITY;

	sei();
}

/* stop the IR LEDs */
void stopIR(){
	TCCR0A &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); // switch Timer 0 off
}

/* start the IR LEDs */
void startIR(){
	TCNT0 = 0;
	TCCR0A |= 1<<CS00; // no prescaler
}

/* initialize detectors (just setting as input) */
void initDetectors(){
    DDR &= ~(1 << RIGHT | 1 << LEFT); 
}

/* 1, if right IR detected an object, 0 otherwise */
int objectRight(){
	int hits = 0;
	for (int i = 0; i < SAMPLES; i++){ // multiple samples for evaluation
		startIR();
		delay(1);
		stopIR();
		if(bit_is_clear(PIN, RIGHT)) hits++;
		delay(1);
	}
	return hits >= BORDERVALUE;
}

/* 1, if left IR detected an object, 0 otherwise */
int objectLeft(){
	int hits = 0;
	for (int i = 0; i < SAMPLES; i++){
		startIR();
		delay(1);
		stopIR();
		if(bit_is_clear(PIN, LEFT)) hits++;
		delay(1);
	}
	return hits >= BORDERVALUE;
}

