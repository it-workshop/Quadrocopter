#include "MotorController.h"
#include "Arduino.h"
#include "TimerCount.h"
#include "PID.h"
#include "RVector3D.h"

RVector3D MotorController::get_angle_correction(RVector3D angle, double dt)
{
    return(pid_angle.get_y(angle, dt));
}

RVector3D MotorController::get_acceleration_correction(RVector3D angle, RVector3D accel_data)
{
    RVector3D g = angle.projections_from_angle();
    RVector3D a = accel_data - g;
    
    static RVector3D a_prev = a, res;
    static TimerCount t_time;

    double period = 4, alpha, dt;

    if(!t_time.get_time_isset()) res = RVector3D(0, 0, 0);
    else {
        dt = t_time.get_time_difference() / 1.E6;

        alpha = dt / (dt + period / (2 * MPI));

        res = a_prev * (1 - alpha) + a * alpha;
    }

    a_prev = res;
    t_time.set_time();
     
    RVector3D correction = res.angle_from_projections();
    RVector3D moment_of_force;
    moment_of_force.x = correction.x * accelerometer_xi.x;
    moment_of_force.y = correction.y * accelerometer_xi.y;
     
    return(moment_of_force);
}

RVector3D MotorController::get_angular_velocity_correction(RVector3D angular_velocity, double dt)
{
    return(pid_angular_velocity.get_y(angular_velocity, dt));
}

double MotorController::get_speed(RVector3D torque_vec, int motor)
{
    double res = get_force();
    res += torque_vec.x * coordinates_of_motors[motor].y - torque_vec.y * coordinates_of_motors[motor].x;

    // it is necessary because the motor controller starts a motor with greater speed than needed
    if (res <= MIN_SPEED && get_force() != 0) res = MIN_SPEED;

    // motors offline
    if(get_force() < MIN_SPEED) res = 0;

    // use_motors implementation
    if(!use_motors[motor]) res = 0;

    return(res);
}

void MotorController::set_torque(RVector3D torque_vec)
{
    for (int i = 0; i < N_MOTORS; i++)
        motors_[i].set_power(get_speed(torque_vec, i));
}

void MotorController::set_motors(double power[N_MOTORS])
{
    for (int i = 0; i < N_MOTORS; i++)
        motors_[i].set_power(power[i]);
}

void MotorController::reset()
{
    pid_angle = PID();
    pid_angular_velocity = PID();

    pid_angle.set_KpKiKd(0.2, 0, 0.1);
    pid_angular_velocity.set_KpKiKd(0.2, 0, 0.1);
    pid_angle.set_y_min(- MPI / 4);
    pid_angular_velocity.set_y_min(- MPI / 4);
    pid_angle.set_y_max(MPI / 4);
    pid_angular_velocity.set_y_max(MPI / 4);
}

MotorController::MotorController(const int motor_control_pins[N_MOTORS])
{
    reset();

    accelerometer_xi = RVector3D(0.5, 0.5, 0);
    
    use_motors[A] = 1;
    use_motors[B] = 0;
    use_motors[C] = 1;
    use_motors[D] = 0;
    
    for (int i = 0; i < N_MOTORS; i++)
    {
        motors_[i].set_control_pin(motor_control_pins[i]);
        motors_[i].set_power(0);
    }

    // wait for ESC
    delay(INIT_TIMEOUT);

    set_force(0);
    set_torque(RVector3D());

    coordinates_of_motors[A] = RVector3D(1, 0, 0);
    coordinates_of_motors[B] = RVector3D(0, -1, 0);
    coordinates_of_motors[C] = RVector3D(-1, 0, 0);
    coordinates_of_motors[D] = RVector3D(0, 1, 0);
}

double MotorController::get_force()
{
    return(force);
}

void MotorController::set_force(double a)
{
    force = a;
}
