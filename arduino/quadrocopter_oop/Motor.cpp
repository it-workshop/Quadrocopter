#include "Motor.h"
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

        int speedie;

        if (power >= 1) speedie = MaxSpeed;
        else if (power <= 0) speedie = MinSpeed;
        else speedie = power * (MaxSpeed - MinSpeed) + MinSpeed;

        analogWrite(controlPin, speedie);
    }
}

void Motor::setControlPin(int control_pin_)
{
    controlPin = control_pin_;
    pinMode(controlPin, OUTPUT);

    setPower(0);
}
