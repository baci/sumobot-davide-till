#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "IRsensor.h"
#include "sumobot.h"

void IRdetectorInit(){
	//enable global interrupt
	SREG |= 1 << SREG_I;	
	
	//Timer 0, WGM:CTC, Prescaler 8
	TCCR0A |= 1 << WGM01 | 1 << COM0A0 | 1 << CS01;
	TCCR0A &= ~(1 << COM0A1);
	TCCR0A &= ~(1 << WGM00);
	TCCR0A &= ~(1 << CS02);
	TCCR0A &= ~(1 << CS00);
	
	// Timer 0 interrup overflow enable
	TIMSK0 |= 1 << TOIE0;
	
	// Outputs of the timer
	DDRB |= 1 << DD4; 	//OC01 Data direction registr - to check!
	PORTB &= ~(1 << DD4); //OC0A

	//Timer 2, WGM:CTC, Prescaler 8
	TCCR2A |= 1 << WGM21 | 1 << COM2A0 | 1 << CS21;
	TCCR2A &= ~(1 << COM2A1);
	TCCR2A &= ~(1 << WGM20);
	TCCR2A &= ~(1 << CS22);
	TCCR2A &= ~(1 << CS20);

	// Timer 2 interrup overflow enable
	TIMSK2 |= 1 << TOIE2;

	// Outputs of the timer
	DDRB |= 1 << DD7; 	//OC21 Data direction registr - to check!
	PORTB &= ~(1 << DD7); //OC2A
	
	// TOP Left
	OCR0A = 12;
	// TOP Right
	OCR2A = 12;
}
