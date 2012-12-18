#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

template <typename T> class ComplementaryFilter;

template <typename T> class LowPassFilter
{
friend class ComplementaryFilter<T>;
private:
    double period;
    T value;
public:
    LowPassFilter();
    LowPassFilter(double nPeriod);

    inline void setPeriod(double nPeriod);

    inline void iteration(T currentRawValue, double dt); // update value using period and argument as dt
    inline double getAlpha(double dt); // returns alpha coefficient
    inline T getValue(); // returns current value
    inline double getPeriod(); // returns period

    inline void reset();
};

#endif // LOWPASSFILTER_H
