#ifndef INFOLED_H
#define INFOLED_H

class InfoLED
{
private:
    int pin;

public:
    void setOn();
    void setOff();

    void setState(bool state);

    InfoLED(int n_pin);
    InfoLED();
};

#endif // INFOLED_H
