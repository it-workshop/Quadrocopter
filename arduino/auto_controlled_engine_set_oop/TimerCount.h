#include "Definitions.h"

#ifndef TIMERCOUNT_H
#define TIMERCOUNT_H

class TimerCount
{
    private:
        unsigned long time;
        bool time_isset;
    public:
        TimerCount();

        void set_time();
        
        unsigned long get_time_difference();
        
        bool get_time_isset();
};

#endif
