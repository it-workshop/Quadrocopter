#ifndef INFOLED_H
#define INFOLED_H

#include "DAC8512.h"

class InfoLED
{
private:
    int pin;
    DAC8512 DACDev;

public:
    enum typeT{DIGITAL, PWM, DAC} type;
    void setOn();
    void setOff();

    // set new state
    // 0 or 1 for DIGITAL type or
    // 0...100 for PWM type
    void setState(int state);

    InfoLED(int nPin, typeT nType = DIGITAL);
    InfoLED();
};

#endif // INFOLED_H
