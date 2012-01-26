/*	InputText.c
*
*	This code demonstrates serial input, looping to continue inputting
*	text until 'x' is received.
*
*	The code uses a function defined in Util.c/h:
*		InKey() -- returns the (next) character in the receive buffer
*			(0 if none)
*	This program takes advantage of the 0 return from InKey instead
*	of using IsCharWaiting() as the InputChar program does.
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

int main()
{
	char ch;
	char sz[32];

	InitComm();		// initialize serial communications

	Print("Type a character ('x' to exit)...");
	ch = InKey();	// read a character from input

	while (ch != 'x')
	{
		if (ch != 0)	// InKey will return value 0 if there's nothing to read
		{
			sprintf(sz, "\r\nYou typed '%c'   Type something else...", ch);
			Print(sz);
		}
		ch = InKey();	// read a character from input
	}
	Print("\r\nbye!\r\n");

	return 0;
}
