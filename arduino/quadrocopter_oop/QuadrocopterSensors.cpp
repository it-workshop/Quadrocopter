#include "Quadrocopter.h"

void Quadrocopter::processSensorsData()
{

    //MPU->iteration();
    angle.parseFloat(MyMPU->getAngleXYZ());
    angularVelocity.parseFloat(MyMPU->getAngularVelocityXYZ());

    voltage = VSensor->getValue();

#ifdef USE_COMPASS
    {
        int16_t mx, my, mz;
        MyCompass->getHeading(&mx, &my, &mz);
        copterHeading = atan2(my, mx);
        if(copterHeading < 0)
            copterHeading += 2 * M_PI;
    }
#endif
}
