/*	PWM.c
*
*	This code demonstrates controlling the motors!
*
*	The code uses three functions defined in Util.c/h:
*		InitPWM() -- this sets up the microprocessor for PWM control
*			of the motors.
*		SetLeftMotorPWM(pwm) and SetRightMotorPWM(pwm) -- these functions
*			control the speed of each motor using the specified "pwm" value.
*				Full-speed reverse is -255
*				Stop is 0
*				Full-speed forward is 255
*			Intermediate values will run the motors proportionally slower,
*			for example 128 is roughly half-speed forward.
*
*	All the actual communication between the microprocessor and motors
*	is handled by the utility functions listed above. Understanding them
*	is outside the scope of the Level 1.2 Workshop.
*/

#include <avr/io.h>
#include "Util.h"

int main()
{
	InitPWM();

	WaitMs(3000);	// wait 3 seconds before it takes off :-)

	/* turn motors on forward, at half speed */
	SetLeftMotorPWM(128);
	SetRightMotorPWM(128);
	/* let the motors run for 3 seconds */
	WaitMs(3000);
	/* have the motors stop */
	SetLeftMotorPWM(0);
	SetRightMotorPWM(0);

	return 0;
}
