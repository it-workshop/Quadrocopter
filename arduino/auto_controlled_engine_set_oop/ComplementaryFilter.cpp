#include "ComplementaryFilter.h"

template <class T> void ComplementaryFilter<T>::iteration(T currentRawFastValue, T currentRawSlowValue, double dt)
{
    //(1-alpha) * fast + alpha * slow === alpha * (slow - fast) + fast
    this->value = (currentRawSlowValue - currentRawFastValue) * this->getAlpha(dt) + currentRawFastValue;
}
