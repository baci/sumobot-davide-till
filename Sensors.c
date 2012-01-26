#include <stdlib.h>
#include <avr/io.h>
#include "Sensors.h"
#include "timer.h"
#include "lcd_functions.h"

void readLineSensorLeft()
{
	int leftValue = -1;
	int lowLeft = 0;

	int sensorInLeft = LEFT_LINE_IN;
	int sensorPowerLeft = LEFT_LINE_POWER;

	DDRB |= 1<<sensorInLeft | 1<<sensorPowerLeft; 	// write mode
	PORTB |= 1<<sensorInLeft | 1<<sensorPowerLeft; 	// set both to HIGH
	
	// pause 1ms
	TimerWait(1);

	DDRB &= ~(1<<sensorInLeft); 				// read mode for In
	leftValue = PINB & (1 << sensorInLeft); 			// read from sensor
	PORTB &= ~(1<<sensorPowerLeft); 			// deactivate sensor

	if(leftValue == 0)
		lowLeft++;
}

int readLineSensorRight()
{
	int rightValue = -1;
	int lowRight = 0;
		
	int sensorInRight = RIGHT_LINE_IN;
	int sensorPowerRight = RIGHT_LINE_POWER;

	DDRB |= 1<<sensorInRight | 1<<sensorPowerRight; 	// write mode
	PORTB |= 1<<sensorInRight | 1<<sensorPowerRight; 	// set both to HIGH
	
	// pause 1ms
	TimerWait(1);

	DDRB &= ~(1<<sensorInRight); 				// read mode for In
	rightValue = PINB & (1 << sensorInRight); 			// read from sensor
	PORTB &= ~(1<<sensorPowerRight); 			// deactivate sensor

	if(rightValue == 0)
		lowRight++;
}

void TurnBot(int lowLeft, int lowRight){
	if(lowLeft > lowRight)
		setMotorSpeed(100, 0);
	else if(lowLeft < lowRight)
			setMotorSpeed(0, 100);
	else setMotorSpeed(100, 100);
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
	lineSensor = (LineSensor *)malloc(sizeof(LineSensor));

	if(!lineSensor){
		lineSensor->leftIn = 0;
		lineSensor->rightIn = 0;
	}
	
	DDRB &= ~0x88; // clear input bits for line sensors (PB3/7)

	return *lineSensor;
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
