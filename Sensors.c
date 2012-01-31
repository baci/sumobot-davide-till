#include <stdlib.h>
#include <avr/io.h>
#include "Sensors.h"
#include "timer.h"

void EmitterInit()
{
	DDRD |= 1<<PD0 | 1<<PD1; // initialize output left/right
}

void DetectorInit()
{
	DDRB  &= ~(1<<PB0) & ~(1<<PB1); 	// initialize input left/right
	DDRD  &= ~(1<<PD2); 				// -//- back
	PORTB |= 1<<PB0 | 1<<PB1;
	PORTD |= 1<<PD2;
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
