#include "Quadrocopter.h"

RVector3D Quadrocopter::get_angle_correction(RVector3D angle, double dt)
{
    pid_angle.set_data0(torqueManualCorrection);
    return(pid_angle.get_y(angle, dt));
}

RVector3D Quadrocopter::get_acceleration_correction(RVector3D angle, RVector3D accel_data)
{
    RVector3D g = angle.projections_from_angle();
    RVector3D a = accel_data - g;

    static RVector3D a_prev = a, res;
    static TimerCount t_time;

    double period = 4, alpha, dt;

    if(!t_time.getTimeIsset()) res = RVector3D(0, 0, 0);
    else {
        dt = t_time.getTimeDifference() / 1.E6;

        alpha = dt / (dt + period / (2 * MPI));

        res = a_prev * (1 - alpha) + a * alpha;
    }

    a_prev = res;
    t_time.setTime();

    RVector3D correction = res.angle_from_projections();
    RVector3D moment_of_force;
    moment_of_force.x = correction.x * accelerometerXi.x;
    moment_of_force.y = correction.y * accelerometerXi.y;

    return(moment_of_force);
}

RVector3D Quadrocopter::get_angular_velocity_correction(RVector3D angular_velocity, double dt)
{
    return(pid_angular_velocity.get_y(angular_velocity, dt));
}
