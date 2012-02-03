#include <avr/io.h>
#include "QTISensor.h"
#include "timer.h"

static int _black = _BLACK;

void QTIInit(){
		// left QTI
        DDRB |= (1<<PB2);
        PORTB &= ~(1<<PB2);
		
		// right QTI
        DDRB |= (1<<PB0);
        PORTB &= ~(1<<PB0);

		QTIAdjust(); // initially black area
}


/* determine initial ground color for black */
void QTIAdjust(){
        _black = _TOP_BLACK;
        _black = (right_raw()+left_raw())/10;
}

/*** Left side ***/

/* R HIGH output and >1ms sleep */
static inline void discharge_left(){
        DDRB |= (1<<PB2);
        PORTB |= (1<<PB2);
        delay(5);
}

/* R LOW input */
static inline void start_charge_left(){
        PORTB &= ~(1<<PB2);
        DDRB &= ~(1<<PB2);
}

static inline int read_left(){
        return bit_is_set(PINB, PB2);
}

/* returns amount of 1's counted from sensor */
int left_raw(){
        int count=0;
        discharge_left();
        
        cli();
        start_charge_left();
        while (read_left() && count<_black) ++count;
        sei();
        
        return count;
}

int left_outside(){
        return left_raw() < _black;
}

/*** Right side ***/

/* R HIGH output, >1ms sleep */
static inline void discharge_right(){
        DDRB |= (1<<PB0);
        PORTB |= (1<<PB0);
        delay(5);
}

/* R LOW input */
static inline void start_charge_right(){
        PORTB &= ~(1<<PB0);
        DDRB &= ~(1<<PB0);
}

static inline int read_right(){
        return bit_is_set(PINB, PB0);
}

/* returns amount of 1's counted from sensor */
int right_raw(){
        int count=0;
        discharge_right();
        
        cli();
        start_charge_right();
        while (read_right() && count<_black) ++count;
        sei();
        
        return count;
}

int right_outside(){
        return right_raw() < _black;
}
