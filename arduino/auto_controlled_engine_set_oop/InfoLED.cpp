#include "InfoLED.h"
#include "Arduino.h"

InfoLED::InfoLED(int nPin, typeT nType)
{
    if(nType == DAC)
        DACDev.init();
    else
        pinMode(nPin, OUTPUT);
    pin = nPin;
    type = nType;
}

InfoLED::InfoLED()
{
}

void InfoLED::setOn()
{
    setState(type == DIGITAL ? 1 : 100);
}

void InfoLED::setOff()
{
    setState(0);
}

void InfoLED::setState(int state)
{
    if(type == DIGITAL)
        digitalWrite(pin, state ? HIGH : LOW);
    else if(type == PWM)
        analogWrite(pin, state * 255 / 100);
    else DACDev.send(((1 << 12) - 1) * ((long int) state) / 100);
}
