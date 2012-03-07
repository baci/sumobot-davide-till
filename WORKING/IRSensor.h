#ifndef IRSENSOR
#define IRSENSOR

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

#define PORT		PORTD
#define DDR			DDRD
#define PIN			PIND
#define RIGHT		PD0
#define LEFT		PD1


/* sensitivity of the IR, "original" is 104 */
#define SENSITIVITY		93

#define SAMPLES			5	// amount of times checked for evaluation
#define INTERVAL		1	// interval in ms
#define BORDERVALUE		3

void initIRemittors();
void initIRdetectors();
int objectRight();
int objectLeft();
void startIR();
void stopIR();

#endif
