'--------------------------------------------------------------------------
' ARC 1.1 (SRS robot) Hardware definition include file
'
' Defines all board I/O pins and any peripheral definition bits that
' might be useful
'
'--------------------------------------------------------------------------
'
' ADC Channels are just numbers passed to GetADC()
'
Const Left_sharp = 0
Const Left_floor = 1
Const Center_floor = 2
Const Right_floor = 3
Const Right_sharp = 7
'
' Alternate use of the A port
'
Left_whisker Alias Pina.0
Right_whisker Alias Pina.7
'
Const Motor_forward = 0
Const Motor_reverse = 1
'
Prog_led Alias Portb.4                                      ' We enable low drive, rather than driving high
Option_a Alias Pinb.6
Option_b Alias Pinb.7

Const Led_on = 0
Const Led_off = 1
'
Right_motor_dir Alias Portc.4
Right_motor_pwm Alias Pwm1a
Right_encoder_a Alias Pind.2                                'INT0
Right_encoder_b Alias Pinc.5
Right_servo_out Alias Portc.5
'
Left_motor_dir Alias Portc.3
Left_motor_pwm Alias Pwm1b
Left_encoder_a Alias Pind.3                                 'INT1
Left_encoder_b Alias Pinc.2
Left_servo_out Alias Portc.2
'
' Library routines
'
' These routines pass parameters by value since a typical situation is to
' pass constants (e.g. 1/2 speed forward, servo position X) otherwise a global
' would have to be declared to hold the value and then the routine called.
'
Declare Sub Initialize_servo()
Declare Sub Left_servo(byval Pulse As Word)                 ' ARC_10_servo.bas
Declare Sub Right_servo(byval Pulse As Word)
'
Declare Sub Set_left_motor_pwm(byval Pwm As Integer)        'ARC_10_PWM.bas
Declare Sub Set_right_motor_pwm(byval Pwm As Integer)
Declare Sub Initialize_pwm()
'
' ARC_10_polled_encoder.bas OR ARC_10_encoder.bas
'
Declare Sub Initialize_encoder()
Declare Function Left_encoder() As Integer
Declare Function Right_encoder() As Integer
Declare Sub Doencoder()