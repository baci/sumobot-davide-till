#include <avr/io.h>
#include "servos.h"

void motorInit(){
	/* Timer 1 */
 	TCCR1A = 0;
 	TCCR1B = 0;
 		
 	DDRB |= (1<<PB5) | (1<<PB6);
 		
 	/* Cycle lasts 20 ms with the chosen pre-scaler according with sumobot book */
 	ICR1H = ICR_VALUE >> 8;
 	ICR1L = ICR_VALUE & (TOP_VALUE);
 		
 	/* Set compare registers */
 	setServoSpeed(0, 0);
 	setServoSpeed(1, 0);
 		
 	/* A register */
 	TCCR1A |= (1<<COM1A1);	
 	TCCR1A |= (1<<COM1B1);		
 		
 	/* B register */
 	TCCR1B |= (1<<WGM13);						
 	TCCR1B |= ((0<<CS12) | (1<<CS11) | (0<<CS10));	// Pre-scaler is 8 
}

void dutyCycle(unsigned int servo, unsigned int duty){
	if (duty > MAX_SPEED)
 		duty = MAX_SPEED;
 	if (duty < MIN_SPEED)
 		duty = MIN_SPEED;
 		
 	switch (servo){
 		/* Timer 1 */
 		case 0:
 			RIGHT_HIGH = duty >> 8;
 			RIGHT_LOW = duty & (TOP_VALUE);
 			break;
 		case 1:
 			LEFT_HIGH = duty >> 8;
 			LEFT_LOW = duty & (TOP_VALUE);
 			break;
 		}
 	return;
}

void setMotorSpeed(int left,int right){
	setServoSpeed(1,left);
	setServoSpeed(0,right);
}

void setServoSpeed(unsigned int servo, int speed){
 	if (speed > 100)
 		speed = 100;
	if (speed < -100)
		speed = -100;
	
 	dutyCycle(servo, NO_MOVE + (((int)speed * SPEED_RANGE) / DUTY_CONST));
}




