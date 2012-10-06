#include "Definitions.h"
#include "RVector3D.h"
#include "TimerCount.h"
#include "Accelerometer.h"
#include "Gyroscope.h"
#include "Motor.h"
#include "MotorController.h"
#include "MySerial.h"
#include "PID.h"
#include "VoltageSensor.h"

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
    VoltageSensor* VSensor;

    // pins configuration
    int DefaultMotorPins[4] = {3, 9, 10, 11};
    int DefaultAccelPins[3] = {A0, A1, A2};
    int DefaultVSensorPin = A3;

    //reaction type (different types of processing sensors' data)
    enum reactionType_ {ReactionNone, ReactionAngularVelocity, ReactionAcceleration, ReactionAngle};
    reactionType_ reactionType = ReactionNone;

    // distance from gyroscope to the accelerometer in meters
    RVector3D gyroToAcc = RVector3D(-1.9E-2, -1.7E-2, 2.1E-2);

    // torque corrections
    RVector3D torqueManualCorrection, torqueAutomaticCorrection;

    double anglePeriod = 7.5; // period for low-pass filter for accelerometer
    double angularVelocityPeriod = 1e-5; // period for low-pass filter for gyroscope
    double angleMaxReset = 0.8 * MPI; // to avoid wrong angle values when is is near MPI

    double DefaultVSensorMaxVoltage = 12.6; //maximal voltage (before voltage divider)

    const double g = 9.80665; // gravitational acceleration

    //physical quantities
    RVector3D angle; // angle between Earth's coordinate and ours
    RVector3D accelData; //data from accelerometer (g - a)
    RVector3D angularVelocity; //angular velocity from gyroscope
    RVector3D angularAcceleration; //discrete derivative of angular velocity
    double voltage; //accumulators voltage

    RVector3D angularVelocityPrev = RVector3D(); // for angular acceleration

    //corrections
    RVector3D accelerometerXi;
    static const double angleMaxCorrection = MPI / 4;
    static const double angularVelocityMaxCorrection = MPI / 4;

    PID pidAngle, pidAngularVelocity;
    RVector3D getAngleCorrection(RVector3D angle, double dt);
    RVector3D getAccelerationCorrection(RVector3D angle, RVector3D accelData0); // totally doesnt work
    RVector3D getAngularVelocityCorrection(RVector3D angularVelocity, double dt);

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

    RVector3D getTorques();
};

#endif // QUADROCOPTER_H
