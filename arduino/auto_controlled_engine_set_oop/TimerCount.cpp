#include <Arduino.h>
#include "TimerCount.h"

TimerCount::TimerCount()
{
    time_isset = false;
}

void TimerCount::set_time()
{
    time = micros();
    time_isset = true;
}

unsigned long TimerCount::get_time_difference()
{
    return(micros() - time);
}

bool TimerCount::get_time_isset()
{
    return(time_isset);
}
