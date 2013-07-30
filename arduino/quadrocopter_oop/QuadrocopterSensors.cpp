#include "Quadrocopter.h"

void Quadrocopter::processSensorsData()
{
    //MPU->iteration();
    angle.parseFloat(MyMPU->getAngleXYZ());
    angularVelocity.parseFloat(MyMPU->getAngularVelocityXYZ());

    voltage = VSensor->getValue();
}
