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
#include "pilot.h"

#define RIGHT	0
#define LEFT	1


int seek();

int main(void){
	LCD_Init();
	motorInit();
	InitTimer();
	QTIInit();	

	delay(1000);
	seek();

	return 0;
}


int seek(){
	while(1){
		if (right_outside()){
			LCD_puts("rightout",0);
			turnBackLeft(100);
			delay(500);
		} 
		if (left_outside()){
			LCD_puts("leftout",0);
			turnBackRight(100);
			delay(500);
		}
		if(!left_outside() && !right_outside()){
			LCD_puts("ok",0);
			//moveForward(100);
			turnBackRight(100);
		}
	}
	return 0;
}

