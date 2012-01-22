/*
	Authors: Davide Berdin, Till Riemer
	

*/
#ifndef _INIT
#define _INIT

#define NO_MOVE 		750		// no movement of a motor
#define SLOW_FWD		50		// minimum forward
#define FAST_FWD		250		// maximum forward
#define SLOW_BWD		-50		// min backward
#define FAST_BWD		-250	// max backward
#define LEFT 			OCR1B	// left motor
#define RIGHT			OCR1A	// right motor


typedef struct _State{
	int leftMotor;
	int rightMotor;
	int isStopped;
} State;

State *state;

State StateInit();
void motorInit();

#endif
