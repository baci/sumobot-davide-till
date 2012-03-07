#ifndef SUMOBOT_H
#define SUMOBOT_H

#define BOOL char
#define FALSE 0
#define TRUE (!FALSE)
#define NULL ((void *)0)

/* macros to set and clear bits in registers */
#define SetBit(sfr, bit) ((sfr) |= (1 << (bit)))
#define ClearBit(sfr, bit) ((sfr) &= ~(1 << (bit)))

#define ENEMY_FRONT 0
#define ENEMY_LEFT	1
#define ENEMY_RIGHT	2

void init();
int seek();

#endif

