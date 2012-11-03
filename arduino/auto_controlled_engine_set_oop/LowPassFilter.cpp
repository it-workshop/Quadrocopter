#include "LowPassFilter.h"
#include "Definitions.h"

LowPassFilter::LowPassFilter()
{
    reset();
    period = 0;
}

LowPassFilter::LowPassFilter(double nPeriod)
{
    LowPassFilter();
    setPeriod(nPeriod);
}

void LowPassFilter::setPeriod(double nPeriod)
{
    period = nPeriod;
}

inline double LowPassFilter::getAlpha(double dt)
{
    return(dt / (dt + period / (2 * MPI)));
}

inline void LowPassFilter::iteration(double currentRawValue, double dt)
{
    value = getAlpha(dt) * (currentRawValue - value) + value;
}

void LowPassFilter::reset()
{
    value = 0;
}

double LowPassFilter::getPeriod()
{
    return(period);
}
