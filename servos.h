/*
	Authors: Davide Berdin, Till Riemer
*/
#ifndef _INIT
#define _INIT

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
/* set direction and wave output mode */
#define LFwd()		( ClearBit(PORTB, PB6),   SetBit(TCCR1A, COM1B1), ClearBit(TCCR1A, COM1B0) ) // clear on compare match - output to low
#define LRev()		(   SetBit(PORTB, PB6),   SetBit(TCCR1A, COM1B1),   SetBit(TCCR1A, COM1B0) ) // set -> high
#define LStop()		( ClearBit(PORTB, PB6), ClearBit(TCCR1A, COM1B1), ClearBit(TCCR1A, COM1B0) )
#define RFwd()		( ClearBit(PORTB, PB5),   SetBit(TCCR1A, COM1A1), ClearBit(TCCR1A, COM1A0) )
#define RRev()		(   SetBit(PORTB, PB5),   SetBit(TCCR1A, COM1A1),   SetBit(TCCR1A, COM1A0) )
#define RStop()		( ClearBit(PORTB, PB5), ClearBit(TCCR1A, COM1A1), ClearBit(TCCR1A, COM1A0) )

void motorInit();
void SetLeftMotorPWM(int pwm);
void SetRightMotorPWM(int pwm);
void setMotorSpeed(int, int);

#endif
