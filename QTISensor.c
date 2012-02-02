#include <avr/io.h>
#include "QTISensor.h"
#include "timer.h"

/* We need to change (maybe) the number of port here in the code */

static int _black = _BLACK;

void QTIInit(){
		// left QTI
        DDRB |= (1<<PB2);
        PORTB &= ~(1<<PB2);
		
		// right QTI
        DDRB |= (1<<PB0);
        PORTB &= ~(1<<PB0);
}

void QTIAdjust(){
        _black = _TOP_BLACK;
        _black = (right_raw()+left_raw())/10;
}

/*** Left side ***/

static inline void discharge_left(){
        /* R HIGH output */
        /* 1 ms pause */
        DDRB |= (1<<PB2);
        PORTB |= (1<<PB2);
        delay(5);
}

static inline void start_charge_left(){
        /* R LOW input */
        PORTB &= ~(1<<PB2);
        DDRB &= ~(1<<PB2);
}

static inline int read_left(){
        /* R input value */
        return bit_is_set(PINB, PB2);
}

int left_raw(){
        int count=0;

        discharge_left();
        
        count = 0;
        cli();
        start_charge_left();
        while (read_left() && count<_black) ++count;
        sei();
        
        return count;
}

int left_outside(){
        return left_raw()<_black;
}

/*** Right side ***/

static inline void discharge_right(){
        /* R HIGH output */
        /* 1 ms pause */
        DDRB |= (1<<PB0);
        PORTB |= (1<<PB0);
        delay(5);
}

static inline void start_charge_right(){
        /* R LOW input */
        PORTB &= ~(1<<PB0);
        DDRB &= ~(1<<PB0);
}

static inline int read_right(){
        /* R input value */
        return bit_is_set(PINB, PB0);
}

int right_raw(){
        int count=0;

        discharge_right();
        
        count = 0;
        cli();
        start_charge_right();
        while (read_right() && count<_black) ++count;
        sei();
        
        return count;
}

int right_outside(){
        return right_raw()<_black;
}
