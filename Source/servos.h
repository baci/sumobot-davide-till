/*
	Authors: Davide Berdin, Till Riemer
*/
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef SERVOS
#define SERVOS

// OCR High or low; depending on the servo
#define LEFT_HIGH		OCR1BH	
#define RIGHT_HIGH		OCR1AH
#define LEFT_LOW		OCR1BL
#define RIGHT_LOW		OCR1AL

#define TOPVALUE		255
#define ICRVALUE       10000
#define MINSPEED       500      
#define MAXSPEED       1000            
#define NOMOVE 		750		// no movement of a motor
#define SPEEDRANGE     500
#define DUTYCONST		200

void initServos();
void setServos(unsigned int, unsigned int);
void setMotorSpeed(int, int);

#endif
