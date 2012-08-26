#ifndef MOTOR_H
#define MOTOR_H

struct Motor
{
    enum EXTREMAL_SPEED 
    {
        MIN_SPEED = 100,
        MAX_SPEED = 254
    };
    int control_pin;
    int speedie;
    inline const int power();
    void makeSpeed(int percentage); 
};

#endif
