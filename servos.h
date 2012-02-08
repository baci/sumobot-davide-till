/*
	Authors: Davide Berdin, Till Riemer
*/

#include <avr/interrupt.h>

#ifndef _INIT
#define _INIT

// OCR High or low; depending on the servo
#define LEFT_HIGH		OCR1BH	
#define RIGHT_HIGH		OCR1AH
#define LEFT_LOW		OCR1BL
#define RIGHT_LOW		OCR1AL

#define TOP_VALUE		255	// or 0xFF

#define ICR_VALUE       10000
#define MIN_SPEED       500      
#define MAX_SPEED       1000            

#define NO_MOVE 		750		// no movement of a motor
#define SPEED_RANGE     500

#define DUTY_CONST		200

void motorInit();
void setServoSpeed(unsigned int, int);
void dutyCycle(unsigned int, unsigned int);
void setMotorSpeed(int, int);

#endif
