'--------------------------------------------------
' DeadReckoning.bas
'
' This sample demonstrates using a FLASH based table of information and
' a rudimentary state machine for accessing the information.  Note this isn't
' like C with a proper structure in memory.  Instead, the layout of the DATA
' and the cooresponding READ must match.  No help from the compiler here!
'
' CHIP OPTIONS: Frame:32, SoftStack:16, HardStack:64
'
'--------------------------------------------------
'
$regfile = "m16def.dat"                                     'define chip to use
$crystal = 8000000                                          'define used crystal
$baud = 38400                                              'define communications rate
'
$include "ARC_11.bas"

declare function TimerMSCur() as word
declare function TimerCheck(byval start_time as word, byval delay_time as word) as byte

Dim Leftpwm As integer , RightPwm As integer, Duration As word
dim StateTimer as word,  state as byte

Config Timer0 = Timer , Prescale = 64   ' 125 khz
Const Timer0_reload = 256 - 125          ' Reload value to get 1000 hz interrupt rate.
On Timer0 Timer0_isr                    ' Our timer interrupt hanlder
Enable Timer0
Enable Interrupts

Ddrb.4 = 1                  ' enable program LED port
Call Initialize_pwm
State = 0
restore s_table

duration = 0
do
    duration = duration + 500
    do
    loop until TimerCheck(statetimer, Duration) = 1
    print "Delay "; Duration
    prog_led = not prog_led

loop until duration = 5000

Do
    read LeftPwm: read RightPwm: read Duration    ' state of zero
    print leftpwm; " "; rightpwm; " "; duration
    call set_left_motor_pwm(LeftPwm)
    call set_right_motor_pwm(RightPwm)
    if Duration = 0 then
        exit do
    end if
    StateTimer = TimerMSCur()
    prog_led = not prog_led

    do
        ' Kill time (do something useful here...)
    loop until TimerCheck(StateTimer, Duration) = 1

    incr state

Loop

end

'---------------------- Timer handler functions -------------------
Dim Sys_timer As  Word

function TimerMSCur() as word
    disable timer0
    TimerMSCur = Sys_timer
    enable timer0
end function

function TimerCheck(byval start_time as word, byval delay_time as word) as Byte
    start_time = TimerMSCur() - start_time
    if start_time > delay_time then
        TimerCheck = 1
    else
        TimerCheck = 0
    end if
end function
'
Timer0_isr:
    Timer0 = Timer0_reload + Timer0     'reload timer adjusted for overflow
    Incr Sys_timer
Return

$include "ARC_10_PWM.bas"

End

$data
s_table:
'     int   int   int
'     Left  Right Duration
data    0%,    0%,  3000%
data  128%,  128%,  2000%
data    0%,  128%,   800%
data  255%,  255%,  1000%
data    0%,    0%,  1000%
data  128%, -128%,   600%
data -128%, -128%,   800%
data    0%,    0%,     0%
