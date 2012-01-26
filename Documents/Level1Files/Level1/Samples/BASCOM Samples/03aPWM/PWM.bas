'--------------------------------------------------------------------------
'                PWM.bas
'
'Demonstrate PWM.
'
' The PWM routines are provided as a "black box".  If you want, you can read about
' timer 1 hardware in the mega16 data sheet.
'
' The file "ARC_11.BAS" provides handy declarations and aliases for common hardware
' bits and pieces of the ARC control board.  It needs to be included first.
'
' The file "ARC_10_PWM.BAS" has the actual code for setting up and controlling
' timer 1 as a PWM device.  It also manages the direction of the motors.
'
' PWM values are from -255 to 255 (-100% to +100% power).  PWM values will be roughly
' correlated to motor speed.  So, 128 will be about 1/2 the speed of 255 and so forth.
'
'--------------------------------------------------------------------------
$regfile = "m16def.dat"
$crystal = 8000000
$include "ARC_11.bas"

Call Initialize_pwm                                         ' Initialize PWM hardware

Wait 5
Call Set_left_motor_pwm(128)                                ' run motors at 1/2 speed
Call Set_right_motor_pwm(128)
Wait 5
Call Set_left_motor_pwm(0)
Call Set_right_motor_pwm(0)

End
'
$include "ARC_10_PWM.bas"

'
' Extensions:
'   Flash the LED once a second for five seconds before starting
'   make robot wait until a whisker is pressed.
'   Make robot stop when whisker is pressed
'   Set left motor PWM to -100.  What happens?
'   make robot go forward 10 inches, turn left 90 degrees.
'   Make robot do previous extension 4 times.
'   Make the LED flash while doing the other extensions.
'
'