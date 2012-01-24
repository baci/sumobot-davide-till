#include "time_correct.h"

volatile uint32_t msecs = 0;

void initialize_time(){
	cli();

	msecs = 0;

	/* Timer 0 */
	TCCR0A = 0;

	/* CTC, clear on compare match */
	TCCR0A |= (1<<WGM01)|(0<<WGM00);

	/* OCR interrupt enable */
	TIMSK0 |= (1<<OCIE0A);

	/* 7812/8 = 976.5 Hz */
	OCR0A = 8;

	sei();
}

static void start_timer(){
	cli();

	msecs = 0;

	TCCR0A &= ~((1<<CS02)|(0<<CS01));
	TCCR0A |= (1<<CS02)|(0<<CS01)|(1<<CS00);

	TCNT0 = 0;

	sei();
}

static void stop_timer(){
	cli();

	TCCR0A &= ~(1<<CS22|1<<CS21|1<<CS20);
	
	sei();
}

ISR(TIMER0_COMP_vect){
	msecs++;
}

// returns passed ms since last start_timer() call
static volatile uint32_t get_sys_ms(){
	static volatile uint32_t retval = 0;
	cli();
	retval = msecs;
	sei();
	return retval;
}

void delay(uint32_t ms){
	start_timer();
	static uint32_t last_time = 0;
	last_time = get_sys_ms();
	while ((get_sys_ms()-last_time) < ms) sleep_mode();
	stop_timer();
}
