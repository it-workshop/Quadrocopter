#include "RVector3D.h"

#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

class Accelerometer
{
private:
    static const unsigned int AXIS = 3, AVG_N = 1;
    static const double ACCURACY = 1E-2;
    
    static const double adc_aref = 5, adc_maxvalue = 1023;
    double map_a[AXIS], map_b[AXIS];
    
    int ports[AXIS];
    RVector3D defaults;

public:
    Accelerometer(int new_ports[AXIS])
    {
        map_a[0] = 0.8616664408; map_b[0] = 1.5204146893;
        map_a[1] = 0.892143084;  map_b[1] = 1.8134632093;
        map_a[2] = 0.8861390819; map_b[2] = 1.5457698227;
        
        defaults = RVector3D();
        
        unsigned int i;
        for(i = 0; i < AXIS; i++)
        {
            ports[i] = new_ports[i];
        }
        
//        set_defaults();
    }

    RVector3D get_readings(); //in m/s^2 divided by g
    RVector3D get_raw_readings(); //in volts

    void set_defaults()
    {
        defaults = get_readings();
    }
};

#endif
