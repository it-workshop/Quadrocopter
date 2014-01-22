#ifndef VOLTAGESENSOR_H
#define VOLTAGESENSOR_H

class VoltageSensor
{
    int pin;
    double maxVoltage;
    static const double maxInputValue = 1023;

public:
    VoltageSensor(int newPin, double newMaxVoltage);
    double getValue();
    double getRawValue();
};

#endif // VOLTAGESENSOR_H
