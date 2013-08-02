#include "InfoLED.h"
#include "Arduino.h"

InfoLED::InfoLED(int nPin, typeT nType)
{
#ifdef DEBUG_DAC
    if(nType == DAC_8512)
        DACDev.init();
    else
#endif
        pinMode(nPin, OUTPUT);
    pin = nPin;
    type = nType;
    digitalState = 0;
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

void InfoLED::changeDigitalState()
{
    digitalState = digitalState == 0 ? 1 : 0;
    setState(digitalState);
}

void InfoLED::setState(int state)
{
    if(type == DIGITAL)
    {
        digitalWrite(pin, state ? HIGH : LOW);
        digitalState = state;
    }
    else if(type == PulseWide)
        analogWrite(pin, state * 255 / 100);
#ifdef DEBUG_DAC
    else DACDev.send(((1 << 12) - 1) * ((long int) state) / 100);
#endif
}
