#include "Accelerometer.h"
#include "Arduino.h"

RVector3D Accelerometer::getVoltageReadings()
{
    unsigned int i;
    RVector3D result = getRawReadings();
    
    for(i = 0; i < Axis; i++)
    {
        result.value_by_axis_index(i) *= adcAref / adcMaxvalue;

        if(result.value_by_axis_index(i) == NAN || result.value_by_axis_index(i) != result.value_by_axis_index(i))
            result.value_by_axis_index(i) = 0;
    }
    
    return(result);
}

RVector3D Accelerometer::getRawReadings()
{
    unsigned int i;
    RVector3D result;

    for(i = 0; i < Axis; i++)
    {
        #ifdef DEBUG_NO_ACCELEROMETER
            result.value_by_axis_index(i) = 0;
        #else
            result.value_by_axis_index(i) = analogRead(ports[i]);

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
        result += getVoltageReadings();
    
    result /= AvgN;

    for(i = 0; i < Axis; i++)
    {
        result.value_by_axis_index(i) = (result.value_by_axis_index(i) - mapB[i]) / mapA[i];

        if(fabs(result.value_by_axis_index(i)) < Accuracy)
            result.value_by_axis_index(i) = 0;
    }

    return(result);
}
