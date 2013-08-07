#include "Quadrocopter.h"

void Quadrocopter::processSensorsData()
{
    //MPU->iteration();
    angle.parseFloat(MyMPU->getAngleXYZ());
    angularVelocity.parseFloat(MyMPU->getAngularVelocityXYZ());

    voltage = VSensor->getValue();

#ifdef USE_COMPASS
    {
        MyCompass->getHeadingCorrected(&magX, &magY, &magZ, &(BMag.x), &(BMag.y), &(BMag.z));

        RVector3D xi3, xi2, xi1;
        xi3 = BMag;

        xi2.x =      xi3.x      +          0      +         0;
        xi2.y =        0        + cos(angle.x) * xi3.y - sin(angle.x) * xi3.z;
        xi2.z =        0        + sin(angle.x) * xi3.y + cos(angle.x) * xi3.z;

        xi1.x = cos(angle.y) * xi2.x +          0      + sin(angle.y) * xi2.z;
        xi1.y =        0        +        xi2.y    +         0;
        xi1.z =-sin(angle.y) * xi2.x +          0      + cos(angle.y) * xi2.z;

        copterHeading = atan2(xi1.y, xi1.x);
        if(copterHeading < 0)
            copterHeading += 2 * M_PI;

        copterHeading = 2 * M_PI - copterHeading;
    }
#endif
}
