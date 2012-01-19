#include <avr/io.h>
#include <avr/interrupt.h>
#include "LCD_driver.h"
#include "lcd_functions.h"

volatile int temp;

ISR (TIMER0_COMP_vect){
	temp++;
}

void delayMs(int s){
	/*SREG &= ~(1 << SREG_I); //disable interrupt 
	temp=0;
	// configure Timer 0
  	TCCR0A |= (1<<CS01) | (1<<CS00) | (1<<WGM01); // Prescaler 1024
	OCR0A = ((F_CPU / 1024)) - 1; 
	//TCCR0A &= ~(1<<CS02) & ~(1<<WGM00); 

  	// allow Overflow Interrupt
  	TIMSK0 |= (1<<TOIE0);
	SREG|=1<<SREG_I; // global enable interrupts

	do{}
	while(temp<s);

	temp=0;
	TCCR0A = 0;*/

	TCCR0A = 0;								//Stop Timer
	TCCR0A |= (1<<WGM01);    				//CTC Mode
	TIMSK0  |= (1 << OCIE0A); 				//Enable Interrupt
	sei();                   				//Enable Global Interrupts
	OCR0A  =   ((F_CPU / 1024)) - 1;        //For Prescaler 1024 it is 1s
	temp=0;                  				//Zero the sec
	TCCR0A |= ((1 << CS02)|(1 << CS00));	//Start Pre 1024
	do{}
	while(temp<s);           				//Loop to wait for 3 sec.
	TCCR0A = 0; 							//Stop Timer
	temp = 0;                				//Zero the sec 
}
