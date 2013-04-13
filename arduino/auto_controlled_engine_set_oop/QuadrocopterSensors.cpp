#include "Quadrocopter.h"
#include "ComplementaryFilter.h"

void Quadrocopter::processSensorsData()
{
    //MPU->iteration();
    angle.parseFloat(MPU->getAngleXYZ());
    angularVelocity.parseFloat(MPU->getAngularVelocityXYZ());

    voltage = VSensor->getValue();
}
