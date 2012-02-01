#ifndef _QTI_
#define _QTI_ 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include "time.h"

#define _BLACK  	300
#define _TOP_BLACK  5000

/*** Functions ***/

void QTIInit();
void calibrate_QTI();
uint32_t right_raw();
uint32_t left_raw();

/* Returns positive if sensor is outside the arena */
uint8_t right_outside();
uint8_t left_outside();

#endif
