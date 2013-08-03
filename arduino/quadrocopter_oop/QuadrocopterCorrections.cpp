#include "Quadrocopter.h"
#include "PID.h"

RVector3D Quadrocopter::getAngleCorrection(RVector3D angle, double dt)
{
    pidAngleX.data0 = (angleManualCorrection.x);
	pidAngleY.data0 = (angleManualCorrection.y);

#ifdef PID_USE_YAW
    pidAngularVelocityZ.data0 = 0;
#endif

    // here should be -1 because the equation
    // should be like x'' = -x'
    RVector3D avPID = angularVelocity * -1;
    if(fabs(avPID.x) < PID_AV_MIN) avPID.x = 0;
    if(fabs(avPID.y) < PID_AV_MIN) avPID.y = 0;
    avPID /= 10;
    RVector3D res;
	res.x = pidAngleX.getY(angle.x, dt, avPID.x);
	res.y = pidAngleY.getY(angle.y, dt, avPID.y);

#ifdef PID_USE_YAW
    RVector3D avPIDav = angularVelocity;
    if(fabs(avPIDav.z) < PID_AV_MIN_Z) avPIDav.z = 0;
    res.z = pidAngularVelocityZ.getY(avPIDav.z, dt);
#else
    res.z = 0;
#endif

    return(res);
}
