/*
	Authors: Davide Berdin, Till Riemer
*/
#ifndef LED
#define LED

#include <avr/io.h>

#define LED_DDR		DDRB
#define	LED_PORT	PORTB
#define GREEN		PB2
#define	RED			PB3

void initLED();
void toggleGreen();
void toggleRed();
void clearGreen();
void clearRed();
void setGreen();
void setRed();

#endif
