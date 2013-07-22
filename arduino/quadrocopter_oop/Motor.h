#ifndef MOTOR_H
#define MOTOR_H

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

public:
    Motor(int pin);
    Motor();

    inline double getPower(); // value in [0...1]
    void setPower(double powerValue); // value in [0...1]
    void setControlPin(int newPin);
};

#endif
