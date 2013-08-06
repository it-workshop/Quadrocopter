#include "InfoLED.h"
#include "Definitions.h"
#include "Arduino.h"

InfoLED::InfoLED(int nPin, typeT nType)
{
#ifdef _arch_arm_
if(nType == DAC_ONBOARD)
{
    if(nPin == 0)
        pinMode(DAC0, OUTPUT);
    else if(nPin == 1)
        pinMode(DAC1, OUTPUT);
}
#endif
#ifdef _arch_avr_
    #ifdef DEBUG_DAC
        if(nType == DAC_8512)
            DACDev.init();
        else
    #endif
#endif
    if(nType == PulseWide || nType == DIGITAL)
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
#ifdef _arch_avr_
    #ifdef DEBUG_DAC
        else if(type == DAC_8512)
            DACDev.send(((1 << 12) - 1) * ((long int) state) / 100);
    #endif
#endif
#ifdef _arch_arm_
    else if(type == DAC_ONBOARD)
    {
        if(pin == 0)
        {
            analogWrite(DAC0, state * 255 / 100);
        }
        else if(pin == 1)
            analogWrite(DAC1, state * 255 / 100);
    }
#endif
}
