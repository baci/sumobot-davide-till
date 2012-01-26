/*	RemoteControl.c
*
*	This program will let you use the numeric keypad (8 = forward, and so forth)
*	to drive the robot in short bursts, printing (via serial output) the values
*	of the light sensors each time a character is received.
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

/* speeds for moving */
#define pwmDrive	128
#define pwmTurn		96

void ShowSensors();
void Drive(char ch);

int main()
{
	char ch;

	SetupLed();
	LedOff();
	InitPWM();
	InitComm();
	InitADC();
	SetupLineSensors();

	Print("Remote Control\r\n");

	while (1)
	{
		ch = InKey();	// read a character from input
		if (ch != 0)
		{
			Print("Before: ");
			ShowSensors();

			Drive(ch);

			Print("After:  ");
			ShowSensors();
		}

		ToggleLed();
		WaitMs(200);
		SetLeftMotorPWM(0);
		SetRightMotorPWM(0);
	}

	return 0;
}

/* print (via serial output) the values of the left and right line sensors */
void ShowSensors()
{
	char sz[32];

	sprintf(sz, "left: %3d     right: %3d\r\n",
		GetADC(leftLineSensor), GetADC(rightLineSensor));
	Print(sz);
}

/* turn on the motors to move the robot as indicated by ch */
void Drive(char ch)
{
	switch (ch)
	{
	case '7':	// fwd / left
		SetLeftMotorPWM(pwmTurn);
		SetRightMotorPWM(pwmDrive);
		break;
	case '8':	// fwd
		SetLeftMotorPWM(pwmDrive);
		SetRightMotorPWM(pwmDrive);
		break;
	case '9':	// fwd / right
		SetLeftMotorPWM(pwmDrive);
		SetRightMotorPWM(pwmTurn);
		break;

	case '4':	// left
		SetLeftMotorPWM(-pwmTurn);
		SetRightMotorPWM(pwmTurn);
		break;
	case '6':	// right
		SetLeftMotorPWM(pwmTurn);
		SetRightMotorPWM(-pwmTurn);
		break;

	case '1':	// back / left
		SetLeftMotorPWM(-pwmTurn);
		SetRightMotorPWM(-pwmDrive);
		break;
	case '2':	// back
		SetLeftMotorPWM(-pwmDrive);
		SetRightMotorPWM(-pwmDrive);
		break;
	case '3':	// back / right
		SetLeftMotorPWM(-pwmDrive);
		SetRightMotorPWM(-pwmTurn);
		break;
	}
}
