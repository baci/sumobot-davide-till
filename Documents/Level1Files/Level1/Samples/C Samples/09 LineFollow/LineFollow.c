/*	LineFollow.c
*
*	This code demonstrates a good line-following algorithm.
*
*	It steers in proportion to the difference between
*	two line sensors.
*
*	For details on the algorithm, see:
*	http://www.barello.net/Papers/LineFollowing/index.htm
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

#define msLoop		5		// to control loop speed
#define msWaitStart	3000	// number of milliseconds to wait before start
#define sumOffLine	100		// off the line if sensor sum is below this
#define pwmDrive	128		// PWM speed for driving

void WaitForGo();

/* create a global buffer to use for snprintf */
char g_szMsgBuffer[32];

int main()
{
	unsigned long msStart;
	unsigned char adcL, adcR;
	int sensorDifference;
	int sensorSum;

	InitTimer();
	InitComm();
	InitADC();
	InitPWM();
	SetupLed();
	SetupLineSensors();

	/* do any start-up UI (wait a few seconds and/or
	   wait for button press) */
	WaitForGo();

	/* follow the line */
	while(1)
	{
		msStart = TimerMsCur();

		/* read line sensor values */
		adcL = GetADC(leftLineSensor);
		adcR = GetADC(rightLineSensor);

		/* calculate sum and difference */
		sensorDifference = (adcL - adcR) / 4;
		sensorSum = adcL + adcR;

		/* are we off the line? */
		if (sensorSum < sumOffLine)
			break;

		/* steer in proportion to the difference between the sensors */
		SetLeftMotorPWM(pwmDrive - sensorDifference);
		SetRightMotorPWM(pwmDrive + sensorDifference);

		while (!TimerCheck(msStart, msLoop))
			;
	}

	SetLeftMotorPWM(0);
	SetRightMotorPWM(0);
	Print("done\r\n");

	return 0;
}

/* flash LED for a few seconds before taking off */
void WaitForGo()
{
	unsigned long msStart;
	unsigned int msElapsed, msPreviousWrite;

	/* flash LED while waiting for a few seconds */
	msPreviousWrite = 0;
	msStart = TimerMsCur();
	while ((msElapsed = TimerMsCur() - msStart) < msWaitStart)
	{
		/* flash LED while waiting */
		if ((msElapsed % 1000) < 800)
			LedOff();
		else
			LedOn();

		/* dump out the sensor values every quarter second */
		if (msElapsed - msPreviousWrite > 250)
		{
			msPreviousWrite = msElapsed;
			snprintf(g_szMsgBuffer, sizeof(g_szMsgBuffer),
				"%3d %3d\r\n",
				GetADC(leftLineSensor),
				GetADC(rightLineSensor));
			Print(g_szMsgBuffer);
		}
	}	
}
