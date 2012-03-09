/*
	Authors: Davide Berdin, Till Riemer
*/
#ifndef PILOT
#define PILOT

#include "servos.h"
#include "LED.h"
#include "timer.h"

void stop();
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnBackLeft(int speed);
void spinLeft(int speed);
void turnRight(int speed);
void turnBackRight(int speed);
void spinRight(int speed);


#endif
