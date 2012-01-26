'---------------------------------------------------
' ARC_10_PWM.BAS
'
' Subroutines to manage the hardware bits of the AVR Timer1 PWM system.
' The passed value is signed and one bit larger than the selected PWM value.
' for 8 bit PWM, pass a 9 bit signed number (e.g. -255 = full reverse,
' 0 = stop, 255 =full forward)
'
' Initialize_pwm() fixed at 8 bit PWM @2khz.
'
' USAGE: See ADC_PWM.BAS for an example of how to use this file.
' At the top of your application:
'
'   $include "ARC_10.bas"   ' hardware bits and subroutine declarations
'
'   call initialize_pwm     ' Initialize PWM hardware
'   ...
'   call set_left_motor_pwm(128)    ' set left forward 1/2 speed.
'   call set_left_motor_pwm(-255)   ' set left full reverse
'   ...
'   $include "ARC_10_PWM.BAS"
'
'
Sub Set_left_motor_pwm(byval Pwm As Integer)
    If Pwm >= 0 Then
        Left_motor_dir = Motor_forward
        Tccr1a.com1b0 = 0                                   ' Switch to normal PWM
    Else
        Left_motor_dir = Motor_reverse
        Tccr1a.com1b0 = 1
        Pwm = -pwm                                          'inverted PWM
    End If
    if pwm > 255 then pwm = 255                            ' Clip output to max
    Left_motor_pwm = Pwm
End Sub
'
Sub Set_right_motor_pwm(byval Pwm As Integer)
    If Pwm >= 0 Then
        Right_motor_dir = Motor_forward
        Tccr1a.com1a0 = 0                                   ' Switch to normal PWM
    Else
        Right_motor_dir = Motor_reverse
        Tccr1a.com1a0 = 1
        Pwm = -pwm                                          'inverted PWM
    End If
    if pwm > 255 then pwm = 255
    Right_motor_pwm = Pwm
End Sub
'
' Eight bit PWM @ 8 mhz with divide by 8 prescale = ~2khz.  With the mega163
' there is a double bit that allows 8 bit PWM to operate near 32khz.  You can
' select slower PWM rates with a different pre-scale.  Read the Chip manual.
'
Sub Initialize_pwm
    Config Timer1 = Pwm , Pwm = 8 , Compare A Pwm = Clear Down , Compare B Pwm = Clear Down , Prescale = 8
    Config Pinc.3 = Output
    Config Pinc.4 = Output
    Config Pind.4 = Output
    Config Pind.5 = Output
    Right_motor_pwm = 0
    Left_motor_pwm = 0
End Sub