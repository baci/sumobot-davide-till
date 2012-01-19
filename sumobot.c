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


void setMotorSpeed(int, int);

int main(void){
	LCD_Init();
	//motorInit();
	//*state = StateInit();
	//*emitter = EmitterInit();
	//*detector = DetectorInit();
	
	LCD_puts("here",0);
	//setMotorSpeed(100,100);
	delay(2000);
	LCD_puts("there",0);
	/*setMotorSpeed(0,0);	
	delay(10);
	
	setMotorSpeed(-100,-100);
	delay(10);
	
	setMotorSpeed(0,100);
	*/
	while(1){
		/*setMotorSpeed(100, 100);
		LCD_puts("running", 0);
		
		if(readLineSensor(1) && !readLineSensor(2)){
			setMotorSpeed(100, 0);// line at left - move to the right
			delay(1000);
		}
		if(readLineSensor(2)){
			setMotorSpeed(0, 100);// line at right or both - move to the left
			delay(1000);
		}
		*/
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
