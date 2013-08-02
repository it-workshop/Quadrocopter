#ifndef INFOLED_H
#define INFOLED_H

#include "Definitions.h"

#ifdef DEBUG_DAC
#include "DAC8512.h"
#endif

class InfoLED
{
private:
    int pin;
    int digitalState;
#ifdef DEBUG_DAC
    DAC8512 DACDev;
#endif

public:
    enum typeT{DIGITAL, PulseWide,
#ifdef DEBUG_DAC
               DAC_8512
#endif
#ifdef _arch_arm_
               DAC_ONBOARD
#endif
    } type;
    void setOn();
    void setOff();
    void changeDigitalState();

    // set new state
    // 0 or 1 for DIGITAL type or
    // 0...100 for PW type
    void setState(int state);

    InfoLED(int nPin, typeT nType = DIGITAL);
    InfoLED();
};

#endif // INFOLED_H
