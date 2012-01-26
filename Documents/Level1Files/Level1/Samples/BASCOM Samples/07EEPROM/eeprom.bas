'
' This code demonstrates how to read from and write to the EEPROM memory.
'
' When a serial command is received, the program reacts as follows:
'
' Command         Action
' -------         ------
' L or l          read "light" value on both light sensors and store to EEPROM
' D or d          read "dark" value on both light sensors and store to EEPROM
' ?               show the values
'
' The data is stored in the following EEPROM addresses:
'
' EEPROM Address       Use
' --------------       ---------------------
'      0               Left "light" reading
'      1               Right "light" reading
'      2               Left "dark" reading
'      3               Right "dark" reading
'
' A more extensive program could use the stored values when following a line,
' for example.
'
$regfile = "m16def.dat"                                     'define chip to use
$crystal = 8000000                                          'define used crystal
$baud = 38400                                              'define communications rate
$eeprom
'
Dim cmd As Byte

' Simply declare variables as eram to put in EEPRROM.

dim LeftLight as eram word, leftdark as eram  word
dim rightlight as eram word, rightdark as eram  word
dim leftval as word, rightval as word
declare sub printnum(num as word)

' Enable ADC

Config Adc = Single , Prescaler = Auto
Start Adc

' make LED an output

ddrb.4 = 1
print "SRS Robot EEPROM sample"

Do
    If Ischarwaiting() = 1 Then
        Cmd = Inkey()
        if cmd > "a" and cmd < "z" then
            cmd.5 = 0      ' convert to upper case
        end if

        print chr(cmd);

        leftval = GetADC(1)
        rightval = GetAdc(3)

        Select Case Cmd
        Case "D"
            print " - Reading dark"
            LeftDark = leftval
            RightDark = rightval
        Case "L"
            print " - Reading light"
            Leftlight = leftval
            Rightlight = rightval
        case "?"
            print
            print "        Left  Right"

            leftval = Leftlight
            rightval = rightlight
            print "Light";
            call printnum(leftval)
            call printnum(rightval)
            print
            leftval = leftdark
            rightval = rightdark
            print " dark";
            call printnum(leftval)
            call printnum(rightval)
            print
        End Select
    End If

    ' Flash the LED each loop

    portb.4 = Not portb.4

    Waitms 200

Loop

End
'
' formatting routine for a padded 6 wide #
'
sub PrintNum(num as word)
    dim var as string * 7
    var = format(str(num), "       ")
    print var ;
end SUB