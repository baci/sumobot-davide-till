/*
	Authors: Davide Berdin, Till Riemer

*/


#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "init.h"
#include "LCD_driver.h"
#include "LCD_functions.h"

void setMotorSpeed(int, int);

int main(void)
{
	LCD_Init();
	motorInit();
	

	setMotorSpeed(750, 750);

	while(1){
		LCD_puts("Hello fucker", 0);		
	}

	return 0;
}

/* sets the speed of the left and right motor, no movement is 750 - remember that the right motor 
is mirrowed to the left */
void setMotorSpeed(int left, int right)
{
	cli();
	OCR1A = left;
	OCR1B = right;
	sei();
}
