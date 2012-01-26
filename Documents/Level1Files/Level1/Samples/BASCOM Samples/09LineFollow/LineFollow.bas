'--------------------------------------------------
' LineFollow.bas
'
' This sample demonstrates the use of "const" and "alias" keywords
' to make code more readable.  It also uses the include file "arc_10.bas"
' to pre-define all the common ARC 1.1 i/o ports.  Finally, it shows how to
' set two or more I/O bits in one statement (more efficient).
'--------------------------------------------------
'
$regfile = "m16def.dat"                                     'define chip to use
$crystal = 8000000                                          'define used crystal
$baud = 38400                                              'define communications rate
'
$include "ARC_11.bas"

Dim Led_timer As Byte, state as byte
Dim Left_sensor As Integer , Right_sensor As Integer
Dim Line_dif As Integer , Temp As Integer

Config Adc = Single , Prescaler = Auto
Start Adc

Const Drive_pwm = 160

Ddrb.4 = 1                  ' enable program LED port
Call Initialize_pwm
state = 10  ' 5 seconds @ 2hz

Do
    ' State machine to flash LED at about 2hz

    Waitms 5                                   ' 50hz update
    Incr Led_timer
    If Led_timer > 100 Then
        Prog_led = Not Prog_led                   ' short cut to flipping the state of the LED
        Led_timer = 0
        print "Left: "; Left_sensor; " Right: "; Right_sensor
        if state <> 0 then
            decr state
        end if
    End If

    Left_sensor = Getadc(left_floor)
    Right_sensor = Getadc(right_floor)

    ' Adjust the drive values to keep the sensors centered on line

    if state = 0 then
        Line_dif = Left_sensor - Right_sensor       ' +/- 1000
        line_dif = line_dif/4                       ' +/- 128
        Temp = Drive_pwm - Line_dif
        Call Set_left_motor_pwm(temp)
        Temp = Drive_pwm + Line_dif
        Call Set_right_motor_pwm(temp)
    end if
Loop

$include "ARC_10_PWM.bas"

End