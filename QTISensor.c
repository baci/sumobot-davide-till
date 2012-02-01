#include <avr/io.h>
#include "QTISensor.h"
#include "timer.h"

/* We need to change (maybe) the number of port here in the code */

static uint32_t _black = _BLACK;

void QTIInit(){
        DDRB |= (1<<PB1);
        PORTB &= ~(1<<PB1);

        DDRB |= (1<<PB0);
        PORTB &= ~(1<<PB0);
}

void calibrate_QTI(){
        _black = _TOP_BLACK;
        _black = (right_raw()+left_raw())/10;
}

/*** Left side ***/

static inline void discharge_left(){
        /* R HIGH output */
        /* 1 ms pause */
        DDRB |= (1<<PB1);
        PORTB |= (1<<PB1);
        TimerWait(5);
}

static inline void start_charge_left(){
        /* R LOW input */
        PORTB &= ~(1<<PB1);
        DDRB &= ~(1<<PB1);
}

static inline uint8_t read_left(){
        /* R input value */
        return bit_is_set(PINB, PB1);
}

uint32_t left_raw(){
        uint32_t t=0;

        discharge_left();
        
        t = 0;
        cli();
        start_charge_left();
        while (read_left() && t<_black) ++t;
        sei();
        
        return t;
}

uint8_t left_outside(){
        return left_raw()<_black;
}

/*** Right side ***/

static inline void discharge_right(){
        /* R HIGH output */
        /* 1 ms pause */
        DDRB |= (1<<PB0);
        PORTB |= (1<<PB0);
        TimerWait(5);
}

static inline void start_charge_right(){
        /* R LOW input */
        PORTB &= ~(1<<PB0);
        DDRB &= ~(1<<PB0);
}

static inline uint8_t read_right(){
        /* R input value */
        return bit_is_set(PINB, PB0);
}

uint32_t right_raw(){
        uint32_t t=0;

        discharge_right();
        
        t = 0;
        cli();
        start_charge_right();
        while (read_right() && t<_black) ++t;
        sei();
        
        return t;
}

uint8_t right_outside(){
        return right_raw()<_black;
}
