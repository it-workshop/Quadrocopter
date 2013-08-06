#include "ArduinoCompass.h"
#include "Arduino.h"

ArduinoCompass::ArduinoCompass()
{
    COMPASSPORT.begin(115200);

    // reading shit
    COMPASSPORT.print('c');
    delay(10);
    while(COMPASSPORT.available())
    {
        COMPASSPORT.read();
    }
}

void ArduinoCompass::requestHeading()
{
    if(requested) return;
    COMPASSPORT.print('r');
    requested = true;
}

void ArduinoCompass::readHeading()
{
    if(!requested) return;
    if(COMPASSPORT.available() >= 2)
    {
        int t_high, t_low;
        t_low = COMPASSPORT.read();
        t_high = COMPASSPORT.read();
        heading = ((t_low) + (t_high << 8)) / 10000.;
        requested = false;
    }
}

double ArduinoCompass::getHeading()
{
    return(heading);
}
