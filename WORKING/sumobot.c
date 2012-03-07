/*
	Authors: Davide Berdin, Till Riemer
*/

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"
#include "servos.h"
#include "QTISensor.h"
#include "IRSensor.h"
#include "pilot.h"
#include "LED.h"
#include "sumobot.h"

int main(void){
	init();
	initTime();
	initLED();
	initMotors();
	initQTI();	
	initIRemittors();
	initIRdetectors();

	delay(5000);

	calibrateQTI(); // adjust "black" color
	
	seek();

	return 0;
}

/* initialize the system clock - set to 8Mhz */
void init(){
	cli();

	CLKPR = 0;

	// enable changing the clock prescaler
	CLKPR |= (1 << CLKPCE);

	// No scaling
	CLKPR |= (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);

	sei();
}

int seek(){
	int oppRight;
	int oppLeft;
	int oppLast;
	int lineAtLeft;
	int lineAtRight;
	unsigned int enemySpotted = 0;

	while(1){
		/* get sensor data */
		oppRight 	=	objectRight();
		oppLeft 	=	objectLeft();
		lineAtLeft 	=	lineLeft();
		lineAtRight	= 	lineRight();


		// checking and reacting on lines...
		if (lineAtLeft==1){
			if(enemySpotted==0){
				moveBackward(100);
				delay(700);
				turnRight(100);
				delay(700);
			}
		} 
		if (lineAtRight==1){
			if(enemySpotted==0){
				moveBackward(100);
				delay(700);
				turnLeft(100);
				delay(700);
			}
		}
		if(lineAtLeft==0 && lineAtRight==0){
			// when no white line, reacting on enemies...		

			if (!oppRight && !oppLeft){			// no enemies detected, use last known values
				enemySpotted = 0;
				moveForward(100);
			}
			else if (oppRight && oppLeft){	// enemy at FRONT
				enemySpotted = 1;
				moveForward(100);
			} else if (oppRight){			// enemy at RIGHT
				enemySpotted = 1;
				turnRight(100);
			} else if (oppLeft){			// enemy at LEFT
				enemySpotted = 1;
				turnLeft(100);
			}

		}
	}
	return 0;
}

