'--------------------------------------------------
' InputChar.bas
'
' Demonstrate waiting for a character, then printing out
' the typed value.
'
' Note the loop waiting for the character flashing the LED.
'--------------------------------------------------
'
$regfile = "m16def.dat"                                     'define chip to use
$crystal = 8000000                                          'define used crystal
$baud = 19200                                               'define communications rate
'
Dim Cmd As Byte
Ddrb.4 = 1                                                  ' Make portb.4 an output
Portb.4 = 1                                                 ' Set LED off

Print "Type any character:";

While Ischarwaiting() = 0
    Waitms 200
    Portb.4 = Not Portb.4                                   ' clever way to toggle the LED
Wend

Cmd = Inkey()

Print
Print "You typed: " ; Chr(cmd)
'
End