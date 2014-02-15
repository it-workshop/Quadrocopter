#include "Motor.h"
#include "Definitions.h"
#include "Arduino.h"

Motor::Motor(int pin)
{
    setControlPin(pin);

    setPower(0);
}

Motor::Motor()
{
    controlPin = -1;
    power = 0;
    PWMInitialized = false;
}

inline double Motor::getPower()
{
    return(power);
}

void Motor::setPower(double powerValue)
{
    if(controlPin != -1)
    {
        power = powerValue;

        if (power >= 1) speedie = MaxSpeed;
        else if (power <= 0) speedie = MinSpeed;
        else speedie = power * (MaxSpeed - MinSpeed) + MinSpeed;

        if(!PWMInitialized) return;

#ifdef _arch_avr_
        analogWrite(controlPin, speedie);
#endif
#ifdef _arch_arm_
        pwm_write_duty(controlPin, speedie * PWMScale);
#endif
    }
}

void Motor::setControlPin(int control_pin_)
{
    controlPin = control_pin_;

    setPower(0);
}

void Motor::initializeControlPin()
{
    #ifdef _arch_avr_
        pinMode(controlPin, OUTPUT);
    #endif
    #ifdef _arch_arm_
        pwm_setup(controlPin, PWMFreq, 1);
    #endif

    PWMInitialized = true;
}
