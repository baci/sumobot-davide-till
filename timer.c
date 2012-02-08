#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "sumobot.h"

/*
*	Timer/Counter 0 (for timing)
*/

/* this counts the number of milliseconds
   since Timer/Counter 0 was initialized
   (volatile to make sure loops work!) */
static volatile unsigned long s_msTimer = 0;

/* sets up Timer/Counter 0 */
void initTimer()
{
	/* start counting at 0 */
	TCNT0 = 0;

	/* clock rate = 8Mhz/1024 = 8kHz
	   CTC (Clear Timer on Compare Match) waveform generation mode
	   OCR0 (top/clear value) = 8
	   for match every 1ms
	*/
	TCCR0A = 0x0D;	// 0000 1101
	OCR0A = 8;

	/* set Compare Match Interrupt Enable (for timer/counter 0) */
	SetBit(TIMSK0, OCIE0A);

	/* set I-bit in SREG (global enable for interrupts) */
	sei();
}

/* returns number of ms since Timer initialized */
unsigned long TimerMsCur()
{
	char sregSav;
	unsigned long ms;

	/* disable (delay) interrupts while we read
	   s_msTimer since it's 4 bytes and we don't
	   want it changing during the read! */
	sregSav = SREG;	// cache register holding Global Interrupt Flag
	cli();			// clear Global Interrupt Flag
	ms = s_msTimer;
	SREG = sregSav;	// restore register holding Global Interrupt Flag

	return ms;
}

/* returns true if the specified number of milliseconds
   has passed since the start time */
char TimerCheck(unsigned long msStart, unsigned int msWait)
{
	return TimerMsCur() - msStart > msWait;
}

/* pauses for the specified number of milliseconds */
void delay(unsigned int ms)
{
	unsigned long msStart;

	msStart = TimerMsCur();
	while (!TimerCheck(msStart, ms))
		;
}

/* this interrupt happens every ms,
   when TCNT0 reaches OCR0 (8) */
ISR(TIMER0_COMP_vect)
{
	++s_msTimer;
}
