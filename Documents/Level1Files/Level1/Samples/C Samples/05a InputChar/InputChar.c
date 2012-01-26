/*	InputChar.c
*
*	This code demonstrates simple serial input, waiting for a single
*	character and echoing it back.
*
*	The code uses two function defined in Util.c/h:
*		IsCharWaiting() -- returns true if there is unread data
*			in the receive buffer
*		InKey() -- returns the (next) character in the receive buffer
*			(0 if none)
*
*	Connect the serial cable (from the kit) from JP5 on the ARC board
*	to your computer's serial port. Use a terminal program at 19.2k baud,
*	with 8 data bits, no parity, and 1 stop bit.
*
*	All the communication with the microprocessor is handled by the
*	utility functions listed above. Understanding them is outside the
*	scope of the Level 1.2 Workshop.
*/

#include <avr/io.h>
#include <stdio.h>
#include "Util.h"

/* define all the stuff doing I/O here; there are two reasons for this:
   1) it makes the code easier to read/understand
   2) it makes it easy to make changes later, e.g. using a different port */
/* green LED is connected to PB4 */
#define SetupLed() SetBit(DDRB, DDB4)	// set direction for B4 as output
#define LedOff()   SetBit(PORTB, PB4)	// set B4 to high to turn off LED
#define LedOn()    ClearBit(PORTB, PB4)	// set B4 to low to turn on LED
#define ToggleLed() (PORTB ^= (1 << PB4)) // XOR with 1 will toggle value (0 leaves it alone)

int main()
{
	char ch;
	char sz[32];

	SetupLed();
	InitComm();		// initialize serial communications

	Print("Type a character...");
	while (!IsCharWaiting())	// see if there's something there
	{
		WaitMs(200);
		ToggleLed();
	}
	LedOff();
	ch = InKey();				// read the character from input

	sprintf(sz, "     You typed '%c'\r\n", ch);
	Print(sz);
	Print("bye!\r\n");

	return 0;
}
