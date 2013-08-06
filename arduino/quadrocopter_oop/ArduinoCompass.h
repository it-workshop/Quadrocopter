#ifndef ARDUINOCOMPASS_H
#define ARDUINOCOMPASS_H

#define COMPASSPORT Serial3

class ArduinoCompass
{
private:
    double heading;
public:
    bool requested;
    ArduinoCompass();
    void requestHeading();
    void readHeading();
    double getHeading();

};

#endif // ARDUINOCOMPASS_H
