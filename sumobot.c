/*
	Authors: Davide Berdin, Till Riemer

*/

#include <stdlib.h>
#include <avr/io.h>
#include "init.h"
#include "LCD_driver.h"
#include "LCD_functions.h"

#define NO_MOVE 		750
#define LEFT 			OCR1B
#define RIGHT			OCR1A


void setMotorSpeed(int, int);


int main(void)
{
	LCD_Init();
	motorInit();
	*state = StateInit();
	secondsPassed = 0;

	setMotorSpeed(100, 100);
	delay(1000);
	setMotorSpeed(-100, -100);
	delay(2000);
	setMotorSpeed(100, -100);

	while(1){
		LCD_puts("running", 0);

		
		
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
