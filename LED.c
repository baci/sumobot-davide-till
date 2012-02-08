#include "LED.h"


void initLED(){
	LED_DDR |= (1<<GREEN);
	LED_DDR |= (1<<RED);
	LED_PORT &= ~(1<<GREEN);
	LED_PORT &= ~(1<<RED);
}

void toggleGreen(){		// green=right
	LED_PORT ^= (1<<GREEN);
}
void setGreen(){
	LED_PORT |= (1<<GREEN);
}
void clearGreen(){
	LED_PORT &= ~(1<<GREEN);
}

void toggleRed(){		// red=left
	LED_PORT ^= (1<<RED);
}
void setRed(){
	LED_PORT |= (1<<RED);
}
void clearRed(){
	LED_PORT &= ~(1<<RED);
}
