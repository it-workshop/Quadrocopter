#include "Accelerometer.h"
#include "Arduino.h"

RVector3D Accelerometer::get_raw_readings()
{
    unsigned int i;
    RVector3D result;
    result *= 0;
    
    for(i = 0; i < AXIS; i++)
    {
        #ifdef DEBUG_NO_ACCELEROMETER
            result.value_by_axis_index(i) = 0;
        #else
            result.value_by_axis_index(i) = analogRead(ports[i]) * adc_aref / adc_maxvalue;
            
            if(result.value_by_axis_index(i) == NAN || result.value_by_axis_index(i) != result.value_by_axis_index(i))
                result.value_by_axis_index(i) = 0;
        #endif
    }
    
    return(result);
}

RVector3D Accelerometer::get_readings()
{
    unsigned int i;
    RVector3D result;
    result = RVector3D();
    
    for(i = 0; i < AVG_N; i++)
        result += get_raw_readings();
    
    result /= AVG_N;

    result -= defaults;

    for(i = 0; i < AXIS; i++)
    {
        result.value_by_axis_index(i) = (result.value_by_axis_index(i) - map_b[i]) / map_a[i];

        if(fabs(result.value_by_axis_index(i)) < ACCURACY)
            result.value_by_axis_index(i) = 0;
    }

    return(result);
}
