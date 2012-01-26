'--------------------------------------------------
' ADCPrint.bas
'
' demonstrates setting up the ADC and reading a value.
' there are 8 ADC ports on the ARC 1.1. board.  Five have
' been brought out to headers: 0, 1, 2, 3 and 7 (from left to right)
'
' In this sample we read and print the value of the left floor sensor
'--------------------------------------------------
'
$regfile = "m16def.dat"                                     'define chip to use
$crystal = 8000000                                          'define used crystal
$baud = 19200                                               'define communications rate
'
Config Adc = Single , Prescaler = Auto                      ' leave everything else at defaults
Start Adc

Print "SRS Robot ADC sample"
Do
    Print "ADC 1 = " ; Getadc(1)
    Waitms 200
Loop
'
End
'
' Extensions:
'  Change program to print out all five ADC values
'  Add flashing LED
'  Add whisker information (hint, look at whisker programs)
'  Use "const" and "alias" to give the ADC channels sensible names
'    e.g. const Left_floor = 1 