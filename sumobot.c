/*
	Authors: Davide Berdin, Till Riemer
*/

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/version.h>
#include <string.h>

#include "servos.h"
#include "timer.h"
#include "LCD_driver.h"
#include "lcd_functions.h"
#include "sumobot.h"
#include "IRemettitor.h"
#include "IRdetector.h"
#include "QTISensor.h"

void setMotorSpeed(int, int);

int main(void){
	InitSumobot();
	sei(); // enable interrupts
	
	// IR manual calibration
	//int L_IR_REF = 250;
	//int R_IR_REF = 250;

	
	//wait 5 seconds before fighting
	delay(5000);
	
	LCD_puts("Start!!!",0);


	while(1)
	{
		stop();
		/*moveFwd();
		delay(2000);
		moveBwd();
		delay(2000);
		moveLeft();
		delay(2000);
		moveRight();
		delay(2000);*/
	}	

	return 0;
}

void InitSumobot(void){
	LCD_Init();
	motorInit();
//	IRsensorInit();
//	IRemettitorInit();
//	IRdetectorInit();
//	QTISensorInit();
}
/*
int readLeftIR(void){
	// switch on green LED
	PORTB |= (1 << DD7);	// OC2A
	
	// switch off green LED
	PORTB &= ~(1 << DD4);	// OC0A
	
	ADC_init(leftIR);
	
	return ADC_read();
}

int readRightIR(void){
	// switch on red LED
	PORTB |= (1 << DD7);	// OC2A
	
	// switch off red LED
	PORTB &= ~(1 << DD4);	// OC0A
	
	ADC_init(rightIR);
	
	return ADC_read();	
}*/

void turnRightOverLine(void){
	moveBwd();
	delay(500);
	moveRight();	// turn right
	delay(500);
	moveFwd();	// go forward
}

void turnLeftOverLine(void){
	moveBwd(); // go back
	delay(500);
	moveLeft(); // turn left
	delay(500);
	moveFwd();	// go forward	
}

/* sets up the Analog-to-Digital Converter (ADC) */
void InitADC()
{
	/* we left-adjust so that we get the 8 most-significant bits
	   in ADCH; use 5V input to AREF for reference voltage
	   (only option since 5V is applied there);
	   divide microcontroller clock by 64 to get 128 kHz ADC clock */
	ADMUX = 0x20;	// 00 1 00000 (Aref, L-shift, [ADC0]) 						-> correct?
	ADCSRA = 0x86;	// 1000 0110 (enable, no auto trigger, no interrupt, clk/64)
}

/* reads the specified ADC channel (0-7) and returns its value (0-255) */
unsigned char GetADC(char adc)
{
	ADMUX &= ~0x1F;		// clear channel selection (low 5 bits) -> all MUX get 0
	ADMUX |= adc;		// select specified channel

	SetBit(ADCSRA, ADSC);	// ADC start conversion
		/* (this appears to clear ADIF) */

	/* wait for conversion to complete */
	while (bit_is_clear(ADCSRA, ADIF))
		;

	return (unsigned char) ADCH; // high part returned
}
