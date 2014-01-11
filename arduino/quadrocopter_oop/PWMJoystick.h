#ifndef PWMJOYSTICK_H
#define PWMJOYSTICK_H

class PWMJoystick
{
    static const int AVIndex = 3, AngleXIndex = 0, AngleYIndex = 1, PowerIndex = 2;
    static const double AVCoeff = 0.32, AngleCoeff = 0.17;
public:
    PWMJoystick();
    double getAV();
    double getAngleX();
    double getAngleY();
    double getPower();
};

#endif // PWMJOYSTICK_H
