#ifndef MOTOR_H
#define MOTOR_H

#ifdef _arch_arm_
    #include <pwm01.h>
#endif

class Motor
{
private:
    // raw data for analogWrite
    enum RawSpeedLimits
    {
        MinSpeed = 100,
        MaxSpeed = 254
    };

    int controlPin;
    double power;
    int speedie;

#ifdef _arch_arm_
    uint32_t PWMFreq  = 500;
    uint32_t PWMScale = 256;
#endif

public:
    Motor(int pin);
    Motor();

    inline double getPower(); // value in [0...1]
    void setPower(double powerValue); // value in [0...1]
    void setControlPin(int newPin);
};

#endif
