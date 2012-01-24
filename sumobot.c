/*
	Authors: Davide Berdin, Till Riemer
*/

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "servos.h"
#include "time_correct.h"
#include "Sensors.h"
#include "LCD_driver.h"
#include "LCD_functions.h"

void setMotorSpeed(int, int);

int main(void){
	LCD_Init();
	motorInit();
	*state = StateInit();
	*emitter = EmitterInit();
	*detector = DetectorInit();
	
	initialize_time();
	
	LCD_puts("hello",0);
	delay(1000);
	LCD_Clear();
	LCD_puts("world",0);
	delay(2000);

	setMotorSpeed(100, 100);

	while(1){
		if(!readLineSensor(1)){ // line at left
			setMotorSpeed(100, 0);
			delay(1000);
		}
		if(!readLineSensor(2)){ // line at right
			setMotorSpeed(0, 100);
			delay(1000);
		}
	}
	return 0;
}


/* sets the speed of the left and right motor */
void setMotorSpeed(int left, int right)
{
	// set motor timers...
	cli();
	LEFT = NO_MOVE + left;
	RIGHT = NO_MOVE - right;
	sei();

	// update state struct...
	state->leftMotor = left;
	state->rightMotor = right;
	if(left==0 && right==0) state->isStopped = 1;
	else state->isStopped = 0;
}
