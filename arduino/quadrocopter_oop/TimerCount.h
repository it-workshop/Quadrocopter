#include "Definitions.h"

#ifndef TIMERCOUNT_H
#define TIMERCOUNT_H

class TimerCount
{
    private:
        unsigned long time;
        bool timeIsset;
    public:
        TimerCount();

        void setTime();
        
        unsigned long getTimeDifference();

        double getTimeDifferenceSeconds();
        
        bool getTimeIsset();
};

#endif
