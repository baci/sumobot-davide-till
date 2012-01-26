/*	Timer.c
*
*	This code demonstrates how to use Counter/Timer 0 for
*	accurate measurement of time.
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
#define ToggleLed() (PORTB ^= (1 << PB4)) // XOR with 1 will toggle value

/* line sensors are connected to ADC1 and ADC3 */
#define SetupLineSensors()	(DDRA &= ~0x0A)	// clear bits DDA1/3 to set as input
#define leftLineSensor		1
#define rightLineSensor		3

#define msBlink		500		// number of ms between toggling LED

int main()
{
	unsigned long msStart;
	unsigned char adcL, adcR;

	SetupLed();
	LedOff();
	InitADC();
	SetupLineSensors();
	InitTimer();

	msStart = TimerMsCur();
	while (1)
	{
		adcL = GetADC(leftLineSensor);
		adcR = GetADC(rightLineSensor);
		
		/* do some interesting stuff here! */
		
		if (TimerCheck(msStart, msBlink))
		{
			/* remember current time for next toggle */
			msStart = TimerMsCur();
			ToggleLed();
		}
	}

	return 0;
}
