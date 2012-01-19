#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Sensors.h"
#include "time_correct.h"
//#include "time.h"

// sensor=1: left, sensor=2: right
int readLineSensor(int _sensor)
{
	int ret = -1;
	
	int sensorIn = (_sensor==1)?LEFT_LINE_IN:RIGHT_LINE_IN;
	int sensorPower = (_sensor==1)?LEFT_LINE_POWER:RIGHT_LINE_POWER;

	DDRB |= 1<<sensorIn | 1<<sensorPower; 	// write mode
	PORTB |= 1<<sensorIn | 1<<sensorPower; 	// set both to HIGH
	
	// pause 1ms
	delay(1);

	DDRB &= ~(1<<sensorIn); 				// read mode for In
	ret = PINB & (1 << sensorIn); 			// read from sensor
	PORTB &= ~(1<<sensorPower); 			// deactivate sensor

	return ret;
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
	DDRD  &= ~(1<<PD2); 				// -//- back
	PORTB |= 1<<PB0 | 1<<PB1;
	PORTD |= 1<<PD2;
	
	return *detector;
}

