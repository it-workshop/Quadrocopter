#include "LowPassFilter.h"
#include "Definitions.h"

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
