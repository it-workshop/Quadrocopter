#include "LowPassFilter.h"

#ifndef COMPLEMENTARYFILTER_H
#define COMPLEMENTARYFILTER_H

class ComplementaryFilter : LowPassFilter
{
public:
    inline void iteration(double currentRawFastValue, double currentRawSlowValue, double dt); // update value using period and argument as dt
};

#endif // COMPLEMENTARYFILTER_H
