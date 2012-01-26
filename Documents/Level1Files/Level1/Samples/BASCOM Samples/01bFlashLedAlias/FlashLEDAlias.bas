'--------------------------------------------------------------------------
'                  Alias
'
'This is "FlashLED.bas" but using an ALIAS and CONST commands to make
'the code more readable.
'
'--------------------------------------------------------------------------
$crystal = 8000000
$regfile = "m16def.dat"
'
' Define consts (aliases) to make code more readable
'
Const Off = 1                                               ' Inverted logic: 1 = off
Const On = 0
Prog_led Alias Portb.4                                      ' Give Led a nicer name
'
Ddrb.4 = 1                                                  'make LED pin an output
Prog_led = Off                                              ' initialize off
'
Do
   Prog_led = On                                            ' Turn LED on
   Waitms 200
   Prog_led = Off
   Waitms 800
Loop
'
End