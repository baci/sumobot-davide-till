/*	HelloWorld.c
*
*	This code demonstrates serial output.
*
*	The code uses two functions defined in Util.c/h:
*		InitComm() -- this sets up the microprocessor for serial
*			output and input.
*		Print() -- this transmits a string using the serial port.
*
*	Connect the serial cable (from the kit) from JP5 on the ARC board
*	to your computer's serial port. Use a terminal program at 19.2k baud,
*	with 8 data bits, no parity, and 1 stop bit.
*
*	You can also use sprintf to build strings to send. (include <stdio.h>)
*
*	All the communication with the microprocessor is handled by the
*	utility functions listed above. Understanding them is outside the
*	scope of the Level 1.2 Workshop.
*/

#include <avr/io.h>
#include "Util.h"

int main()
{
	InitComm();		// initialize serial communications

	Print("Hello, World!\r\n");		// transmit text via serial connection

	return 0;
}
