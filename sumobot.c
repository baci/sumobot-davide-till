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
	initTimer();
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

int seek(){
	while(1){
		/*
		if (obstacle_right()){
			LCD_puts("RIGHT",0);
			setGreen();
			turnRight(100);

		}
		else if(obstacle_left()){
				LCD_puts("LEFT",0);
				setRed();
				turnLeft(100);
		}
		else{
			LCD_puts("NO",0);
			clearGreen();
			clearRed();
			moveForward(100);
			}*/
			
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

