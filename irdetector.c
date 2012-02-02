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
				setMotorSpeed(100, 100);
				delay(2000);
				stop();
				break;
			case 2:
				setMotorSpeed(100, 0);
				delay(2000);
				stop();
				break;
			case 3:
				setMotorSpeed(0,100);
				delay(2000);
				stop();
				break;
			case 4:
				setMotorSpeed(0,100);
				delay(2000);
				stop();
				break;
			case 5:
				TimerWait(2000);
				return 0;
			default:
				setMotorSpeed(0, 0);
		}
	}
	return 0;
}
