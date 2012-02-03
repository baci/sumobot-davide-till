#ifndef _QTI_
#define _QTI_ 1

#include <avr/io.h>
#include <avr/interrupt.h>

#define _BLACK  	300
#define _TOP_BLACK  5000

/*** Functions ***/

void QTIInit();
int right_raw();
int left_raw();
void QTIAdjust();

/* Returns positive if sensor is outside the arena ( = white) */
int right_outside();
int left_outside();

#endif
