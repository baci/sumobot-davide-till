#include "IRSensor.h"

void initialize_IR_LEDs(){
	cli();

	TCCR2A = 0;

    DDRB |= 1<<PB7; // OC2A as output for IR LEDs
    TCCR2A |= 1<<WGM21 | 1<<COM2A0; // CTC mode and toggle OC2A on compare match

    OCR2A = IR_SENSITIVITY;

	sei();
}

void stop_IR_LEDs(){
	TCCR2A &= ~(1<<CS22|1<<CS21|1<<CS20); // Timer 2 off
}

void start_IR_LEDs(){
	TCNT2 = 0;
	TCCR2A |= 1<<CS20; // no presc
}

void initialize_IR_detectors(){
    DET_DDR &= ~(1<<DET_RIGHT|1<<DET_LEFT); // IR detectors as input    
}

static unsigned int raw_obstacle_right(){
	start_IR_LEDs();
	delay(1);
	stop_IR_LEDs();

    return bit_is_clear(DET_PIN, DET_RIGHT);
}

static unsigned int raw_obstacle_left(){
	start_IR_LEDs();
	delay(1);
	stop_IR_LEDs();
	
    return bit_is_clear(DET_PIN, DET_LEFT);
}

unsigned int obstacle_right(){
	unsigned int hits=0;
	for (unsigned int i=0; i<IR_SAMPLES; ++i){
		if (raw_obstacle_right()) hits++;
		delay(IR_INTERVAL);
	}
	return hits >= IR_THRESHOLD;
}

unsigned int obstacle_left(){
	unsigned int hits=0;
	for (unsigned int i=0; i<IR_SAMPLES; ++i){
		if (raw_obstacle_left()) hits++;
		delay(IR_INTERVAL);
	}
	return hits >= IR_THRESHOLD;
}
