#include "InfoLED.h"
#include "Arduino.h"

InfoLED::InfoLED(int n_pin)
{
    pinMode(n_pin, OUTPUT);
    pin = n_pin;
}

InfoLED::InfoLED()
{
}

void InfoLED::setOn()
{
    setState(1);
}

void InfoLED::setOff()
{
    setState(0);
}

void InfoLED::setState(bool state)
{
    digitalWrite(pin, state ? HIGH : LOW);
}
