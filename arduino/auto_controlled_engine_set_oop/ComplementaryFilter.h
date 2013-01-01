#include "LowPassFilter.h"

#ifndef COMPLEMENTARYFILTER_H
#define COMPLEMENTARYFILTER_H

template <typename T> class ComplementaryFilter : public LowPassFilter<T>
{
public:
    void iteration(T currentRawFastValue, T currentRawSlowValue, double dt); // update value using period and argument as dt
};

#endif // COMPLEMENTARYFILTER_H
