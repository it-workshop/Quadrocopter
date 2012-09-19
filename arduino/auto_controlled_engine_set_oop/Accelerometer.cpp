#include "Accelerometer.h"
#include "Arduino.h"

RVector3D Accelerometer::getRawReadings()
{
    unsigned int i;
    RVector3D result;
    result *= 0;
    
    for(i = 0; i < Axis; i++)
    {
        #ifdef DEBUG_NO_ACCELEROMETER
            result.value_by_axis_index(i) = 0;
        #else
            result.value_by_axis_index(i) = analogRead(ports[i]) * adcAref / adcMaxvalue;
            
            if(result.value_by_axis_index(i) == NAN || result.value_by_axis_index(i) != result.value_by_axis_index(i))
                result.value_by_axis_index(i) = 0;
        #endif
    }
    
    return(result);
}

RVector3D Accelerometer::getReadings()
{
    unsigned int i;
    RVector3D result;
    result = RVector3D();
    
    for(i = 0; i < AvgN; i++)
        result += getRawReadings();
    
    result /= AvgN;

    for(i = 0; i < Axis; i++)
    {
        result.value_by_axis_index(i) = (result.value_by_axis_index(i) - mapB[i]) / mapA[i];

        if(fabs(result.value_by_axis_index(i)) < Accuracy)
            result.value_by_axis_index(i) = 0;
    }

    return(result);
}
