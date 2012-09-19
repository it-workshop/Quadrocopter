#include "RVector3D.h"
#include "Motor.h"
#include "Definitions.h"
#include "PID.h"

#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

class MotorController
{
private:

#ifdef DEBUG_NO_MOTORS
    static const int INIT_TIMEOUT = 0; // ms
#else
    static const int INIT_TIMEOUT = 8000; // ms
#endif

    double force;
    
    RVector3D accelerometer_xi;
    
    static const double angle_max_correction = MPI / 4;

    static const double angular_velocity_max_correction = MPI / 4 / 2;
    
    static const double MIN_SPEED = 0.1;

    enum MOTORS
    {
        A, B, C, D, N_MOTORS
    };

    Motor motors_[N_MOTORS];

    bool use_motors[N_MOTORS];

    RVector3D coordinates_of_motors[N_MOTORS];

public:
    PID pid_angle, pid_angular_velocity;

    MotorController(const int motor_control_pins[N_MOTORS]);
    ~MotorController();

    double get_speed(RVector3D torque_vec, int motor);

    //set raw speed
    void set_motors(double power[N_MOTORS]); // values in [0...1]

    //torques and force
    void set_torque(RVector3D torque_vec);

    //force
    double get_force();
    void set_force(double a);
    
    RVector3D get_angle_correction(RVector3D angle, double dt);
    RVector3D get_acceleration_correction(RVector3D angle, RVector3D accel_data); // totally doesnt work
    RVector3D get_angular_velocity_correction(RVector3D angular_velocity, double dt);

    void reset();
};

#endif
