#include "servos.h"
#include "IRemettitor.h"
#include "IRdetector.h"
#include "sumobot.h"
#include "timer.h"

int IRdetect(){
	int n = 0;
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
				delay(2000);
				stop();
				break;
			case 2:
				moveLeft();
				delay(2000);
				stop();
				break;
			case 3:
				moveRight();
				delay(2000);
				stop();
				break;
			case 4:
				moveBwd();
				delay(2000);
				stop();
				break;
			case 5:
				delay(2000);
				return 0;
			default:
				stop();
		}
	}
	return 0;
}
