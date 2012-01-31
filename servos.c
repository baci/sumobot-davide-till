#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include "servos.h"


/* 
FastPWM timer:
2000: L rev / R fwd
1500: L stop / R stop
1000: L fwd / R rev 
*/
#define TOP 		20000
#define LEFT 		OCR1B
#define RIGHT 		OCR1A

/* motor timers initialization */
void motorInit(){

	DDRB |= 1<<PB6 | 1<<PB5; //for writing to OC1B,A
	PORTB |= ~(1 << PB5);
	PORTB |= ~(1 << PB6);

	//fast pwm, prescaler 8, toggle on compare match
	TCCR1A |= 1<<COM1A0 | 1<<COM1A1 | 1<<COM1B1 | 1<<COM1B1 | 1<<WGM11;
	TCCR1A &= ~(1<<WGM10);

	TCCR1B |= 1<<CS11 | 1<<WGM13 | 1<<WGM12;
	TCCR1B &= ~(1<<CS10);						// prescaler
	TCCR1B &= ~(1<<CS12);

	ICR1 = TOP;
	
}

void moveFwd(){
	LEFT = TOP-2000;
	RIGHT = TOP-1000;
}

void moveLeft(){
	LEFT = TOP-1000;
	RIGHT = TOP-1000;
}

void moveRight(){
	LEFT = TOP-2000;
	RIGHT = TOP-2000;
}

void moveBwd(){
	LEFT = TOP-1000;
	RIGHT = TOP-2000;
}

void stop(){
	LEFT = TOP-1500;
	RIGHT = TOP-1500;
}
