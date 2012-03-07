/********* Line detection *********
 *
 * Functions for using the line
 * sensors.
 *
 * W is connected to Vcc
 * R is read as an input
 * B is connected to Vss
 *
 **********************************/

#ifndef _QTI_
#define _QTI_ 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

#define QTI_PORT                PORTB
#define QTI_DDR                 DDRB
#define QTI_PIN                 PINB

#define RR                              PB2
#define LR                              PB0

#define DEFAULT_BLACK   300
#define MAX_BLACK               5000

/*** Functions ***/

void initQTI();
void calibrateQTI();
uint32_t right_raw();
uint32_t left_raw();

/* Returns positive if sensor is outside the arena */
uint8_t lineRight();
uint8_t lineLeft();

#endif
