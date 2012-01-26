#include <avr/io.h>
#include <stdlib.h>
#include "servos.h"
#include "Sensors.h"

State StateInit(){
	state = (State *)malloc(sizeof(State));

	if(!state){
		state->leftMotor = 0;
		state->rightMotor = 0;
		state->isStopped = 1;
	}

	return *state;
}

void setMotorSpeed(int left, int right)
{
	SetLeftMotorPWM(left);
	SetRightMotorPWM(right);

	// update state struct...
	state->leftMotor = left;
	state->rightMotor = right;
	if(left==0 && right==0) state->isStopped = 1;
	else state->isStopped = 0;
}

/* sets up microprocessor for PWM control of motors */
void motorInit()
{
	/* set up ports */
	SetupLDir();
	SetupRDir();
	SetupLPWM();
	SetupRPWM();

	TCNT1 = 0;

	/* see comment above for info on PWM initialization */
	/* start with motors disconnected from Timer/Counter output */
	TCCR1A = 0x01;	// 00 00 00 01
	TCCR1B = 0x0B;	// 000 01 011 (512 Hz) /64 C

	/* OCR1A/B are the values that the timer is compared to; a match will
	   cause the output to change; small values mean the motor runs for a
	   short period (slower); larger values are longer times (faster)*/
	lPWM = rPWM = 0;	// (value is irrelevant since outputs are disconnected)
}

/* pwm values can range from -255 (full-speed reverse)
   to 255 (full-speed forward), with 0 indicating a stop */
void SetLeftMotorPWM(int pwm)
{
	if (pwm == 0) LStop();
	else
	{
		if (pwm >= 0)LRev();
		else
		{
			LFwd();
			pwm = -pwm;
		}
		if (pwm > 255)pwm = 255;

		lPWM = pwm;		// set width for PWM
	}
}

/* pwm values can range from -255 (full-speed reverse)
   to 255 (full-speed forward), with 0 indicating a stop */
void SetRightMotorPWM(int pwm)
{
	if (pwm == 0)RStop();
	else
	{
		if (pwm >= 0)RFwd();
		else
		{
			RRev();
			pwm = -pwm;
		}
		if (pwm > 255)pwm = 255;

		rPWM = pwm;		// set width for PWM
	}
}
