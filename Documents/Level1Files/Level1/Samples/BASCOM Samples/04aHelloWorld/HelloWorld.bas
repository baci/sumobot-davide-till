'--------------------------------------------------
' hello_world.bas
'
' demonstrates simple serial output
'--------------------------------------------------
'
$regfile = "m16def.dat"                                     'define chip to use
$crystal = 8000000                                          'define used crystal
$baud = 19200                                               'define communications rate
'
Print "Hello World"
'
End