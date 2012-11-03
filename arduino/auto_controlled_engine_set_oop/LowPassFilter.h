#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

class ComplementaryFilter;

class LowPassFilter
{
friend class ComplementaryFilter;
private:
    double period;
    double value;
public:
    LowPassFilter();
    LowPassFilter(double nPeriod);

    inline void setPeriod(double nPeriod);

    inline void iteration(double currentRawValue, double dt); // update value using period and argument as dt
    inline double getAlpha(double dt); // returns alpha coefficient
    inline double getValue(); // returns current value
    inline double getPeriod(); // returns period

    inline void reset();
};

#endif // LOWPASSFILTER_H
