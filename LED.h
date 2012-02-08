#ifndef LED
#define LED

#include <avr/io.h>

#define LED_DDR		DDRB
#define	LED_PORT	PORTB
#define GREEN		PB3
#define	RED			PB2

void initLED();
void toggleGreen();
void toggleRed();
void clearGreen();
void clearRed();
void setGreen();
void setRed();

#endif
