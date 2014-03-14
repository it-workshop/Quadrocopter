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
#include "PWMJoystick.h"

#ifdef USE_COMPASS
    // i2cdevlib
    #include <HMC5883L.h>
#endif

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
    MPU6050DMP* MyMPU;
#ifdef USE_COMPASS
    HMC5883L* MyCompass;
#endif
    PWMJoystick* Joystick;

    // pins configuration

    int DefaultMotorPins[4];

    int DefaultVSensorPin;

    //reaction type (different types of processing sensors' data)
    enum reactionType_ {ReactionNone, ReactionAngularVelocity, ReactionAcceleration, ReactionAngle};
    reactionType_ reactionType;

    // torque corrections
    RVector3D torqueAutomaticCorrection;

    RVector3D angleManualCorrection;

    static const double DefaultVSensorMaxVoltage = 17.95; //maximal voltage (before voltage divider)
    //15.16? 11.95->2.6

    static const double g = 9.80665; // gravitational acceleration

    //physical quantities
    RVector3D angle; // angle between Earth's coordinate and ours (filtered)
    RVector3D angularVelocity; // angular velocity from gyroscope
    double voltage; //accumulators voltage

#ifdef USE_COMPASS
    double copterHeading;
    double joystickHeading;
    RVector3D BMag;

    //temp mag variables
    int16_t magX, magY, magZ;
#endif

    //corrections
    static const double angleMaxCorrection = MPI / 4;
    static const double angularVelocityMaxCorrection = MPI / 4;

    PID pidAngleX, pidAngleY;

#ifdef PID_USE_YAW
    PID pidAngularVelocityZ;
#endif

#ifdef PID_USE_YAW_ANGLE
    PID pidAngleZ;
#endif

    RVector3D getAngleCorrection(RVector3D angle, double dt);

    double dt, dtMax, sensorsTime, calculationsTime;
    TimerCount tCount;

    InfoLED myLed;

    double forceOverrideValue;
    bool forceOverride;

    RVector3D angleOffsetPC;

#ifdef DEBUG_FREQ_PIN
    InfoLED freqLed;
#endif

#ifdef DEBUG_MPUBYTES_PIN
    InfoLED mpuBytesLed;
#endif

    // bytes to read
    // Defines in Quadrocopter.cpp
    unsigned int serialReadN;
    bool needPCTx;

public:
    Quadrocopter();

    void reset();

    void processSensorsData();
    void processCorrection();
    void processMotors();

    void processSerialGetCommand();
    void processSerialDoCommand();

    void processSerialPCRx();
    void processSerialPCTx();

    void processSerialTextTx();

    void processJoystickRx();

    void iteration();
    void MPUInterrupt();

    RVector3D getTorques();
};

#endif // QUADROCOPTER_H
