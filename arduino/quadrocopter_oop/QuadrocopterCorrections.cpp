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

    pidAngleX.setIUse(flying);
    pidAngleY.setIUse(flying);

    RVector3D res;

    res.x = pidAngleX.getY(angle.x, dt);
    res.y = pidAngleY.getY(angle.y, dt);

    res.z = 0;

#ifdef PID_USE_YAW_ANGLE
    pidAngleZ.setIUse(flying);

    res.z = pidAngleZ.getY(copterHeading, dt, angularVelocity.z);
#endif

#ifdef MINIMUM_PID_THROTTLE
    if(MController->getForce() < MINIMUM_PID_THROTTLE)
        return(RVector3D(0, 0, 0));
    else
#endif
        return(res);
}
