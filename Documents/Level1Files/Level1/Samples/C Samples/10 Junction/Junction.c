/*	Junction.c
*
*	This code demonstrates how to follow a line and detect a junction.
*
*	Ports PA0-PA7 each have the option of being used for analog input,
*	using the names ADC0, ADC1, ... ADC7.
*
*	We have line sensors hooked up as follows:
*		PA1/ADC1 = left line sensor
*		PA3/ADC3 = right line sensor
*
*	The code uses some functions defined in Util.c/h:
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

/* green LED is connected to PB4 */
#define SetupLed() SetBit(DDRB, DDB4)	// set direction for B4 as output
#define LedOff()   SetBit(PORTB, PB4)	// set B4 to high to turn off LED
#define LedOn()    ClearBit(PORTB, PB4)	// set B4 to low to turn on LED
#define ToggleLed() (PORTB ^= (1 << PB4)) // XOR with 1 will toggle value

/* line sensors are connected to ADC1 and ADC3 */
#define SetupLineSensors()	(DDRA &= ~0x0A)	// clear bits DDA1/3 to set as input
#define leftLineSensor		1
#define rightLineSensor		3

/* bumpers are connected to PA0, PA7 */
/* set direction for A0/A7 as input (clear bits in DDRA)
   and enable pull-up resistors (set bits in PORTA) */
#define SetupBumpers()	( DDRA &= ~(0x81), PORTA |= 0x81 )
/* bumper is touching when input is low */
#define LeftBumperTouch()	bit_is_clear(PINA, PINA0)
#define RightBumperTouch()	bit_is_clear(PINA, PINA7)

/* these are the parameters for the line following code, adjust to taste */
#define sumOffLine        125 // if sum is less than this, we're off the line
#define sumStartJunction  340 // if sum is great than this, junction has started
#define sumLRJunction     415 // if the sum gets this large, junction has L&R branches
/* helper macros for line following code */
#define LostLine(sum)		((sum) < sumOffLine)
#define FoundJunction(sum)	((sum) > sumStartJunction)
#define LRJunction(sum)		((sum) > sumLRJunction)

#define pwmDrive          128 // PWM speed for driving

#define msJunctionLimit   150

/* line following constants */
enum
{
	stateFollowLine,
	stateDoJunction
};
enum
{
	junctionNone     = 0,
	junctionLeft     = 0x01,
	junctionRight    = 0x02,
	junctionForward  = 0x04,
	junctionDeadEnd  = 0x08,
};

/* line following functions */
void SetLineState(char state);
int JunctionCheck(unsigned char adcL, unsigned char adcR);

/* pause before we start driving */
void WaitForGo();

/* create a global buffer to use for snprintf */
char g_szMsgBuffer[50];

int main()
{
	unsigned long msStart;
	unsigned char adcL, adcR;
	int junctionType;
	
	InitTimer();
	InitComm();
	InitADC();
	InitPWM();
	SetupLed();
	SetupLineSensors();
	SetupBumpers();
		
	/* do any start-up UI (wait five second and/or wait for button press */
	WaitForGo();
	
	/* initialize the line-following state machine */
	SetLineState(stateFollowLine);
	
	/* loop until we find a junction */
	do
	{
		msStart = TimerMsCur();

		/* read line sensor values */
		adcL = GetADC(leftLineSensor);
		adcR = GetADC(rightLineSensor);

		/* advance on line */
		junctionType = JunctionCheck(adcL, adcR);

		/* wait a bit before doing the next update */
		while (!TimerCheck(msStart, 5))
			;
	} while (junctionType == junctionNone);

	/* stop the robot */
	SetLeftMotorPWM(0);
	SetRightMotorPWM(0);

	/* report the junction type found */
	Print("Junction: ");
	if (junctionType & junctionDeadEnd)
		Print("X");
	if (junctionType & junctionLeft)
		Print("L");
	if (junctionType & junctionRight)
		Print("R");
	if (junctionType & junctionForward)
		Print("F");
	Print("\r\n");

	return 0;
}

/* wait for the left bumper touch before starting;
   turn on the LED when the sensors are balanced */
void WaitForGo()
{
	unsigned long msStart;
	unsigned int msElapsed, msPreviousWrite;
	unsigned char adcL, adcR;
	int difference;

	Print("ready\r\n");

	msPreviousWrite = 0;
	msStart = TimerMsCur();
	/* flash LED while waiting for five seconds */
	while (!LeftBumperTouch())
	{
		adcL = GetADC(leftLineSensor);
		adcR = GetADC(rightLineSensor);
		difference = adcL - adcR;
		msElapsed = TimerMsCur() - msStart;

		if (difference < 0)
			difference = -difference;
		if (100 < adcL && adcL < 200
			&& 100 < adcR && adcR < 200
			&& difference < 50)
		{
			LedOn();
		}
		else
		{
			LedOff();
		}
		
		/* dump out the sensor values every quarter second */
		if ((msElapsed - msPreviousWrite) > 250)
		{
			msPreviousWrite = msElapsed;
			snprintf(g_szMsgBuffer, sizeof(g_szMsgBuffer),
				"%3d %3d\r\n", adcL, adcR);
			Print(g_szMsgBuffer);
		}
	}
	
	/* wait for button release */
	while (LeftBumperTouch())
		;

	Print("go\r\n");
}

/*
*	State Machine for Junction Detection
*/

/* variables shared among the line following functions */
static char s_stateCur;
static unsigned long s_msStartState;

/* set the state for the line following state machine */
void SetLineState(char state)
{
	s_stateCur = state;
	s_msStartState = TimerMsCur();
}

/* do the next step in following the line, report if a junction was found */
int JunctionCheck(unsigned char adcL, unsigned char adcR)
{
	int sensorDifference;
	int sensorSum;
	unsigned long msInState;
	int junction;
	
	/* this persistent variables remember stats within junction */
	static int s_sumMax;
	static unsigned char s_leftAtMax, s_rightAtMax;
	
	/* calculate sum and difference */
	sensorDifference = (adcL - adcR) / 4;
	sensorSum = adcL + adcR;
	
	/* how long have we been in this state? */
	msInState = TimerMsCur() - s_msStartState;
	
	/* default to no junction */
	junction = junctionNone;
	
	/* the state machine */
	switch (s_stateCur)
	{

	/* we're just following the line, until it ends or reaches a junction */
	case stateFollowLine:

		/* are we off the line? */
		if (LostLine(sensorSum))
		{
			junction = junctionDeadEnd;
		}
		
		/* have we found a junction? */
		if (FoundJunction(sensorSum))
		{
			s_sumMax = s_leftAtMax = s_rightAtMax = 0;
			SetLineState(stateDoJunction);
		}
		break;
	
	/* we are watching values as we cross a junction */
	case stateDoJunction:
	
		/* have we hit a new max sum value? */
		if (sensorSum > s_sumMax)
		{
			s_sumMax = sensorSum;
			s_leftAtMax = adcL;
			s_rightAtMax = adcR;
		}
		
		/* have we crossed the end of the junction? */
		if (LostLine(sensorSum) || msInState > msJunctionLimit)
		{
			junction = 0;
			
			if (LRJunction(s_sumMax))
			{
				junction |= junctionLeft | junctionRight;
			}
			else
			{
				junction |= (s_leftAtMax > s_rightAtMax)
					? junctionLeft : junctionRight;
			}
			if (!LostLine(sensorSum))
			{
				junction |= junctionForward;
			}
		}
		break;
	}
		
	/* steer in proportion to the difference between the sensors */
	SetLeftMotorPWM(pwmDrive - sensorDifference);
	SetRightMotorPWM(pwmDrive + sensorDifference);
	
	/* debug message, #ifdef or comment out when not needed */
	if (junction != junctionNone)
	{
		SetLeftMotorPWM(0);
		SetRightMotorPWM(0);
		
		while (!LeftBumperTouch())
			;

		snprintf(g_szMsgBuffer, sizeof(g_szMsgBuffer),
			"%d, L = %d, R = %d\r\n", s_stateCur, adcL, adcR);
		Print(g_szMsgBuffer);
		
		TimerWait(5);
		
		if (s_stateCur == stateDoJunction)
		{
			snprintf(g_szMsgBuffer, sizeof(g_szMsgBuffer),
				"maxSum = %d, Lm = %d, Rm = %d\r\n",
				s_sumMax, s_leftAtMax, s_rightAtMax);
			Print(g_szMsgBuffer);
		}

		TimerWait(3);
	}

	return junction;
}

