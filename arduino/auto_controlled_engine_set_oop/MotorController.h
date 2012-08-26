#include "RVector3D.h"
#include "Motor.h"
#include "Definitions.h"

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

    static const int DEF_SPEED_STEP = 400;

    double throttle_abs;
    
    RVector3D accelerometer_xi;
    
    static const double angle_max_rotation = MPI / 4;

    static const double angular_velocity_max_rotation = MPI / 4 / 2;
    
    static const double MIN_SPEED_PERCENT = 10;

    enum SIGN
    {
        ZERO = 0,
        PLUS = 1,
        MINUS = -1
    };

    enum MOTORS
    {
        A, B, C, D, N_MOTORS
    };

    Motor motors_[N_MOTORS];

    bool use_motors[N_MOTORS];

    inline const SIGN x_sign(int i);
    inline const SIGN y_sign(int i);

public:
    double angle_Kp, angle_Ki, angle_Kd;
    double angular_velocity_Kp, angular_velocity_Ki, angular_velocity_Kd;

    MotorController(const int motor_control_pins[N_MOTORS]);
    ~MotorController();

    void speedChange(RVector3D throttle_vec);
    double speedGet(RVector3D throttle_vec, int motor);
    //void linearSpeedInc(int inc_percent, int speed_step_time);

    double get_throttle_abs();

    void set_throttle_abs(double a);
    
    RVector3D get_angle_rotation(RVector3D angle, double dt);
    RVector3D get_acceleration_rotation(RVector3D angle, RVector3D accel_data); // totally doesnt work
    RVector3D get_angular_velocity_rotation(RVector3D angular_velocity, double dt);
};

#endif
