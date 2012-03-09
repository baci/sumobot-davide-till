/*
	Authors: Davide Berdin, Till Riemer
*/
#ifndef SUMOBOT
#define SUMOBOT

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "servos.h"
#include "QTISensor.h"
#include "IRSensor.h"
#include "pilot.h"
#include "LED.h"

void init();
int mainLoop();

#endif
