#ifndef SUMOBOT_H
#define SUMOBOT_H

#define BOOL char
#define FALSE 0
#define TRUE (!FALSE)
#define NULL ((void *)0)

/* macros to set and clear bits in registers */
#define SetBit(sfr, bit) ((sfr) |= (1 << (bit)))
#define ClearBit(sfr, bit) ((sfr) &= ~(1 << (bit)))

void InitSumobot(void);
int readLeftIR(void);
int readRightIR(void);
void turnRightOverLine(void);
void turnLeftOverLine(void);

#endif

