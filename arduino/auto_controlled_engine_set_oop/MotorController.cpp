#include "MotorController.h"
#include "Arduino.h"
#include "TimerCount.h"
#include "RVector3D.h"

double MotorController::getSpeed(RVector3D torqueVec, int motor)
{
    double res = getForce();
    res += torqueVec.x * coordinatesOfMotors[motor].y - torqueVec.y * coordinatesOfMotors[motor].x;

    // it is necessary because the motor controller starts a motor with greater speed than needed
    if (res <= MIN_SPEED && getForce() != 0) res = MIN_SPEED;

    // motors offline
    if(getForce() < MIN_SPEED) res = 0;

    // use_motors implementation
    if(!useMotors[motor]) res = 0;

    return(res);
}

void MotorController::setTorque(RVector3D torqueVec)
{
    for (int i = 0; i < N_MOTORS; i++)
        motors_[i].setPower(getSpeed(torqueVec, i));
}

void MotorController::setMotors(double power[N_MOTORS])
{
    for (int i = 0; i < N_MOTORS; i++)
        motors_[i].setPower(power[i]);
}

MotorController::MotorController(const int motorControlPins[N_MOTORS])
{
    useMotors[A] = 0;
    useMotors[B] = 1;
    useMotors[C] = 0;
    useMotors[D] = 1;
    
    for (int i = 0; i < N_MOTORS; i++)
    {
        motors_[i].setControlPin(motorControlPins[i]);
        motors_[i].setPower(0);
    }

    // wait for ESC
    delay(INIT_TIMEOUT);

    setForce(0);
    setTorque(RVector3D());

    coordinatesOfMotors[A] = RVector3D(1, 0, 0);
    coordinatesOfMotors[B] = RVector3D(0, -1, 0);
    coordinatesOfMotors[C] = RVector3D(-1, 0, 0);
    coordinatesOfMotors[D] = RVector3D(0, 1, 0);
}

double MotorController::getForce()
{
    return(force);
}

void MotorController::setForce(double a)
{
    force = a;
}
