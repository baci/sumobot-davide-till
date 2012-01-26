'--------------------------------------------------
' Remote Control.bas
'
' Use the keypad of the terminal to control the robot.  8 = forward, and so forth
' print out the values of the left/right floor sensors with each key press
' check out following a line, for example.
'
'--------------------------------------------------
'
$regfile = "m16def.dat"                                     'define chip to use
$crystal = 8000000                                          'define used crystal
$baud = 19200                                               'define communications rate
'
$include "ARC_11.bas"

Call Initialize_pwm
Config Adc = Single , Prescaler = Auto
Start Adc

Ddrb.4 = 1
Porta = &H81                                                ' Enable pull-up on pin 0 and 7
Dim Cmd As Byte

Print "Remote Control"

Do
    If Ischarwaiting() = 1 Then
        Print "Left " ; Getadc(left_floor) ; " Right " ; Getadc(right_floor)
        Cmd = Inkey()
        Select Case Cmd
        Case "9"
            Call Set_left_motor_pwm(128)
            Call Set_right_motor_pwm(96)
        Case "8"
            Call Set_left_motor_pwm(128)
            Call Set_right_motor_pwm(128)
        Case "7"
            Call Set_left_motor_pwm(96)
            Call Set_right_motor_pwm(128)
        Case "6"
            Call Set_left_motor_pwm(96)
            Call Set_right_motor_pwm( -96)
        Case "4"
            Call Set_left_motor_pwm( -96)
            Call Set_right_motor_pwm(96)
        Case "3"
            Call Set_left_motor_pwm( -128)
            Call Set_right_motor_pwm( -96)
        Case "2"
            Call Set_left_motor_pwm( -128)
            Call Set_right_motor_pwm( -128)
        Case "1"
            Call Set_left_motor_pwm( -96)
            Call Set_right_motor_pwm( -128)
        End Select
    End If

    Prog_led = Not Prog_led                                 ' short cut to flipping the state of the LED

    Waitms 200

    Call Set_left_motor_pwm(0)
    Call Set_right_motor_pwm(0)
Loop
'
End
$include "ARC_10_PWM.bas"
