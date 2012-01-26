'--------------------------------------------------------------------------
'                  Flash an LED
'
' This code blinks the PROG LED on the uARC controller board
' PROG is connected to MOSI, or portb bit 5. (bits start counting at zero,
' hence "4" used, below.
'
' On the uARC the LEDs are "ON" when the output is low.  Hence the inverted
' logic, below.
'
' Set the corresponding bit to one in the special function register (SFR) DDRB to
' make a bit an output.  Set the value of the output by manipulating the SFR
' PORTB.
'
' NOTE: Read "Config Port" section of help file for more details.
'--------------------------------------------------------------------------
$crystal = 8000000                                          'tell compiler what the CPU frequency is
$regfile = "m16def.dat"

Ddrb.4 = 1                                                  'make LED pin an output
Portb.4 = 1                                                 ' initialize off

Do
    Portb.4 = 0                                             ' Turn LED on
    Waitms 200
    Portb.4 = 1
    Waitms 800
Loop

End