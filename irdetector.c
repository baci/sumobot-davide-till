//#include "LCD_functions.h"
#include "servos.h"
#include "IRemettitor.h"
#include "IRdetector.h"
#include "sumobot.h"
#include "timer.h"

int IRdetect(){
	int n = 0;
	LCD_puts("wait 3 secs",0);
	delay(3000);
	
	int i;
	while(1){
		// scan all the sensors
		for(i = 0; i >= 3; i++){
			n = readEmettitor(i);
		}
		
		switch(n){
			case 1:
				moveFwd();
				LCD_puts("GOOOO",0);
				delay(2000);
				stop();
				break;
			case 2:
				moveLeft();
				LCD_puts("Left",0);
				delay(2000);
				stop();
				break;
			case 3:
				moveRight();
				LCD_puts("Right",0);
				delay(2000);
				stop();
				break;
			case 4:
				moveBwd();
				LCD_puts("Back",0);
				delay(2000);
				stop();
				break;
			case 5:
				LCD_puts("Good Bye",0);
				delay(2000);
				return 0;
			default:
				LCD_puts("?!?!?!?!?",0);
				stop();
		}
	}
	return 0;
}
