#ifndef PWMINPUT_H
#define PWMINPUT_H

#include <Arduino.h>

#define NMAX 4

class PWMInput
{
private:
    int pin;
    volatile int A, B;
    // A = \tau, B = T - \tau
    void updateLH();
    void updateHL();
public:
    PWMInput(int _pin);
    int getValue();
};

#endif // PWMINPUT_H
