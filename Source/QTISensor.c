/*
	Authors: Davide Berdin, Till Riemer
*/
#include "QTISensor.h"

static unsigned int black;

/* initialize QTI input pins and set black color according to initial input */
void initQTI(){
        QTI_DDR |= (1<<LR);
        QTI_PORT &= ~(1<<LR);

        QTI_DDR |= (1<<RR);
        QTI_PORT &= ~(1<<RR);

		black = MAX_BLACK;
        black = (rightBrightness()+leftBrightness());
}

static inline void rightHigh(){
        // set to output, sleep 1ms
        QTI_DDR |= (1<<RR);
        QTI_PORT |= (1<<RR);
        delay(5);
}

static inline void leftHigh(){
        // set to output, sleep 1ms
        QTI_DDR |= (1<<LR);
        QTI_PORT |= (1<<LR);
        delay(5);
}

static inline void rightLow(){
        // set to input
        QTI_PORT &= ~(1<<RR);
        QTI_DDR &= ~(1<<RR);
}

static inline void leftLow(){
        // set to input
        QTI_PORT &= ~(1<<LR);
        QTI_DDR &= ~(1<<LR);
}

static inline unsigned int readRight(){
        return bit_is_set(QTI_PIN, RR);
}

static inline unsigned int readLeft(){
        return bit_is_set(QTI_PIN, LR);
}

unsigned int rightBrightness(){
        unsigned int bright = 0;

        rightHigh();
        cli();
        rightLow();
        while ((readRight() && bright) < black) ++bright;
        sei();
       
        return bright;
}

unsigned int leftBrightness(){
        unsigned int bright = 0;

        leftHigh();
        cli();
        leftLow();
        while ((readLeft() && bright) < black) ++bright;
        sei();
       
        return bright;
}

unsigned int rightIsWhite(){
        return rightBrightness() < black;
}

unsigned int leftIsWhite(){
        return leftBrightness() < black;
}
