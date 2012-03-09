/*
	Authors: Davide Berdin, Till Riemer
*/
#ifndef IRSENSOR
#define IRSENSOR

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

#define PORT		PORTD
#define DDR			DDRD
#define PIN			PIND
#define RIGHT		PD0
#define LEFT		PD2


#define SENSITIVITY		100 // sensitivity of the IR, original was 104
#define SAMPLES			5	// amount of times checked for evaluation
#define BORDERVALUE		4	// amount of successful checks needed for verification

void initEmittors();
void initDetectors();
int objectRight();
int objectLeft();
void startIR();
void stopIR();

#endif
