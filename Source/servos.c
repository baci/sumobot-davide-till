/*
	Authors: Davide Berdin, Till Riemer
*/
#include "servos.h"

void initServos(){
	// Timer 1, cycle lasts 20ms, Prescaler is 8

 	TCCR1A = 0;
 	TCCR1B = 0;
 		
	// initialize servo pins
 	DDRB |= (1<<PB5) | (1<<PB6);
 		
 	// set ICR value
 	ICR1H = ICRVALUE >> 8;
 	ICR1L = ICRVALUE & (TOPVALUE);
 		
	// initially no movement
 	setMotorSpeed(0,0);
 		
 	TCCR1A |= (1<<COM1A1);	
 	TCCR1A |= (1<<COM1B1);		
 		
 	TCCR1B |= (1<<WGM13);						
 	TCCR1B |= ((0<<CS12) | (1<<CS11) | (0<<CS10));	// set Prescaler
}

/* setting the OCR values for servos - servo=0: right, servo=1: left */
void setServos(unsigned int servo, unsigned int duty){
	if (duty > MAXSPEED) duty = MAXSPEED;
 	if (duty < MINSPEED) duty = MINSPEED;
 		
 	if(servo==0){
 			RIGHT_HIGH = duty >> 8;
 			RIGHT_LOW = duty & (TOPVALUE);
 	}
 	if(servo==1){
 			LEFT_HIGH = duty >> 8;
 			LEFT_LOW = duty & (TOPVALUE);
 	}
}

/* setting the speed of both motors (left goes to other direction, thus has to be inverted!) */
void setMotorSpeed(int leftSpeed,int rightSpeed){
	leftSpeed = -leftSpeed;

	setServos(0, NOMOVE + (((int)rightSpeed * SPEEDRANGE) / DUTYCONST));
	setServos(1, NOMOVE + (((int)leftSpeed * SPEEDRANGE) / DUTYCONST));
}




