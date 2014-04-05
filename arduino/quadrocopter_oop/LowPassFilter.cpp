#include <Arduino.h>
#include "LowPassFilter.h"
#include "Definitions.h"

#ifndef LOWPASSFILTER_CPP
#define LOWPASSFILTER_CPP

template <typename T> LowPassFilter<T>::LowPassFilter()
{
    reset();
    period = 0;
}

template <typename T> LowPassFilter<T>::LowPassFilter(double nPeriod)
{
    LowPassFilter();
    setPeriod(nPeriod);
}

template <typename T> void LowPassFilter<T>::setPeriod(double nPeriod)
{
    period = nPeriod;
}

template <typename T> T LowPassFilter<T>::getValue()
{
    return(value);
}

template <typename T> inline void LowPassFilter<T>::iterationAngle(T currentRawValue, double dt)
{
    // see https://github.com/Mrkol/RoboMobo/blob/master/src/com/RoboMobo/Map.java#L272
    double delta = currentRawValue - value;
    angleMPIPINorm(delta);
    value += delta * getAlpha(dt);
    angleMPIPINorm(value);
    //value -= M_PI;
}

template <typename T> inline void LowPassFilter<T>::iteration(T currentRawValue, double dt)
{
    value = (currentRawValue - value) * getAlpha(dt) + value;
}

template <typename T> double LowPassFilter<T>::getAlpha(double dt)
{
    return(dt / (dt + period / (2 * MPI)));
}

template <typename T> inline void LowPassFilter<T>::reset()
{
    value = 0;
}

template <typename T> double LowPassFilter<T>::getPeriod()
{
    return(period);
}

template <typename T> void LowPassFilter<T>::setValue(T newValue)
{
    value = newValue;
}

#endif
