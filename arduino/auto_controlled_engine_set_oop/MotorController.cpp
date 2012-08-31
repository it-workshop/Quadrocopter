#include "MotorController.h"
#include "Arduino.h"
#include "TimerCount.h"
#include "RVector3D.h"

double MotorController::get_force()
{
    return(force);
}

void MotorController::set_force(double a)
{
    force = a;
}

RVector3D MotorController::get_angle_correction(RVector3D angle, double dt)
{
    static RVector3D prev_e = RVector3D(0, 0, 0),
        e_integral = RVector3D(0, 0, 0);
    
    //requested angle
    RVector3D angle0 = RVector3D(0, 0, 0);
    
    //difference between requested and current angle
    RVector3D e = angle0 - angle;
    
    //discrete derivative
    RVector3D e_derivative = (e - prev_e) / dt;
    
    //discrete integral
    e_integral += e * dt;
    
    //correction
    RVector3D y = e * angle_Kp + e_integral * angle_Ki + e_derivative * angle_Kd;
    
    RVector3D u = y; // reaction
    
    //max and min correction
    for(int i = 0; i < 2; i++)
    {
        if(u.value_by_axis_index(i) < -angle_max_correction)
            u.value_by_axis_index(i) = -angle_max_correction;
        if(u.value_by_axis_index(i) > angle_max_correction)
            u.value_by_axis_index(i) = angle_max_correction;
    }
    
    //for integral and derivative
    prev_e = e;
    
    return(u);
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
 
        //cerr << "alpha=" << alpha << endl;

        res = a_prev * (1 - alpha) + a * alpha;
    }

    a_prev = res;
    t_time.set_time();
     
    RVector3D correction = res.angle_from_projections();
    RVector3D moment_of_force;
    moment_of_force.x = correction.x * accelerometer_xi.x;
    moment_of_force.y = correction.y * accelerometer_xi.y;
     
    RVector3D torque_new;
    torque_new.z = 1 / sqrt(1 + pow(moment_of_force.x / 2, 2) + pow(moment_of_force.y / 2, 2));
    torque_new.x =  moment_of_force.y / 2 * torque_new.z;
    torque_new.y = -moment_of_force.x / 2 * torque_new.z;
     
    return(torque_new);
}

RVector3D MotorController::get_angular_velocity_correction(RVector3D angular_velocity, double dt)
{
    static RVector3D prev_e = RVector3D(0, 0, 0),
        e_integral = RVector3D(0, 0, 0);
    
    //requested angular velocity
    RVector3D angular_velocity0 = RVector3D(0, 0, 0);
    
    //difference between requested and current angular velocity
    RVector3D e = angular_velocity0 - angular_velocity;
    
    //discrete derivative
    RVector3D e_derivative = (e - prev_e) / dt;
    
    //discrete integral
    e_integral += e * dt;
    
    //correction
    RVector3D y = e * angular_velocity_Kp + e_integral * angular_velocity_Ki + e_derivative * angular_velocity_Kd;
    
    RVector3D u = y; // reaction
    
    //max and min correction
    for(int i = 0; i < 2; i++)
    {
        if(u.value_by_axis_index(i) < -angular_velocity_max_correction)
            u.value_by_axis_index(i) = -angular_velocity_max_correction;
        if(u.value_by_axis_index(i) > angular_velocity_max_correction)
            u.value_by_axis_index(i) = angular_velocity_max_correction;
    }
    
    //for integral and derivative
    prev_e = e;
    
    return(u);
}

double MotorController::get_speed(RVector3D torque_vec, int motor)
{
    // This comes from the Cubic Vector Model, which one sucks
    /*double res = ( torque_vec.module_sq() + coordinates_of_motors[motor].x * torque_vec.x
                   + coordinates_of_motors[motor].y * torque_vec.y ) / torque_vec.z;

    // force implementation
    res *= get_force();
    */

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

MotorController::MotorController(const int motor_control_pins[N_MOTORS])
{
    angle_Kp = 1;
    angle_Ki = 0;
    angle_Kd = 0;
    
    angular_velocity_Kp = 1;
    angular_velocity_Ki = 0;
    angular_velocity_Kd = 0;
    
    accelerometer_xi.x = 0.5;
    accelerometer_xi.y = 0.5;
    
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
