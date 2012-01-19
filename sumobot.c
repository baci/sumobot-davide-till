/*
	Authors: Davide Berdin, Till Riemer

*/

#include <stdlib.h>
#include <avr/io.h>
#include "init.h"
#include "time_correct.h"
#include "Sensors.h"
#include "LCD_driver.h"
#include "LCD_functions.h"

void startRobot(int);
void setMotorSpeed(int, int);

int state_machine;	//global variable for the state machine

// state=0 - start moving forward, state=1 - move left, start=2 - move right

void startRobot(int state_machine){
	while(1){
		switch(state_machine){
		case 0:
			//delay(2000);		
			setMotorSpeed(100, 100);
			state_machine = 1;
			break;
		case 1:
			delay(2000);
			setMotorSpeed(100, 0);
			delay(1000);
			state_machine = 2;
			break;
		case 2:
			delay(2000);
			setMotorSpeed(0, 0);
			break;
		}
	}
}


int main(void){
	LCD_Init();
	motorInit();
	*state = StateInit();
	*emitter = EmitterInit();
	*detector = DetectorInit();
	
	initialize_time();

	//startRobot(0);
	
	while(1)
	{
		LCD_puts("hello",0);
	delay(1000);
	LCD_Clear();
	LCD_puts("world",0);
	delay(2000);
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
