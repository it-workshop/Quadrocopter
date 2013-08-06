#include "ArduinoCompass.h"

ArduinoCompass::ArduinoCompass()
{
    COMPASSPORT.begin(115200);

    // reading shit
    COMPASSPORT.print('c');
    while(COMPASSPORT.available())
    {
        COMPASSPORT.read();
    }
}

double ArduinoCompass::requestHeading()
{
    COMPASSPORT.print('r');
}

double ArduinoCompass::readHeading()
{
    if(COMPASSPORT.available() >= 2)
    {
        int t_high, t_low;
        t_low = COMPASSPORT.read();
        t_high = COMPASSPORT.read();
        heading = ((t_low) + (t_high << 8)) / 10000.;
    }
}

double ArduinoCompass::getHeading()
{
    return(heading);
}
