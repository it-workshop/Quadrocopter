#include "Quadrocopter.h"
#include "ComplementaryFilter.cpp"
#include "LowPassFilter.cpp"

void Quadrocopter::processSensorsData()
{
    //MPU->iteration();
    angle.parseFloat(MPU->getAngleXYZ());
    angularVelocity.parseFloat(MPU->getAngularVelocityXYZ());

    voltage = VSensor->getValue();
}
