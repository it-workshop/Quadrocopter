#include "Accelerometer.h"
#include "Arduino.h"

RVector3D Accelerometer::getVoltageReadings()
{
    unsigned int i;
    RVector3D result = getRawReadings();
    
    for(i = 0; i < Axis; i++)
    {
        result.valueByAxisIndex(i) *= adcAref / adcMaxvalue;

        if(result.valueByAxisIndex(i) == NAN || result.valueByAxisIndex(i) != result.valueByAxisIndex(i))
            result.valueByAxisIndex(i) = 0;
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
            result.valueByAxisIndex(i) = analogRead(ports[i]);

            if(result.valueByAxisIndex(i) == NAN || result.valueByAxisIndex(i) != result.valueByAxisIndex(i))
                result.valueByAxisIndex(i) = 0;
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
        result.valueByAxisIndex(i) = (result.valueByAxisIndex(i) - mapB[i]) / mapA[i];

        if(fabs(result.valueByAxisIndex(i)) < Accuracy)
            result.valueByAxisIndex(i) = 0;
    }

    return(result);
}
