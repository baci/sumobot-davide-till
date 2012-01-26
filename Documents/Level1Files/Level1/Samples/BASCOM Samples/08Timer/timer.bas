'
' Timer.bas sample code
'
' Implement a periodic timer (interrupt handler) at 1000hz (1ms)
' implement some code to manage precision delays based upon the timer tick
'
' NOTE WELL: YOU MUST INCREASE THE HARDWARE STACK ALLOCATED IN BASCOM FOR THIS OR
' ANY OTHER INTERRUPT DRIVEN PROGRAM TO WORK!!  You need roughly 35 bytes for each
' enabled interrupt source.
'
$regfile = "m16def.dat"
$crystal = 8000000
$baud = 38400
'
declare function TimerMSCur() as word
declare function TimerCheck(byval start_time as word, byval delay_time as word) as byte
'
Config Timer0 = Timer , Prescale = 64   ' 125 khz
Const Timer0_reload = 256 - 125          ' Reload value to get 1000 hz interrupt rate.
On Timer0 Timer0_isr                    ' Our timer interrupt hanlder
Enable Timer0
Enable Interrupts
'
Config Pinb.4 = Output                  ' LED for winkie
prog_led alias portb.4

dim left_val as word, right_val as word , myTimer as word
'

Do
    myTimer = TimerMSCur()
    '
    ' Do some significant processing
    '
    print "Right "; GetADC(1); " Left "; GetADC(3)
    waitms 100
    '
    ' Now wait for time to toggle LED
    '
    do
        ' Nothing useful
    loop until TimerCheck(myTimer, 500) = 1

    prog_led = not prog_led
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
'
End                                     'end program