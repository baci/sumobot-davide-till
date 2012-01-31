#include <avr/io.h>
#include "QTISensor.h"
#include "timer.h"

void QTISensorInit(){
	// set direction as out
	DDRB |= (1 << PB0) | (1 << PB2);

	//set bits to high to initilize
	PORTB |= (1 << PB0) | (1 << PB2);
	
	//according with the datasheet, after initialization we need to have a delay of 1ms
	TimerWait(1);
	
	//set bits to low for reading
	DDRB &= ~(1<<PB0);
	DDRB &= ~(1<<PB2);

}

int readLeftQTISensor(){
	QTISensorInit();
	return(bit_is_set(PINB, PINB2)); //robot on the black ground (1)
}

int readRightQTISensor(){
	QTISensorInit();
	return(bit_is_set(PINB, PINB0)); //robot on the black ground (1)
}
