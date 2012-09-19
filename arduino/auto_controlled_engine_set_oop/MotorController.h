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

    double force;
    
    static const double MIN_SPEED = 0.1;

    enum MOTORS
    {
        A, B, C, D, N_MOTORS
    };

    Motor motors_[N_MOTORS];

    bool use_motors[N_MOTORS];

    RVector3D coordinates_of_motors[N_MOTORS];

public:

    MotorController(const int motor_control_pins[N_MOTORS]);
    ~MotorController();

    double get_speed(RVector3D torque_vec, int motor);

    //set raw speed
    void set_motors(double power[N_MOTORS]); // values in [0...1]

    //torques and force method
    void set_torque(RVector3D torque_vec); //torques
    void set_force(double a); //force

    double get_force();
};

#endif
