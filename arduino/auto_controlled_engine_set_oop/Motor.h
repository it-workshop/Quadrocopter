#ifndef MOTOR_H
#define MOTOR_H

class Motor
{
private:
    // raw data for analogWrite
    enum EXTREMAL_SPEED 
    {
        MIN_SPEED = 100,
        MAX_SPEED = 254
    };

    int control_pin;
    double power;

public:
    Motor(int control_pin_);
    Motor();

    inline double get_power(); // value in [0...1]
    void set_power(double); // value in [0...1]
    void set_control_pin(int control_pin_);
};

#endif
