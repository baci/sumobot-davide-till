/*
	Authors: Davide Berdin, Till Riemer
*/
#include "sumobot.h"

int main(void){
	init();
	initTimer();
	initLED();
	initServos();
	initQTI();	
	initEmittors();
	initDetectors();
	
	/* initialization for push buttons in order to determine whether the robot is pushed from the side,
		as we had to rebuild our bot on the last day, we didnt have the time to implement this
		feature properly - see also the ISRs below */
	//PCMSK |= (1<<PD2) | (1<<PD3); // set pins for push button interrupts
	//MCUCR = (1<<ISC01) | (1<<ISC00); // set interrupt to falling edge (sensor triggered)
	//PCMSK0  |= (1<<PCINT0) | (1<<PCINT1); // enable interrupts for buttons

	delay(5000);
	
	mainLoop();

	return 0;
}

/* push button ISRs, not used

ISR(PCINT0_vect) // left push button
{ 
    turnRight(100);
	delay(1500);
}

ISR(PCINT1_vect) // right push button
{ 
    turnLeft(100);
	delay(1500);
}

*/


/* initialize the system clock (8Mhz) */
void init(){
	cli();

	// enable clock prescaler changing and set to no scaling
	CLKPR = 0;
	CLKPR |= (1 << CLKPCE);
	CLKPR |= (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);

	sei();
}

int mainLoop(){
	int oppRight;
	int oppLeft;
	int lineAtLeft;
	int lineAtRight;
	unsigned int enemySpotted = 0;
	unsigned int movement=0; // current seek movement direction (0=left, 1=right)
	unsigned int i=0; // movement amount since last left-right change


	while(1){
		// get sensor data
		oppRight 	=	objectRight();
		oppLeft 	=	objectLeft();
		lineAtLeft 	=	leftIsWhite();
		lineAtRight	= 	rightIsWhite();


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
				if(i>3){						// change movement direction after 3 times
					(movement==1)?(movement=0):(movement=1);
					i=0;
				}
				if(movement==0) turnLeft(100);
				if(movement==1)	turnRight(100);
				delay(100);
				i++;
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

