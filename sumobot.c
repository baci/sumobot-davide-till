/*
	Authors: Davide Berdin, Till Riemer
*/

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "servos.h"
#include "Sensors.h"
#include "LCD_driver.h"
#include "LCD_functions.h"
#include "timer.h"
#include "QTISensor.h"

#define STOP 		setMotorSpeed(0,0)
#define FWD_SLOW	setMotorSpeed(20,20)
#define FWD_FAST	setMotorSpeed(255,255)
#define BWD_SLOW	setMotorSpeed(x,x)
#define BWD_FAST	setMotorSpeed(x,x)
#define LEFT_SHARP	setMotorSpeed(100,0)
#define LEFT_SLOW	setMotorSpeed(100,50)
#define RIGHT_SHARP	setMotorSpeed(0,100)
#define RIGHT_SLOW	setMotorSpeed(50,100)
#define TURN_LEFT	setMotorSpeed(x,100)
#define	TURN_RIGHT	setMotorSpeed(100,x)

int main(void){
	LCD_Init();
	motorInit();
	InitTimer();
	//QTISensorInit();	

	while(1){
		if(!readLeftQTISensor()){
			setMotorSpeed(0,100);
			TimerWait(2000);
		}
		else if(!readRightQTISensor()){
			setMotorSpeed(100,0);
			TimerWait(2000);
		}
		setMotorSpeed(100,100);
	}
	return 0;
}
