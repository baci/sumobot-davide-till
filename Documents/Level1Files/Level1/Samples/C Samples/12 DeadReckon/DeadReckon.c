/*	DeadReckon.c
*
*	This code demonstrates how to use a table-driven state machine
*	to control a robot's motion.
*
*	It also shows how to store constant data in Flash (instead of
*	the default RAM) and how to retrieve the data when it's needed.
*	The relevant code for that uses PROGMEM and memcpy_P.
*/

#include <stdio.h>
#include <avr/pgmspace.h>
#include "Util.h"

/* define all the stuff doing I/O here; there are two reasons for this:
   1) it makes the code easier to read/understand
   2) it makes it easy to make changes later, e.g. using a different port */

/* green LED is connected to PB4 */
#define SetupLed() SetBit(DDRB, DDB4)	// set direction for B4 as output
#define LedOff()   SetBit(PORTB, PB4)	// set B4 to high to turn off LED
#define LedOn()    ClearBit(PORTB, PB4)	// set B4 to low to turn on LED
#define ToggleLed() (PORTB ^= (1 << PB4)) // XOR with 1 will toggle value

typedef struct
{
	int pwmL, pwmR;		// speeds for motors
	unsigned int ms;	// milliseconds for this state
} STATE;

/* store table in program memory (flash);
   this means we don't have to devote SRAM to storing
   the entire table, but it adds an extra step for
   retrieving the info when we change states */
STATE s_table[] PROGMEM =
{
  /* left  right  ms */
	{   0,    0, 3000}, // wait 3 seconds before start
	{ 128,  128, 2000}, // go forward for 2 seconds
	{   0,  128,  800}, // pivot on left wheel
	{ 255,  255, 1000}, // go fast
	{   0,    0, 1000}, // pause
	{ 128, -128,  600}, // spin in place
	{-128, -128,  800}, // back up
	{   0,    0,    1}, // stop
};
#define numStates (sizeof(s_table) / sizeof(s_table[0]))

void SetState(int stateCur, STATE *pstateInfo, unsigned long *pms);

int main()
{
	int stateCur;
	STATE stateInfo;
	unsigned long msStart;

	InitTimer();
	InitComm();
	InitPWM();
	SetupLed();
	LedOff();

	/* start at the first state (first entry in table) */
	stateCur = 0;
	SetState(stateCur, &stateInfo, &msStart);

	while (stateCur < numStates)
	{

		/* read sensors, etc... */


		/* see if it's time to transition to the next state */
		if (TimerCheck(msStart, stateInfo.ms))
		{
			/* move to the next state */
			++stateCur;
			SetState(stateCur, &stateInfo, &msStart);
		}
	}

	/* stop the robot (in case table doesn't!) */
	SetLeftMotorPWM(0);
	SetRightMotorPWM(0);
	LedOff();

	return 0;
}

/* set up for the specified state */
void SetState(
	int stateCur,			// the state for which to get info
	STATE *pstateInfo,		// where to store the state info
	unsigned long *pms)		// millisecond count when state started
{
	if (stateCur < numStates)
	{
		/* load from program memory (Flash) into SRAM */
		memcpy_P(pstateInfo, &s_table[stateCur], sizeof(STATE));

		/* start the state */
		*pms = TimerMsCur();
		SetLeftMotorPWM(pstateInfo->pwmL);
		SetRightMotorPWM(pstateInfo->pwmR);

		ToggleLed();
	}
}
