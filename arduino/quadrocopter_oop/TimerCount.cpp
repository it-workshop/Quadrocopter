#include <Arduino.h>
#include "TimerCount.h"

TimerCount::TimerCount()
{
    timeIsset = false;
}

void TimerCount::setTime()
{
    time = micros();
    timeIsset = true;
}

unsigned long TimerCount::getTimeDifference()
{
    return(micros() - time);
}

double TimerCount::getTimeDifferenceSeconds()
{
    return(getTimeDifference() / 1.E6);
}

bool TimerCount::getTimeIsset()
{
    return(timeIsset);
}
