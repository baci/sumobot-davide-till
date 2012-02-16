#include "QTISensor.h"

static uint32_t _black=INIT_BLACK;

void initQTI(){
	QTI_DDR |= (1<<QTI_LEFT);
	QTI_DDR |= (1<<QTI_RIGHT);
	QTI_PORT &= ~(1<<QTI_LEFT);
	QTI_PORT &= ~(1<<QTI_RIGHT);

	_black = INIT_BLACK; 					// "real" black, deepest possible color
	_black = (countRight() + countLeft()); 	// determine "black" based on initial position
}

static inline void dischargeLeft(){
	/* set output to HIGH, pause > 1ms */
	QTI_DDR |= (1<<QTI_LEFT);
	QTI_PORT |= (1<<QTI_LEFT);
	delay(5);
}

static inline void dischargeRight(){
	/* set output to HIGH, pause > 1ms */
	QTI_DDR |= (1<<QTI_RIGHT);
	QTI_PORT |= (1<<QTI_RIGHT);
	delay(5);
}

static inline void startChargeLeft(){
	QTI_PORT &= ~(1<<QTI_LEFT);
	QTI_DDR &= ~(1<<QTI_LEFT);
}

static inline void startChargeRight(){
	QTI_PORT &= ~(1<<QTI_RIGHT);
	QTI_DDR &= ~(1<<QTI_RIGHT);
}

static inline int readLeft(){
	return bit_is_set(QTI_PIN, QTI_LEFT);
}

static inline int readRight(){
	return bit_is_set(QTI_PIN, QTI_RIGHT);
}

unsigned int countLeft(){
	unsigned int t=0;

	dischargeLeft();
	
	t = 0;
	cli();
	startChargeLeft();
	while (readLeft() && t < _black) ++t;
	sei();
	
	return t;
}

unsigned int countRight(){
	unsigned int t=0;

	dischargeRight();
	
	t = 0;
	cli();
	startChargeRight();
	while (readRight() && t < _black) ++t;
	sei();
	
	return t;
}

int leftIsWhite(){
	return countLeft() < _black;
}

int rightIsWhite(){
	return countRight() < _black;
}
