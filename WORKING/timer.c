/* timer for delays and AI, using HW timer 2 */

#include "timer.h"

volatile int ms = 0;

void initTime(){
	cli();

	ms = 0;

	TCCR2A = 0;

	// CTC, clear on compare match
	TCCR2A |= (1 << WGM21) | (0 << WGM20);

	// OCR interrupt enable
	TIMSK2 |= (1 << OCIE2A);

	// setting asynchronous clock
	ASSR |= (1 << AS2);

	// 1024 Hz
	OCR2A = 1;

	TCNT2 = 0;

	sei();
}

void startTimer(){
	cli();

	ms = 0;

	// 1024 Hz
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

void delay(int ms){
	int stop = 0;
	if (stop = !timerRunning()) startTimer();
	static int last = 0;
	last = getMs();
	while ((getMs() - last) < ms) sleep_mode();
	if (stop) stopTimer();
}

int timeDelta(int * start_o){
	int start = *start_o;
	*start_o = getMs();
	return *start_o - start;
}

int timeSince(int t_start){
	return getMs() - t_start;
}
