/*
	Authors: Davide Berdin, Till Riemer
*/
#ifndef _INIT
#define _INIT

#define NO_MOVE 		750		// no movement of a motor
#define SLOW_FWD		50		// minimum forward
#define FAST_FWD		250		// maximum forward
#define SLOW_BWD		-50		// min backward
#define FAST_BWD		-250	// max backward
#define LEFT 			OCR1B	// left motor
#define RIGHT			OCR1A	// right motor

/* PB6 controls left motor direction (forward/reverse) */
#define SetupLDir()	SetBit(DDRB, PB6)
/* PB5 controls right motor direction (forward/reverse) */
#define SetupRDir()	SetBit(DDRB, PB5)
/* PWM output on PD4/OC1B for left motor, PD5/OC1A for right motor; these
   pins are connected to H-bridge; we just need to send signals */
#define SetupLPWM()	SetBit(DDRB, PB6)
#define SetupRPWM()	SetBit(DDRB, PB5)
/* we compare to OCR1A/B for R/L motor speeds */
#define lPWM		OCR1B
#define rPWM		OCR1A
/* set direction (input to H-bridge) and wave output mode */
#define LFwd()		( ClearBit(PORTB, PB6),   SetBit(TCCR1A, COM1B1), ClearBit(TCCR1A, COM1B0) )
#define LRev()		(   SetBit(PORTB, PB6),   SetBit(TCCR1A, COM1B1),   SetBit(TCCR1A, COM1B0) )
#define LStop()		( ClearBit(PORTB, PB6), ClearBit(TCCR1A, COM1B1), ClearBit(TCCR1A, COM1B0) )
#define RFwd()		( ClearBit(PORTB, PB5),   SetBit(TCCR1A, COM1A1), ClearBit(TCCR1A, COM1A0) )
#define RRev()		(   SetBit(PORTB, PB5),   SetBit(TCCR1A, COM1A1),   SetBit(TCCR1A, COM1A0) )
#define RStop()		( ClearBit(PORTB, PB5), ClearBit(TCCR1A, COM1A1), ClearBit(TCCR1A, COM1A0) )


typedef struct _State{
	int leftMotor;
	int rightMotor;
	int isStopped;
} State;

State *state;

State StateInit();
void motorInit();
void SetLeftMotorPWM(int pwm);
void SetRightMotorPWM(int pwm);
void setMotorSpeed(int, int);

#endif
