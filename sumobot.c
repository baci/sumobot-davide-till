/*
	Authors: Davide Berdin, Till Riemer
*/

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "servos.h"
#include "LCD_driver.h"
#include "LCD_functions.h"
#include "timer.h"
#include "QTISensor.h"
#include "IRSensor.h"
#include "pilot.h"
#include "LED.h"



void init();
int seek();

int main(void){
	init();
	LCD_Init();
	initMotors();
	//initTimer();
	initialize_time();
	initQTI();	
	initLED();
	initialize_IR_LEDs();
	initialize_IR_detectors();

	delay(1000);
	
	seek();

	return 0;
}

/* Set clock frequency to 8 MHz */
void init(){
	cli();

	CLKPR = 0;

	/* Enable changing the clock prescaler */
	CLKPR |= (1<<CLKPCE);

	/* No scaling; This is only for reference */
	CLKPR |= (0<<CLKPS3)|(0<<CLKPS2)|(0<<CLKPS1)|(0<<CLKPS0);

	sei();
}

#define FRONT 	0
#define LEFT	1
#define RIGHT	2

int seek(){
	int opp_right;
	int opp_left;
	int opp_last;
	int p, i;

	while(1){
		/* Sensor */
		opp_right = obstacle_right();
		opp_left = obstacle_left();

		/* Act on suspicion when unknown */
		if (!opp_right && !opp_left){
			opp_right = (opp_last == RIGHT || opp_last == FRONT);
			opp_left = (opp_last == LEFT || opp_last == FRONT);
			LCD_puts("NOTHING",0);
			moveForward(100);
		}

		/* Adjustments */
		if (opp_right && opp_left){
			
			p = 0;
			i = 0;
			opp_last = FRONT;
			LCD_puts("FRONT",0);
			moveBackward(100);
		} else if (opp_right){
			p = 1;
			if (opp_last == RIGHT) ++i;
			opp_last = RIGHT;
			LCD_puts("RIGHT",0);
			turnRight(100);
		} else if (opp_left){
			p = -1;
			if (opp_last == LEFT) --i;
			opp_last = LEFT;
			LCD_puts("LEFT",0);
			turnLeft(100);
		}

		

		/*	
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
		*/
	}
	return 0;
}

