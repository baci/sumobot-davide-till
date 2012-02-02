/*
	Authors: Davide Berdin, Till Riemer
*/

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "servos.h"
#include "Sensors.h"
#include "LCD_driver.h"
#include "LCD_functions.h"
#include "timer.h"
#include "QTISensor.h"

#define RIGHT	0
#define LEFT	1

#define SEEK		0
#define ATTACK		1
#define ON_EDGE		2

typedef struct stateMachine {
	int currentState;
};

typedef struct stateMachine State;

int status(int);
int seek(int);
int attack(int);
int onEdge(int);

int main(void){
	//LCD_Init();
	motorInit();
	InitTimer();
	QTIInit();
	
	// small delay to prepare for the battle
	//delay(1000);
	/* The Sumobot must initially be on black playarea */
	QTIAdjust();	

	// variables for the state machine
	State *state;
	state = (State *)malloc(sizeof(State));
	state->currentState = SEEK;

	while(1){
		state->currentState = status(state->currentState);
	}
	return 0;
}

int status(int _status){
	int status = _status;

	if(status == 0){ 
		seek(status); 
	} else {
		if(status == 1) {
			attack(status);
		} else {
			if(status == 2){ 
				onEdge(status);
			}
		}
	}
	return status = 0;	
}

int seek(int _status){
	int status = _status;

	int direction = RIGHT;
	while(1){
		if (direction == RIGHT)
			setMotorSpeed(100, 100);
		else { 
			if(direction == LEFT) 
				setMotorSpeed(100, 100);
		}
		if (left_outside() || right_outside()){
			status = ON_EDGE;
			return status;
		}
		/*
		if (obstacle_right() || obstacle_left())
			return ATTACK;
		*/
	}
}

int attack(int _status){
	int status = _status;
	return status;
}

int onEdge(int _status){
	int status = _status;

	while (1){
		if (right_outside()){
			setMotorSpeed(0, 100);
			delay(300);
		} else if (left_outside()){
			setMotorSpeed(100, 0);
			delay(300);
		} else {
			status = SEEK;
			return status;
		}
	}
	status = ON_EDGE;
	return status;
}

