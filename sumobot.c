/*
	Authors: Davide Berdin, Till Riemer

*/


#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "init.h"
#include "LCD_driver.h"

int main(void)
{
	LCD_Init();
	LCD_puts("AVR BUTTERFLY GCC");

	for(;;){}

	return 0;
}
