#include "Definitions.h"
#include "RVector3D.h"
#include "TimerCount.h"
#include "Accelerometer.h"
#include "Gyroscope.h"
#include "Motor.h"
#include "MotorController.h"
#include "MySerial.h"
#include "PID.h"

#ifndef QUADROCOPTER_H
#define QUADROCOPTER_H

class Quadrocopter
{
private:
    MotorController* MController;
    Accelerometer* Accel;
    Gyroscope* Gyro;
    TimerCount DeltaT;
    MySerial* MSerial;

    //reaction type (different types of processing sensors' data)
    enum reactionType_ {ReactionNone, ReactionAngularVelocity, ReactionAcceleration, ReactionAngle};
    reactionType_ reactionType = ReactionNone;

    // distance from gyroscope to the accelerometer in meters
    RVector3D gyroToAcc = RVector3D(-1.9E-2, -1.7E-2, 2.1E-2);

    // torque corrections
    RVector3D torqueManualCorrection, torqueAutomaticCorrection;

    double anglePeriod = 4; // period for low-pass filter for accelerometer

    const double g = 9.80665; // gravitational acceleration

    //physical quantities
    RVector3D angle; // angle between Earth's coordinate and ours
    RVector3D accelData; //data from accelerometer (g - a)
    RVector3D angularVelocity; //angular velocity from gyroscope
    RVector3D angularAcceleration; //discrete derivative of angular velocity

    RVector3D angularVelocityPrev = RVector3D(); // for angular acceleration

    //corrections
    RVector3D accelerometerXi;
    static const double angle_max_correction = MPI / 4;
    static const double angular_velocity_max_correction = MPI / 4 / 2;

    RVector3D angularVelocityCorrection, accelerationCorrection, angleCorrection;

    PID pid_angle, pid_angular_velocity;
    RVector3D get_angle_correction(RVector3D angle, double dt);
    RVector3D get_acceleration_correction(RVector3D angle, RVector3D accel_data); // totally doesnt work
    RVector3D get_angular_velocity_correction(RVector3D angularVelocity, double dt);

    double dt;

public:
    Quadrocopter();

    void reset();

    void processSensorsData();
    void processCorrection();
    void processMotors();

    void processSerialRx();
    void processSerialTx();

    void iteration();
};

#endif // QUADROCOPTER_H
