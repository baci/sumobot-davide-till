/*
	Authors: Davide Berdin, Till Riemer

*/

#include <stdlib.h>
#include <avr/io.h>
#include "init.h"
#include "time.h"
#include "Sensors.h"
#include "LCD_driver.h"
#include "LCD_functions.h"


void setMotorSpeed(int, int);

void waitMs(int ms){
	int temp=0, temp2=0;
	while(temp<ms){
		temp++;
		while(temp2<650){
			temp2++;
		}
	}
}

int main(void){
	//LCD_Init();
	motorInit();
	*state = StateInit();
	*emitter = EmitterInit();
	*detector = DetectorInit();
	
	

	setMotorSpeed(100,100);
	delayMs(1);
	
	setMotorSpeed(0,0);	
	delayMs(1);
	
	setMotorSpeed(100,100);
	delayMs(1);
	
	setMotorSpeed(0,0);
	
	while(1){
		/*setMotorSpeed(100, 100);
		LCD_puts("running", 0);
		
		if(readLineSensor(1) && !readLineSensor(2)){
			setMotorSpeed(100, 0);// line at left - move to the right
			waitMs(2000);
		}
		if(readLineSensor(2)){
			setMotorSpeed(0, 100);// line at right or both - move to the left
			waitMs(2000);
		}*/
		
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
