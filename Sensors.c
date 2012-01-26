#include <stdlib.h>
#include <avr/io.h>
#include "Sensors.h"
#include "timer.h"

// sensor=1: left, sensor=2: right
int readLineSensor(int _sensor)
{
	int ret = -1;
	
	int sensorIn = (_sensor==1)?LEFT_LINE_IN:RIGHT_LINE_IN;
	int sensorPower = (_sensor==1)?LEFT_LINE_POWER:RIGHT_LINE_POWER;

	DDRB |= 1<<sensorIn | 1<<sensorPower; 	// write mode
	PORTB |= 1<<sensorIn | 1<<sensorPower; 	// set both to HIGH
	
	// pause 1ms
	TimerWait(1);

	DDRB &= ~(1<<sensorIn); 				// read mode for In
	ret = PINB & (1 << sensorIn); 			// read from sensor
	PORTB &= ~(1<<sensorPower); 			// deactivate sensor

	/*
	if(_sensor==1)
		return GetADC(LEFT_LINE_IN);
	else if (_sensor==2)
		return GetADC(RIGHT_LINE_IN);
	else
		return -1;
	*/

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

LineSensor LineSensorInit()
{
	DDRB &= ~0x88; // clear input bits for line sensors (PB3/7)
}

/* sets up the Analog-to-Digital Converter (ADC) */
void InitADC()
{
	/* we left-adjust so that we get the 8 most-significant bits
	   in ADCH; use 5V input to AREF for reference voltage
	   (only option since 5V is applied there);
	   divide microcontroller clock by 64 to get 128 kHz ADC clock */
	ADMUX = 0x20;	// 00 1 00000 (Aref, L-shift, [ADC0]) 						-> correct?
	ADCSRA = 0x86;	// 1000 0110 (enable, no auto trigger, no interrupt, clk/64)
}

/* reads the specified ADC channel (0-7) and returns its value (0-255) */
unsigned char GetADC(char adc)
{
	ADMUX &= ~0x1F;		// clear channel selection (low 5 bits) -> all MUX get 0
	ADMUX |= adc;		// select specified channel

	SetBit(ADCSRA, ADSC);	// ADC start conversion
		/* (this appears to clear ADIF) */

	/* wait for conversion to complete */
	while (bit_is_clear(ADCSRA, ADIF))
		;

	return (unsigned char) ADCH; // high part returned
}
