#include "Quadrocopter.h"
#include "PID.h"

RVector3D Quadrocopter::getAngleCorrection(RVector3D angle, double dt)
{
#ifdef PID_USE_YAW_ANGLE
    #ifndef COMPASS_ROTATE_COPTER
        // HERE should be code that "rotates" data0
        // by (copterHeading - joystickHeading)
    #endif
#endif
    pidAngleX.data0 = (angleManualCorrection.x + angleOffsetPC.x);
    pidAngleY.data0 = (angleManualCorrection.y + angleOffsetPC.y);

#ifdef PID_USE_YAW
    pidAngularVelocityZ.data0 = 0;
#endif

#ifdef PID_USE_YAW_ANGLE
    #ifdef COMPASS_ROTATE_COPTER
        pidAngleZ.data0 = (joystickHeading + angleOffsetPC.z);
    #else
        pidAngleZ.data0 = 0;
    #endif
#endif

    // here should be -1 because the equation
    // should be like x'' = -x'
    RVector3D avPID = angularVelocity * -1;
    if(fabs(avPID.x) < PID_AV_MIN) avPID.x = 0;
    if(fabs(avPID.y) < PID_AV_MIN) avPID.y = 0;
    avPID /= 10;
    RVector3D res;
    res.x = pidAngleX.getY(angle.x, dt/*, avPID.x*/);
    res.y = pidAngleY.getY(angle.y, dt/*, avPID.y*/);

    res.z = 0;
#ifdef PID_USE_YAW
    RVector3D avPIDav = angularVelocity;
    if(fabs(avPIDav.z) < PID_AV_MIN_Z) avPIDav.z = 0;
    res.z = pidAngularVelocityZ.getY(avPIDav.z, dt);
#endif

#ifdef PID_USE_YAW_ANGLE
    RVector3D avPIDav = angularVelocity * -1;
    avPIDav /= 10;
    if(fabs(avPIDav.z) < PID_AV_MIN_Z) avPIDav.z = 0;
    res.z = pidAngleZ.getY(copterHeading, dt, avPIDav.z);
#endif

    return(res);
}
