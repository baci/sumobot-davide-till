/*	FlashLED.c
*
*	This code blinks the green LED on the AVR Robot Controller (ARC) board.
*
*	The green LED is connected to PB4 on the Atmel chip, and is wired so that
*	it will be ON when the output is low and OFF when the output is high.
*
*	The code uses two macros (defined in Util.h):
*		SetBit(sfr, bit) -- This sets the given "bit" in the
*			specified "sfr" (special function register).
*		ClearBit(sfr, bit) -- This clears the given "bit" in the
*			specified "sfr" (special function register).
*	The "special function registers" used in this program are:
*		DDRB -- Port B Data Direction Register
*			A high (1) value indicates output, low (0) is input.
*			Bits are DDB0, DDB1, ... DDB7.
*		PORTB -- Port B Data Register
*			This is used to set the output value for each of the
*			eight port B pins. Bits are PB0, PB1, ... PB7.
*	WaitMs() is defined in Util.c/h.
*/

#include <avr/io.h>
#include "Util.h"

int main()
{
	SetBit(DDRB, DDB4);	// set direction for B4 (LED connection) as output
	SetBit(PORTB, PB4);	// set B4 to high (to turn off LED)

	/* loop to flash LED */
	while (1)
	{
		ClearBit(PORTB, PB4);	// set B4 to low (to turn on LED)
		WaitMs(200);		// wait 200 ms (0.2 seconds)
		SetBit(PORTB, PB4);	// set B4 to high (to turn off LED)
		WaitMs(800);		// wait 800 ms (0.8 seconds)
	}

	return 0;
}
