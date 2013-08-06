#ifndef ARDUINOCOMPASS_H
#define ARDUINOCOMPASS_H

#define COMPASSPORT Serial3

class ArduinoCompass
{
private:
    double heading;
public:
    ArduinoCompass();
    double requestHeading();
    double readHeading();
    double getHeading();

};

#endif // ARDUINOCOMPASS_H
