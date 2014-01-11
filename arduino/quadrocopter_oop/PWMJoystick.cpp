#include "PWMJoystick.h"
#include "PWMInput.h"

PWMJoystick::PWMJoystick()
{
    PWMInit();
}

double PWMJoystick::getAV()
{
    return(PWMGetValue(AVIndex) * AVCoeff);
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
