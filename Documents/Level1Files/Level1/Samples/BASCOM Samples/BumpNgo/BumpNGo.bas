'--------------------------------------------------
' BumpNGo.bas
'
' This sample demonstrates the use of "const" and "alias" keywords
' to make code more readable.  It also uses the include file "arc_10.bas"
' to pre-define all the common ARC 1.1 i/o ports.  Finally, it shows how to
' set two or more I/O bits in one statement (more efficient).
'--------------------------------------------------
'
$regfile = "m16def.dat"                                     'define chip to use
$crystal = 8000000                                          'define used crystal
$baud = 19200                                               'define communications rate
'
$include "ARC_11.bas"

Call Initialize_pwm
Config Adc = Single , Prescaler = Auto
start adc

Ddrb.4 = 1
Porta = &H81                                                ' Enable pull-up on pin 0 and 7
'
' Wait until one or the other whisker is closed
'
While Left_whisker = 1 And Right_whisker = 1
   Waitms 200
   Prog_led = Not Prog_led
Wend

Dim State As Byte , Timeout As Byte
Const Drive = 0
Const Avoidleft = 1
Const Avoidright = 2
Const Backup = 3
'
' Below is a simple state machine.  The sensor input determines the state which
' determines the motor power values.
'
' The timer sets the state to normal (drive) when the time goes to zero, except
' when backing up.  Then it turns a little so the robot doesn't get stuck bumping
' headlong into a wall.
'
Do
   Print "Left " ; Getadc(left_floor) ; " Right " ; Getadc(right_floor)
   Select Case State
   Case Drive
      Call Set_left_motor_pwm(128)
      Call Set_right_motor_pwm(128)

   Case Avoidleft
      Call Set_right_motor_pwm( -128)

   Case Avoidright
      Call Set_left_motor_pwm( -128)

   Case Backup
      Call Set_right_motor_pwm( -128)
      Call Set_left_motor_pwm( -128)

   End Select

   If Timeout <> 0 Then
      Timeout = Timeout - 1
      If Timeout = 0 Then
         If State = Backup Then
            State = Avoidleft
            Timeout = 1
         Else
            State = Drive
         End If
      End If
   End If

   If Left_whisker = 0 Then
      State = Avoidright
      Timeout = Timeout + 1
   End If

   If Right_whisker = 0 Then
      State = State Or Avoidleft
      Timeout = Timeout + 1
   End If

   Prog_led = Not Prog_led                                  ' short cut to flipping the state of the LED
   Waitms 200
Loop
'
End
$include "ARC_10_PWM.bas"