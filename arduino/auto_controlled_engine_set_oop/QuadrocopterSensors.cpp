#include "Quadrocopter.h"
#include "ComplementaryFilter.cpp"
#include "LowPassFilter.cpp"

void Quadrocopter::processSensorsData()
{
    //MPU->iteration();
    angle.x = MPU->getYPR()[2];
    angle.y = MPU->getYPR()[1];
    angle.z = MPU->getYPR()[0];
    voltage = VSensor->getValue();
}
