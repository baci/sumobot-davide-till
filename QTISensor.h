/*
	authors: Davide Berdin, Till Riemer

	Functions for the use of QTI/Line sensors
*/


#ifndef _QTI_
#define _QTI_ 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

#define QTI_PORT		PORTB
#define QTI_DDR			DDRB
#define QTI_PIN			PINB

#define QTI_RIGHT		PB0
#define QTI_LEFT		PB1

#define INIT_BLACK		5000

/* calculates the value which should be the lowest one accepted as black */
void initQTI();

/* returns count of 1's returned by sensor for determining brightness of surface */
unsigned int countRight();
unsigned int countLeft();

/* returns positive if sensor is on white ground */
int rightIsWhite();
int leftIsWhite();

#endif
