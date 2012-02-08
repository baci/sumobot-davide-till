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
#include "LED.h"


int seek();

int main(void){
	LCD_Init();
	initMotors();
	initTimer();
	initQTI();	
	initLED();

	delay(1000);
	
	seek();

	return 0;
}


int seek(){
	while(1){
		
		if (leftIsWhite()){
			LCD_puts("leftout",0);
			turnBackRight(100);
		} 
		else if (rightIsWhite()){
			LCD_puts("rightout",0);
			turnBackLeft(100);
		}
		else if(!leftIsWhite() && !rightIsWhite()){
			LCD_puts("ok",0);
			moveForward(100);
		}
	}
	return 0;
}

