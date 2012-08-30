#include "Motor.h"
#include "Arduino.h"

Motor::Motor(int control_pin_)
{
    set_control_pin(control_pin_);

    set_power(0);
}

Motor::Motor()
{
    control_pin = -1;
    power = 0;
}

inline double Motor::get_power()
{
    return(power);
}

void Motor::set_power(double n_power)
{
    if(control_pin != -1)
    {
        power = n_power;

        int speedie;

        if (power >= 1) speedie = MAX_SPEED;
        else if (power <= 0) speedie = MIN_SPEED;
        else speedie = power * (MAX_SPEED - MIN_SPEED) + MIN_SPEED;

        analogWrite(control_pin, speedie);
    }
}

void Motor::set_control_pin(int control_pin_)
{
    control_pin = control_pin_;
    pinMode(control_pin, OUTPUT);

    set_power(0);
}
