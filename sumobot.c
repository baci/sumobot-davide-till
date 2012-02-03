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
			//setMotorSpeed(650, 850);
			delay(200);
		} 
		if (left_outside()){
			LCD_puts("leftout",0);
			//setMotorSpeed(850, 650);
			delay(200);
		}
		if(!left_outside() && !right_outside()){
			LCD_puts("ok",0);
			//setMotorSpeed(1000,500);
		}
	}
	return 0;
}

