/*
	Authors: Davide Berdin, Till Riemer
*/
/* timer for delays and AI, using HW timer 2 */

#include "timer.h"

volatile int ms = 0;

void initTimer(){
	cli();

	ms = 0;
	TCCR2A = 0;	
	TCCR2A |= (1 << WGM21) | (0 << WGM20); // CTC, clear on compare match	
	TIMSK2 |= (1 << OCIE2A); // OCR interrupt enable	
	ASSR |= (1 << AS2); // setting asynchronous clock	
	OCR2A = 1; // 1024 Hz
	TCNT2 = 0;

	sei();
}

void startTimer(){
	cli();

	ms = 0;
	// 1024Hz...
	TCCR2A &= ~(1 << CS22);
	TCCR2A |= (0 << CS22) | (1 << CS21) | (1 << CS20);
	TCNT0 = 0;

	sei();
}

void stopTimer(){
	cli();

	TCCR2A &= ~(1 << CS22 | 1 << CS21 | 1 << CS20);
	
	sei();
}

ISR(TIMER2_COMP_vect){
	ms += 2;
}

static inline int timerRunning(){
	return TCCR2A & (1 << CS22 | 1 << CS21 | 1 << CS20);
}

static volatile int getMs(){
	volatile int ret = 0;
	cli();
	ret = ms;
	sei();
	return ret;
}

/* sleeping for the specified amount of ms */
void delay(int ms){
	int stop = 0;
	if (stop == !timerRunning()) startTimer();
	static int last = 0;
	last = getMs();
	while ((getMs() - last) < ms) sleep_mode();
	if (stop) stopTimer();
}
