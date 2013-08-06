#include "Quadrocopter.h"

void Quadrocopter::processSensorsData()
{
    //MPU->iteration();
    angle.parseFloat(MyMPU->getAngleXYZ());
    angularVelocity.parseFloat(MyMPU->getAngularVelocityXYZ());

    voltage = VSensor->getValue();

#ifdef USE_COMPASS
    {
        double ax = angle.x;
        double ay = angle.y;
        int16_t mx, my, mz;
        MyCompass->getHeading(&mx, &my, &mz);

        RVector3D xi3, xi2, xi1;
        xi3.x = mx;
        xi3.y = my;
        xi3.z = mz;

        xi2.x =      xi3.x      +          0      +         0;
        xi2.y =        0        + cos(ax) * xi3.y - sin(ax) * xi3.z;
        xi2.z =        0        + sin(ax) * xi3.y + cos(ax) * xi3.z;

        xi1.x = cos(ay) * xi2.x +          0      + sin(ay) * xi2.z;
        xi1.y =        0        +        xi2.y    +         0;
        xi1.z =-sin(ay) * xi2.x +          0      + cos(ay) * xi2.z;

        copterHeading = atan2(xi1.y, xi1.x);
        if(copterHeading < 0)
            copterHeading += 2 * M_PI;
    }
#endif
}
