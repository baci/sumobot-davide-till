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
#include "timer.h"

void setMotorSpeed(int, int);

int main(void){
	LCD_Init();
	motorInit();
	//LineSensorInit();

	InitTimer();

	*state = StateInit();
	//*emitter = EmitterInit();
	//*detector = DetectorInit();
	
	while(1){
		setMotorSpeed(100, 100);
		TimerWait(2000);
		setMotorSpeed(0, 0);
		TimerWait(2000);
	}
	/*{
		if(!readLineSensor(1)){ // line at left
			setMotorSpeed(100, 0);
			delay(1000);
		}
		if(!readLineSensor(2)){ // line at right
			setMotorSpeed(0, 100);
			delay(1000);
		}
	}*/
	return 0;
}
