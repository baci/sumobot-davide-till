#include <avr/io.h>
#include <stdlib.h>
#include "servos.h"
#include "Sensors.h"


/*
	Prescaler 8, 50Hz
	Output freq. = clock/(2*Prescaler*TOP)
	= 8000000/(2*8*10000) = 50Hz
*/


void setMotorSpeed(int left, int right)
{
        if(left>MAX) 		left = MAX;
		else if(left<MIN) 	left = MIN;
		if(right>MAX) 		right = MAX;
		else if(right<MIN) 	right = MIN;

		// left servo
		OCR1B = left;

		// right servo
        OCR1A = right;
}

/* sets up microprocessor for PWM control of motors */
void motorInit()
{
        TCCR1A = 0;
        TCCR1B = 0;

        DDRB |= (1<<PB5) | (1<<PB6);

		// with 50Hz Prescaler, cycle lasts 20ms
		ICR1 = TOP;

		setMotorSpeed(750,750);

		// set compare registers:
		// PFCM, set up, clear up
		TCCR1A |= (1<<COM1A1) | (1<<COM1B1);

		// PFCM, Top value = ICR1
		TCCR1B |= (1<<WGM13) | (0<<CS12) | (1<<CS11) | (0<<CS10);
}
