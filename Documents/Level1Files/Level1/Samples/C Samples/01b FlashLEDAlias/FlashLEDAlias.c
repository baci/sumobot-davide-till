/*	FlashLEDAlias.c
*
*	This code works the same way as FlashLED.c, but uses #defines
*	to make the code easier to read and update.
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

/* define all the stuff doing I/O here; there are two reasons for this:
   1) it makes the code easier to read/understand
   2) it makes it easy to make changes later, e.g. using a different port */
/* green LED is connected to PB4 */
#define SetupLed() SetBit(DDRB, DDB4)	// set direction for B4 as output
#define LedOff()   SetBit(PORTB, PB4)	// set B4 to high to turn off LED
#define LedOn()    ClearBit(PORTB, PB4)	// set B4 to low to turn on LED

int main()
{
	SetupLed();
	LedOff();

	/* flash LED */
	while (1)
	{
		LedOn();
		WaitMs(200);
		LedOff();
		WaitMs(800);
	}

	return 0;
}
