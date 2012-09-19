#include "MotorController.h"
#include "Arduino.h"
#include "TimerCount.h"
#include "RVector3D.h"

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
        motors_[i].setPower(get_speed(torque_vec, i));
}

void MotorController::set_motors(double power[N_MOTORS])
{
    for (int i = 0; i < N_MOTORS; i++)
        motors_[i].setPower(power[i]);
}

MotorController::MotorController(const int motor_control_pins[N_MOTORS])
{
    use_motors[A] = 1;
    use_motors[B] = 0;
    use_motors[C] = 1;
    use_motors[D] = 0;
    
    for (int i = 0; i < N_MOTORS; i++)
    {
        motors_[i].setControlPin(motor_control_pins[i]);
        motors_[i].setPower(0);
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
