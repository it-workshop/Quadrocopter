#include "PWMInput.h"

PWMInput::PWMInput(int _pin)
{
    pin = _pin;
    attachInterrupt(pin, (void (*)(void))(this->updateLH), RISING);
    attachInterrupt(pin, (void (*)()) (this->updateHL), FALLING);
}

void PWMInput::updateLH()
{
    int C = micros();
    A = C;
    B = C - B;
}


void PWMInput::updateHL()
{
    int C = micros();
    A = C - A;
    B = micros();
}

int PWMInput::getValue()
{
    return(100 * A / (A + B));
}
