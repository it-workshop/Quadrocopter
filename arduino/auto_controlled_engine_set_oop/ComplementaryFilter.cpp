#include "ComplementaryFilter.h"

void ComplementaryFilter::iteration(double currentRawFastValue, double currentRawSlowValue, double dt)
{
    value = currentRawSlowValue;
    value = getAlpha(dt) * (currentRawFastValue - value) + value;
}
