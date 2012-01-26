/*	BumpNGo.c
*
*	This code is an example of a solution to the challenge:
*		- wait for a bumper touch before starting the motors
*			(flashing the LED during this period)
*		- move forward
*		- when a bumper is touched:
*			- back up
*			- turn away from the obstacle
*			- continue exploring
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
#define ToggleLed() (PORTB ^= (1 << PB4)) // XOR with 1 will toggle value

/* bumpers are connected to PA0, PA7 */
/* set direction for A0/A7 as input (clear bits in DDRA)
   and enable pull-up resistors (set bits in PORTA) */
#define SetupBumpers()	( DDRA &= ~(0x81), PORTA |= 0x81 )
/* bumper is touching when input is low */
#define LeftBumperTouch()	bit_is_clear(PINA, PINA0)
#define RightBumperTouch()	bit_is_clear(PINA, PINA7)

/* states -- "f" prefix is a flag, multiple can be set at one time */
#define stateDrive			0x00	// normal exploring
#define fstateBackup		0x01	// backing up from bump
#define fstateAvoidLeft		0x02	// turning away from obstacle on left
#define fstateAvoidRight	0x04	// turning away from obstacle on right

int main()
{
	char state, timeout;

	SetupLed();
	SetupBumpers();
	InitPWM();

	/* wait for a bump to start things off */
	while (!LeftBumperTouch() && !RightBumperTouch())
	{
		WaitMs(200);
		ToggleLed();
	}
	LedOff();
	WaitMs(500);	// allow time for bumper release!

	state = stateDrive;
	timeout = 0;

	while (1)
	{
		/*
		*	check for transition to another state
		*	(starting avoidance)
		*/

		/* if bumper touched, set flags in state for backup and turn;
		   note: a touch on the "left" bumper indicates an obstacle
		   on the right since the bumpers cross to the other side! */
		if (LeftBumperTouch())
		{
			state |= fstateBackup | fstateAvoidRight;
			timeout = 3;	// backup for 3 cycles
			LedOn();
		}
		if (RightBumperTouch())
		{
			state |= fstateBackup | fstateAvoidLeft;
			timeout = 3;	// backup for 3 cycles
			LedOn();
		}

		/*
		*	handle current state
		*/

		if (state == stateDrive)
		{
			/* normal exploring */
			SetLeftMotorPWM(128);
			SetRightMotorPWM(128);
		}
		else
		{
			/* avoidance -- there are 2 possibilities,
			   backing away and (then) turning */
			if (state & fstateBackup)
			{
				/* we're in initial stage of obstacle avoidance */
				SetLeftMotorPWM(-128);
				SetRightMotorPWM(-128);
			}
			else if (state & fstateAvoidLeft)
			{
				/* we've backed away from object; it was either
				   on the left or directly in front (we get here if
				   both bumpers registered); turn right */
				SetLeftMotorPWM(128);
				SetRightMotorPWM(-128);
			}
			else
			{
				/* we've backed away from object on the right;
				   turn left to avoid it */
				SetLeftMotorPWM(-128);
				SetRightMotorPWM(128);
			}

			/*
			*	check for transition to another state
			*	(finishing avoidance)
			*/

			/* timeout counts how many "cycles" we need to do something;
			   decrement it and then see if we're done with this stage */
			if (--timeout == 0)
			{
				/* end of this state; see what's next */
				if (state & fstateBackup)
				{
					/* done backing up; turn */
					state &= ~fstateBackup;
					timeout = 2;	// turn for 2 cycles
				}
				else
				{
					/* done turning; go back to exploring */
					state = stateDrive;
					LedOff();
				}
			}
		}

		WaitMs(200);	// perform this loop ~5 times/second
	}

	return 0;
}
