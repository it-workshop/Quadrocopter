#include "Quadrocopter.h"
#include "PID.h"

RVector3D Quadrocopter::getAngleCorrection(RVector3D angle, double dt)
{
    pidAngle.setData0(angleManualCorrection);

#ifdef PID_USE_YAW
    pidAngularVelocity.setData0(0);
#endif

    // here should be -1 because the equation
    // should be like x'' = -x'
    RVector3D avPID = angularVelocity * -1;
    if(fabs(avPID.x) < PID_AV_MIN) avPID.x = 0;
    if(fabs(avPID.y) < PID_AV_MIN) avPID.y = 0;
    avPID /= 10;
    RVector3D res = pidAngle.getY(angle, dt, avPID);

#ifdef PID_USE_YAW
    RVector3D resAV = pidAngularVelocity.getY(angularVelocity, dt);
    res.z = resAV.z;
#else
    res.z = 0;
#endif

    return(res);
}
