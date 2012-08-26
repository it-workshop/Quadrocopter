#include "Motor.h"
#include "Arduino.h"

inline const int Motor::power()
{
    return (double) 100 * (speedie - MIN_SPEED) / (MAX_SPEED - MIN_SPEED);
}

void Motor::makeSpeed(int percentage)
{
    if (percentage >= 100) speedie = MAX_SPEED;
    else if (percentage <=   0) speedie = MIN_SPEED;
    else speedie = (double) 0.01 * percentage * (MAX_SPEED - MIN_SPEED) + MIN_SPEED;
    
    analogWrite(control_pin, speedie);
}
