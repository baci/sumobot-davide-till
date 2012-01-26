/*	Util.c
*
*	Useful utilities for Workshop Code examples.
*
*	Revisions:
*		6-25-06		avr-gcc change from signal.h/INTERRUPT() to interrupt.h/ISR()
*		10-22-05	ADC: ADC clock at 128 kHz
*		10-22-05	motor: check/fix PWM value out of range
*		11-03-05	motor: disconnect OC1A/B output when speed=0
*		11-05-05	motor: set dir fwd when disconnect OC1A/B
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Util.h"

#include <stdio.h>

/* waits (pauses) for ms milliseconds (assumes internal clock at 8MHz) */
void WaitMs(unsigned int ms)
{
	int i;

	while (ms-- > 0)
	{
		/* 8192 (8k) clock cycles for 1ms; each time through loop
		   is 5 cycles (for loop control + nop) */
		for (i = 0; i < 1638; ++i)
			asm("nop");
	}
}

/*
*	motor control via H-bridge IC and PWM using Timer/Counter 1
*/

/*	A motor will turn when its inputs differ. The direction depends on which
	input is positive and which is ground. (If both are the same, the motor
	will stop moving!)

	When we go forward, we send a low (0) signal to the H-bridge for direction.
	We also send a PWM (pulse width modulation) signal to the H-bridge. When
	that signal is high, the motor turns; when it is low, the motor stops.

	So, we send a pulse that's high for the width specified, then drop it
	low for the remainder of the pulse frame, returning it to high at the
	end of that frame, when the next pulse starts.
	 ___   ___   ___
	|   |_|   |_|   |_		wave for 75% full speed forward

	When we go in reverse, we send a high (1) signal to the H-bridge for
	direction. In this case, the motor turns when the PWM signal is low,
	and stops when it's high.

	In this case, we sent a pulse that's low for the width specified, then
	raise it for the remainder of the frame.
	     _     _     _
	 ___| |___| |___| |		wave for 75% full speed in reverse


	The code below sets up Timer/Counter 1 as follows:
	- Waveform Generation Mode: Fast PWM (0101) -- counting from 0-255
			(then wrapping)
	- Compare Output Mode: this determines how the signals on OC1A/PD5
				(right motor) and OC1B/PD4 (left motor) change when the
				counter matches the value in OCR1A or OCR1B (our "PWM"
				values indicating the desired speeds of the motors)
				and at the maximum count value (255, end of frame).
			clear on match, set at TOP (10) [for forward, see above], or
			set on match, clear at TOP (11) [for reverse, see above]
	- Clock Select: clkIO/64 (011)
			8 Mhz / 64 (prescaler) = 128 kHz for counter
			128 kHz / 256 (clock pulses/frame) = 512 Hz for PWM [frame ~2 ms]

	See the Atmel ATmega16 data sheet for details on these settings and how
	they correspond to bits in TCCR1A/B (Timer/Counter 1 Control Registers).
*/

/* define all the stuff doing I/O here; there are two reasons for this:
   1) it makes the code easier to read/understand
   2) it makes it easy to make changes later, e.g. using a different port */

/* PC3 controls left motor direction (forward/reverse) */
#define SetupLDir()	SetBit(DDRC, DDC3)
/* PC4 controls right motor direction (forward/reverse) */
#define SetupRDir()	SetBit(DDRC, DDC4)
/* PWM output on PD4/OC1B for left motor, PD5/OC1A for right motor; these
   pins are connected to H-bridge; we just need to send signals */
#define SetupLPWM()	SetBit(DDRD, DDD4)
#define SetupRPWM()	SetBit(DDRD, DDD5)
/* we compare to OCR1A/B for R/L motor speeds */
#define lPWM		OCR1B
#define rPWM		OCR1A
/* set direction (input to H-bridge) and wave output mode */
#define LFwd()		( ClearBit(PORTC, PC3),   SetBit(TCCR1A, COM1B1), ClearBit(TCCR1A, COM1B0) )
#define LRev()		(   SetBit(PORTC, PC3),   SetBit(TCCR1A, COM1B1),   SetBit(TCCR1A, COM1B0) )
#define LStop()		( ClearBit(PORTC, PC3), ClearBit(TCCR1A, COM1B1), ClearBit(TCCR1A, COM1B0) )
#define RFwd()		( ClearBit(PORTC, PC4),   SetBit(TCCR1A, COM1A1), ClearBit(TCCR1A, COM1A0) )
#define RRev()		(   SetBit(PORTC, PC4),   SetBit(TCCR1A, COM1A1),   SetBit(TCCR1A, COM1A0) )
#define RStop()		( ClearBit(PORTC, PC4), ClearBit(TCCR1A, COM1A1), ClearBit(TCCR1A, COM1A0) )

/* sets up microprocessor for PWM control of motors */
void InitPWM()
{
	/* set up ports */
	SetupLDir();
	SetupRDir();
	SetupLPWM();
	SetupRPWM();

	TCNT1 = 0;

	/* see comment above for info on PWM initialization */
	/* start with motors disconnected from Timer/Counter output */
	TCCR1A = 0x01;	// 00 00 00 01
	TCCR1B = 0x0B;	// 000 01 011 (512 Hz) /64 C

	/* OCR1A/B are the values that the timer is compared to; a match will
	   cause the output to change; small values mean the motor runs for a
	   short period (slower); larger values are longer times (faster)*/
	lPWM = rPWM = 0;	// (value is irrelevant since outputs are disconnected)
}

/* pwm values can range from -255 (full-speed reverse)
   to 255 (full-speed forward), with 0 indicating a stop */
void SetLeftMotorPWM(int pwm)
{
	if (pwm == 0)
	{
		LStop();
	}
	else
	{
		if (pwm >= 0)
		{
			LFwd();
		}
		else
		{
			LRev();
			pwm = -pwm;
		}
		if (pwm > 255)
			pwm = 255;
		lPWM = pwm;		// set width for PWM
	}
}

/* pwm values can range from -255 (full-speed reverse)
   to 255 (full-speed forward), with 0 indicating a stop */
void SetRightMotorPWM(int pwm)
{
	if (pwm == 0)
	{
		RStop();
	}
	else
	{
		if (pwm >= 0)
		{
			RFwd();
		}
		else
		{
			RRev();
			pwm = -pwm;
		}
		if (pwm > 255)
			pwm = 255;
		rPWM = pwm;		// set width for PWM
	}
}

/*
*	Serial Communications
*/

static void TxCh(char ch);

/* initializes serial transmit and receive at 19.2k baud, 8-N-1 */
void InitComm()
{
	SetBit(UCSRB, TXEN);	// enable transmit
	SetBit(UCSRB, RXEN);	// enable receive

	/* UBRR = 25 gives us 19.2k baud */
	UBRRH = 0;
	UBRRL = 25;		// change to 12 for 38.4k baud

	/* set UCSRC (not UBRRH), asynch, no parity, 1 stop bit, 8 data bits */
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
}

/* transmits text via serial connection */
void Print(const char *psz)
{
	const char *pch;

	for (pch = psz; *pch != 0; ++pch)
		TxCh(*pch);
}

/* helper function: transmits a single character (after waiting
   for any prev transmission to complete) */
static void TxCh(char ch)
{
	while (bit_is_clear(UCSRA, UDRE))
		;
	UDR = ch;
}

/* returns true if there is unread data in the receive buffer */
char IsCharWaiting()
{
	return bit_is_set(UCSRA, RXC);
}

/* returns the (next) character in the receive buffer (0 if none) */
char InKey()
{
	if (bit_is_clear(UCSRA, RXC))
		return 0;
	return UDR;
}

/*
*	Analog-to-Digital Converter (ADC)
*/

/* sets up the Analog-to-Digital Converter (ADC) */
void InitADC()
{
	/* we left-adjust so that we get the 8 most-significant bits
	   in ADCH; use 5V input to AREF for reference voltage
	   (only option since 5V is applied there);
	   divide microcontroller clock by 64 to get 128 kHz ADC clock */
	ADMUX = 0x20;	// 00 1 00000 (Aref, L-shift, [ADC0])
	ADCSRA = 0x86;	// 1000 0110 (enable, no auto trigger, no interrupt, clk/64)
}

/* reads the specified ADC channel (0-7) and returns its value (0-255) */
unsigned char GetADC(char adc)
{
	ADMUX &= ~0x1F;		// clear channel selection (low 5 bits)
	ADMUX |= adc;		// select specified channel

	SetBit(ADCSRA, ADSC);	// ADC start conversion
		/* (this appears to clear ADIF) */

	/* wait for conversion to complete */
	while (bit_is_clear(ADCSRA, ADIF))
		;

	return (unsigned char) ADCH;
}

/*
*	Timer/Counter 0 (for timing)
*/

/* this counts the number of milliseconds
   since Timer/Counter 0 was initialized
   (volatile to make sure loops work!) */
static volatile unsigned long s_msTimer = 0;

/* sets up Timer/Counter 0 */
void InitTimer()
{
	/* start counting at 0 */
	TCNT0 = 0;

	/* clock rate = 8Mhz/1024 = 8kHz
	   CTC (Clear Timer on Compare Match) waveform generation mode
	   OCR0 (top/clear value) = 8
	   for match every 1ms
	*/
	TCCR0 = 0x0D;	// 0000 1101
	OCR0 = 8;

	/* set Compare Match Interrupt Enable (for timer/counter 0) */
	SetBit(TIMSK, OCIE0);

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
void TimerWait(unsigned int ms)
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
