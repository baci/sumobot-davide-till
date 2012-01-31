#ifndef _SENSORS
#define _SENSORS

#define LEFT_LINE_POWER		PB4
#define RIGHT_LINE_POWER	PB2
#define LEFT_LINE_IN		PB7
#define RIGHT_LINE_IN		PB3

/* macros to set and clear bits in registers */
#define SetBit(sfr, bit) ((sfr) |= (1 << (bit)))
#define ClearBit(sfr, bit) ((sfr) &= ~(1 << (bit)))

void EmitterInit();
void DetectorInit();

/* sets up the Analog-to-Digital Converter (ADC) */
void InitADC();
/* reads the specified ADC channel (0-7) and returns its value (0-255) */
unsigned char GetADC(char adc);



#endif
