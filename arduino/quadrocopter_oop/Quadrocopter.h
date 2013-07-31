#include "Definitions.h"
#include "MPU6050DMP.h"
#include "RVector3D.h"
#include "TimerCount.h"
#include "Motor.h"
#include "MotorController.h"
#include "MySerial.h"
#include "PID.h"
#include "InfoLED.h"
#include "VoltageSensor.h"

#ifndef QUADROCOPTER_H
#define QUADROCOPTER_H

#define BN 60

class Quadrocopter
{
private:
    MotorController* MController;
    TimerCount DeltaT;
    MySerial* MSerial;
    VoltageSensor* VSensor;
    MPU6050DMP* MPU;

    // pins configuration

    int DefaultMotorPins[4];

    int DefaultVSensorPin;

    //reaction type (different types of processing sensors' data)
    enum reactionType_ {ReactionNone, ReactionAngularVelocity, ReactionAcceleration, ReactionAngle};
    reactionType_ reactionType;

    // torque corrections
    RVector3D torqueAutomaticCorrection;

    RVector3D angleManualCorrection;

    static const double DefaultVSensorMaxVoltage = 14.2; //maximal voltage (before voltage divider)

    static const double g = 9.80665; // gravitational acceleration

    //physical quantities
    RVector3D angle; // angle between Earth's coordinate and ours (filtered)
    RVector3D angularVelocity; // angular velocity from gyroscope
    double voltage; //accumulators voltage

    //corrections
    static const double angleMaxCorrection = MPI / 4;

    PID<RVector3D> pidAngle;
    RVector3D getAngleCorrection(RVector3D angle, double dt);

    double dt, dtMax, sensorsTime, calculationsTime;
    TimerCount tCount;

    InfoLED myLed;

#ifdef DEBUG_FREQ_PIN
    InfoLED freqLed;
#endif

    static const unsigned int serialReadN = 30; // bytes to read

public:
    Quadrocopter();

    void reset();

    void processSensorsData();
    void processCorrection();
    void processMotors();

    void processSerialRx();
    void processSerialTx();

    void iteration();
    void MPUInterrupt();

    RVector3D getTorques();
};

#endif // QUADROCOPTER_H
