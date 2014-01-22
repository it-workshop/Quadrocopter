#include "PWMJoystick.h"
#include "PWMInput.h"

PWMJoystick::PWMJoystick()
{
    PWMInit();
}

double PWMJoystick::getAV()
{
    double v = PWMGetValue(AVIndex) * AVCoeff;
    if(fabs(v) < AVCoeffMin) v = 0;
    return(v);
}

double PWMJoystick::getAngleX()
{
    return(PWMGetValue(AngleXIndex) * AngleCoeff);
}

double PWMJoystick::getAngleY()
{
    return(PWMGetValue(AngleYIndex) * AngleCoeff);
}

double PWMJoystick::getPower()
{
    return(PWMGetValue(PowerIndex));
}
