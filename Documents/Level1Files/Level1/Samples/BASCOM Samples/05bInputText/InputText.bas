'--------------------------------------------------
' InputText.bas
'
' This is the same as InputChar.bas except that it
' continues telling you what you typed in until an 'x'
' has been entered.  Then it quits.
'
' Refer to the BASCOM help for details about all key
' words, below (bold blue)
'--------------------------------------------------
'
$regfile = "m16def.dat"                                     'define chip to use
$crystal = 8000000                                          'define used crystal
$baud = 19200                                               'define communications rate
'
Dim Cmd As Byte

Ddrb.4 = 1
Portb.4 = 1                                                 ' make LED port and output and high level

Print "Type any character:"

Do
    While Ischarwaiting() = 0
        Waitms 200
        Portb.4 = Not Portb.4                               ' flash LED while waiting for character.
    Wend

    Cmd = Inkey()

    If Cmd <> Asc( "x") Then
        Print "You typed: " ; Chr(cmd) ; " Try again!"
    End If

Loop Until Cmd = Asc( "x")

Print
Print "Goodbye!"
'
End