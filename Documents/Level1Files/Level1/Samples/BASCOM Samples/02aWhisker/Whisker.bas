'--------------------------------------------------------------------------
'                Whisker.bas
'
'Demonstrate how to read a switch on an I/O port.
'
'Reads the state of PA0 (left input) and sets the program LED to that state.
'
'Typically, when using a PORT bit as an input for a switch, one should enable the
'internal CPU pull-up resistor and connect the switch between the pin and ground.
'When the switch is closed, the pin will read zero, when open, it will read 1.
'
'The pull-up resistor is enabled by writing a 1 to the corresponding PORT bit.
'--------------------------------------------------------------------------
$regfile = "m16def.dat"
$crystal = 8000000

Ddrb.4 = 1                                                  ' Make LED pin an output.
Ddra.0 = 0                                                  ' Make PA0 an input
Porta.0 = 1                                                 ' enable internal pull up on A input

Do
   Waitms 10                                                ' loop at 100hz (roughly)
   If Pina.0 = 1 Then
      Portb.4 = 1
   Else
      Portb.4 = 0
   End If
Loop

End

'
' Note: this program can be simplified by replacing the entire "IF/THEN/ELSE"
' with the following line:
'
' portb.4 = pina.0
'
'Extensions:
'  Use aliases
'  Make either whisker closure light the LED