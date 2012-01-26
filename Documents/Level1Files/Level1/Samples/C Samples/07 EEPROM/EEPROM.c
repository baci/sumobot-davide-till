/*	EEPROM.c
*
*	This code demonstrates how to read from and write to the EEPROM memory.
*
*	When a serial command is received, the program reacts as follows:
*
*	Command         Action
*	-------         ------
*	L or l          read "light" value on both light sensors
*	                   and store to EEPROM
*	D or d          read "dark" value on both light sensors
*	                   and store to EEPROM
*	?               show the values
*
*	The data is stored in the following EEPROM addresses:
*
*	EEPROM Address       Use
*	--------------       ---------------------
*	     0               Left "light" reading
*	     1               Right "light" reading
*	     2               Left "dark" reading
*	     3               Right "dark" reading
*
*	A more extensive program could use the stored values when following a line,
*	for example.
*
*	This program uses two macros to assist in EEPROM operations:
*		EEPROMWriteByte(addr, byte) -- this writes the
*			given byte value at the specified address.
*		EEPROMReadByte(addr) -- this reads and returns
*			the byte value at the specified address.
*
*	Those macros call functions provided by the GCC compiler:
*		eeprom_write_byte(addr, byte)
*		eeprom_read_byte(addr)
*	See GCC documentation for additional EEPROM functions.
*/

#include <stdio.h>
#include <avr/io.h>
#include "Util.h"

/* define all the stuff doing I/O here; there are two reasons for this:
   1) it makes the code easier to read/understand
   2) it makes it easy to make changes later, e.g. using a different port */
#define SetupLineSensors()	(DDRA &= ~0x0A)	// clear bits DDA1/3 to set as input
#define leftLineSensor		1
#define rightLineSensor		3

/* EEPROM addresses */
enum
{
	leftLight = 0,
	rightLight,
	leftDark,
	rightDark,
};

int main()
{
	unsigned char adcL, adcR;
	char sz[48];

	InitComm();
	InitADC();
	SetupLineSensors();
	Print("EEPROM write/read example\r\n");

	while (1)
	{
		switch (InKey())
		{
		case 'L':
		case 'l':
			adcL = GetADC(leftLineSensor);
			adcR = GetADC(rightLineSensor);
			EEPROMWriteByte(leftLight, adcL);
			EEPROMWriteByte(rightLight, adcR);
			sprintf(sz, "metered light: %d %d\r\n", adcL, adcR);
			Print(sz);
			break;
		case 'D':
		case 'd':
			adcL = GetADC(leftLineSensor);
			adcR = GetADC(rightLineSensor);
			EEPROMWriteByte(leftDark, adcL);
			EEPROMWriteByte(rightDark, adcR);
			sprintf(sz, "metered dark: %d %d\r\n", adcL, adcR);
			Print(sz);
			break;
		case '?':
			Print("        left right\r\n");
			adcL = EEPROMReadByte(leftLight);
			adcR = EEPROMReadByte(rightLight);
			sprintf(sz, "light   %3d   %3d\r\n", adcL, adcR);
			Print(sz);
			adcL = EEPROMReadByte(leftDark);
			adcR = EEPROMReadByte(rightDark);
			sprintf(sz, "dark    %3d   %3d\r\n", adcL, adcR);
			Print(sz);
			break;
		}
	}

	return 0;
}
