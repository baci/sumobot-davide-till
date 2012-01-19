//***************************************************************************
//
//  File........: button.c
//
//  Author(s)...: ATMEL Norway
//
//  Target(s)...: ATmega169
//
//  Compiler....: IAR EWAAVR 4.20a
//
//  Description.: AVR Butterfly button handling routines
//
//  Revisions...: 1.0
//
//  YYYYMMDD - VER. - COMMENT                                       - SIGN.
//
//  20030116 - 1.0  - Created                                       - KS
//
//***************************************************************************

#include <inavr.h>
#include "iom169.h"
#include "main.h"
#include "button.h"
#include "timer0.h"

extern char gPowerSaveTimer;

unsigned char gButtonTimeout = FALSE;

char KEY = NULL;
char KEY_VALID = FALSE;

char CountdownTimerHandle;

/*****************************************************************************
*
*   Function name : Button_Init
*
*   Returns :       None
*
*   Parameters :    None
*
*   Purpose :       Initializes the five button pin
*
*****************************************************************************/
void Button_Init(void)
{
    // Init port pins
    cbi(DDRB,7);
    cbi(DDRB,6);
    cbi(DDRB,4);
    PORTB |= PINB_MASK;
    DDRE = 0x00;
    PORTE |= PINE_MASK;

    // Enable pin change interrupt on PORTB and PORTE
	PCMSK0 = PINE_MASK;
	PCMSK1 = PINB_MASK;
	EIFR = (1<<PCIF0)|(1<<PCIF1);
	EIMSK = (1<<PCIE0)|(1<<PCIE1);
	
	CountdownTimerHandle = Timer0_AllocateCountdownTimer();
}


#pragma vector = PCINT0_vect
__interrupt void PCINT0_interrupt(void)
{
    PinChangeInterrupt();
}

#pragma vector = PCINT1_vect
__interrupt void PCINT1_interrupt(void)
{
    PinChangeInterrupt();
}


/*****************************************************************************
*
*   Function name : PinChangeInterrupt
*
*   Returns :       None
*
*   Parameters :    None
*
*   Purpose :       Check status on the joystick
*
*****************************************************************************/
void PinChangeInterrupt(void)
{
    char buttons;

    char key;

/*
    Read the buttons:

    Bit             7   6   5   4   3   2   1   0
    ---------------------------------------------
    PORTB           B   A       O
    PORTE                           D   C
    ---------------------------------------------
    PORTB | PORTE   B   A       O   D   C
    =============================================
*/


    buttons = (~PINB) & PINB_MASK;
    buttons |= (~PINE) & PINE_MASK;

    // Output virtual keys
    if (buttons & (1<<BUTTON_A))
        key = KEY_PLUS;
    else if (buttons & (1<<BUTTON_B))
        key = KEY_MINUS;
    else if (buttons & (1<<BUTTON_C))
        key = KEY_PREV;
    else if (buttons & (1<<BUTTON_D))
        key = KEY_NEXT;
    else if (buttons & (1<<BUTTON_O))
        key = KEY_ENTER;
    else
        key = KEY_NULL;

    
    if(key != KEY_NULL)
    {
        if(gButtonTimeout)  // gButtonTimeout is set in the LCD_SOF_interrupt in LCD_driver.c
        {
            if (!KEY_VALID)
            {
                KEY = key;          // Store key in global key buffer
                KEY_VALID = TRUE;
            }

         gButtonTimeout = FALSE;
    
        }
    }
    
    EIFR = (1<<PCIF1) | (1<<PCIF0);     // Delete pin change interrupt flags

    gPowerSaveTimer = 0;                // Reset the Auto Power Down timer
    
}


/*****************************************************************************
*
*   Function name : getkey
*
*   Returns :       The valid key
*
*   Parameters :    None
*
*   Purpose :       Get the valid key 
*
*****************************************************************************/
char getkey(void)
{
    char k;

    __disable_interrupt();

    if (KEY_VALID)              // Check for unread key in buffer
    {
        k = KEY;
        KEY_VALID = FALSE;
    }
    else
        k = KEY_NULL;           // No key stroke available

    __enable_interrupt();

    return k;
}