/*	Whisker.c
*
*	This code demonstrates how to read a switch on an input pin.
*
*	It reads the state of the left bumper (connected to PA0), lighting
*	the green LED when the bumper is pressed.
*
*	This code activates the internal "pull-up resistor" for PA0 so that
*	the bumper input will read high (1) when nothing is touching it.
*	When the whisker contacts the metal standoff, the bumper's signal
*	wire is connected to ground, resulting in a low (0) input.
*
*	See the FlashLED code for details on controlling the green LED
*	and writing to outputs.
*
*	New topics in this code:
*
*	The code uses a macro supplied by the avr-gcc compiler:
*		bit_is_clear(sfr, bit) -- this returns:
*			true if the given "bit" in the specified "sfr" is clear (0), or
*			false if the bit is set (1).
*		(There is also the opposite macro: bit_is_set(sfr, bit).)
*	The "special function registers" used for the bumpers are:
*		DDRA -- Port A Data Direction Register
*			A high (1) value indicates output, low (0) is input.
*			Bits are DDA0, DDA1, ... DDA7.
*		PORTA -- Port A Data Register
*			When a pin is configured for input, this register is used to
*			enable the pull-up resistor. Bits are PA0, PA1, ... PA7.
*		PINA -- Port A Input Pins Address
*			This is used to read the input value for each of the
*			eight port A pins. Bits are PINA0, PINA1, ... PINA7.
*/

#include <avr/io.h>
#include "Util.h"

int main()
{
	SetBit(DDRB, DDB4);		// set LED as output

	ClearBit(DDRA, DDA0);	// set direction for A0 (left bumper) as input
	SetBit(PORTA, PA0);		// enable pull-up resistor for A0

	/* keep checking the bumper state, using that to set the LED state */
	while (1)
	{
		WaitMs(10);			// loop at approximately 100Hz
		if (bit_is_clear(PINA, PINA0))	// bumper is touching when input is low
			ClearBit(PORTB, PB4);	// turn on LED
		else
			SetBit(PORTB, PB4);		// turn off LED
	}

	return 0;
}
