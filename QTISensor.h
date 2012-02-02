#ifndef _QTI_
#define _QTI_ 1

#include <avr/io.h>
#include <avr/interrupt.h>

#define _BLACK  	300
#define _TOP_BLACK  5000

/*** Functions ***/

void QTIInit();
void QTIAdjust();
int right_raw();
int left_raw();

/* Returns positive if sensor is outside the arena */
int right_outside();
int left_outside();

#endif
