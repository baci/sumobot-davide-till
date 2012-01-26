/*	ADCPrint.c
*
*	This code demonstrates how to set up the Analog-to-Digital
*	Converter (ADC) and read analog input.
*
*	Ports PA0-PA7 each have the option of being used for analog input,
*	using the names ADC0, ADC1, ... ADC7.
*
*	We have line sensors hooked up as follows:
*		PA1/ADC1 = left line sensor
*		PA3/ADC3 = right line sensor
*
*	The code uses two functions defined in Util.c/h:
*		InitADC() -- this sets up the Analog-to-Digital Converter (ADC).
*		GetADC(adc) -- this reads the specified ADC channel (0-7) and
*			returns its value (0-255).
*/

#include <avr/io.h>
#include <stdio.h>
#include "Util.h"

/* define all the stuff doing I/O here; there are two reasons for this:
   1) it makes the code easier to read/understand
   2) it makes it easy to make changes later, e.g. using a different port */
#define SetupLeftLineSensor()	ClearBit(DDRA, DDA1)	// set as input
#define leftLineSensor			1

int main()
{
	char sz[32];

	InitComm();
	InitADC();
	SetupLeftLineSensor();

	/* keep checking the line sensor value, outputting its value
	   approximately 5 times per second */
	while (1)
	{
		sprintf(sz, "ADC left line = %d\r\n", GetADC(leftLineSensor));
		Print(sz);
		WaitMs(200);
	}

	return 0;
}
