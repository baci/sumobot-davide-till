/*	Util.h
*
*	Useful utilities for Workshop Code examples.
*
*	Revisions:
*		11-03-05	added <avr/eeprom.h> include
*/

#include <avr/eeprom.h>

/* macros to set and clear bits in registers */
#define SetBit(sfr, bit) ((sfr) |= (1 << (bit)))
#define ClearBit(sfr, bit) ((sfr) &= ~(1 << (bit)))

/* waits (pauses) for ms milliseconds (assumes internal clock at 8MHz) */
void WaitMs(unsigned int ms);

/* sets up microprocessor for PWM control of motors */
void InitPWM();
/* pwm values can range from -255 (full-speed reverse)
   to 255 (full-speed forward), with 0 indicating a stop */
void SetLeftMotorPWM(int pwm);
void SetRightMotorPWM(int pwm);

/* initializes serial transmit and receive at 19.2k baud, 8-N-1 */
void InitComm();
/* transmits text via serial connection */
void Print(const char *psz);
/* returns true if there is unread data in the receive buffer */
char IsCharWaiting();
/* returns the (next) character in the receive buffer (0 if none) */
char InKey();

/* sets up the Analog-to-Digital Converter (ADC) */
void InitADC();
/* reads the specified ADC channel (0-7) and returns its value (0-255) */
unsigned char GetADC(char adc);

/* initializes Timer/Counter 0 for timing */
void InitTimer();
/* returns number of ms since Timer initialized */
unsigned long TimerMsCur();
/* returns true if the specified number of milliseconds
   has passed since the start time (else returns false) */
char TimerCheck(unsigned long msStart, unsigned int msWait);
/* pauses for the specified number of milliseconds */
void TimerWait(unsigned int ms);

#define EEPROMWriteByte(addr, byte) \
			eeprom_write_byte((uint8_t *)(addr), (byte))
#define EEPROMReadByte(addr) \
			eeprom_read_byte((uint8_t *)(addr))
