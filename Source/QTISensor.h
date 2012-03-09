/*
	Authors: Davide Berdin, Till Riemer
*/
#ifndef QTISensor
#define QTISensor

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

#define QTI_PORT                PORTB
#define QTI_DDR                 DDRB
#define QTI_PIN                 PINB
#define RR                      PB1
#define LR                      PB0

#define MAX_BLACK               5000


void initQTI();
unsigned int rightBrightness();
unsigned int leftBrightness();
unsigned int rightIsWhite();
unsigned int leftIsWhite();

#endif
