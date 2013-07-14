#include "Quadrocopter.h"
#include "PID.h"

RVector3D Quadrocopter::getAngleCorrection(RVector3D angle, double dt)
{
    pidAngle.setData0(angleManualCorrection);

    // here should be -1 because the equation
    // should be like x'' = -x'
    RVector3D avPID = angularVelocity * -1;
    if(fabs(avPID.x) < PID_AV_MIN) avPID.x = 0;
    if(fabs(avPID.y) < PID_AV_MIN) avPID.y = 0;
    avPID /= 10;
    RVector3D res = pidAngle.getY(angle, dt, avPID);
    res.z = 0;
    return(res);
}

RVector3D Quadrocopter::getAccelerationCorrection(RVector3D angle, RVector3D accelData0)
{
    RVector3D g = angle.projectionsFromAngle();
    RVector3D a = accelData0 - g;

    static RVector3D aPrev = a, res;
    static TimerCount tTime;

    double period = 4, alpha, dt;

    if(!tTime.getTimeIsset()) res = RVector3D(0, 0, 0);
    else {
        dt = tTime.getTimeDifference() / 1.E6;

        alpha = dt / (dt + period / (2 * MPI));

        res = aPrev * (1 - alpha) + a * alpha;
    }

    aPrev = res;
    tTime.setTime();

    RVector3D correction = res.angleFromProjections();
    RVector3D momentOfForce;
    momentOfForce.x = correction.x * accelerometerXi.x;
    momentOfForce.y = correction.y * accelerometerXi.y;

    return(momentOfForce);
}

RVector3D Quadrocopter::getAngularVelocityCorrection(RVector3D angular_velocity, double dt)
{
    return(pidAngularVelocity.getY(angular_velocity, dt));
}
