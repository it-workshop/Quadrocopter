#include "VoltageSensor.h"
#include "Arduino.h"

VoltageSensor::VoltageSensor(int newPin, double newMaxVoltage)
{
    pin = newPin;
    maxVoltage = newMaxVoltage;

    pinMode(pin, INPUT);
}

double VoltageSensor::getValue()
{
    double t_val = analogRead(pin);
    t_val /= maxInputValue;
    t_val *= maxVoltage;
    return(t_val);
}
