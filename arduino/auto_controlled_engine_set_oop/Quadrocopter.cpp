#include "Arduino.h"
#include "LowPassFilter.h"
#include "Quadrocopter.h"
#include "PID.h"
#include <avr/delay.h>

Quadrocopter::Quadrocopter()
{
    MSerial = new MySerial;
    MController = new MotorController(DefaultMotorPins);
    VSensor = new VoltageSensor(DefaultVSensorPin, DefaultVSensorMaxVoltage);
    MPU = new MPU6050DMP;

    //myLed = InfoLED(A0, InfoLED::);
    //myLed.setState(0);
    myLed = MPU->myLed;

    this->reset();

    MPU->initialize();
    interrupts();
}

void Quadrocopter::reset()
{
    angle = RVector3D();
    angularAcceleration = RVector3D();
    accelDataRaw = RVector3D();
    accelData.reset();
    torqueAutomaticCorrection = RVector3D();
    angleManualCorrection = RVector3D();

    MController->setForce(0);
    MController->setTorque(RVector3D());

    pidAngle.reset();
    pidAngularVelocity.reset();

    accelerometerXi = RVector3D(0, 0, 0);

    voltage = 0;
    dtMax = 0;

    pidAngle.setKpKiKd(0, 0, 0);
    pidAngularVelocity.setKpKiKd(0, 0, 0);
    pidAngle.setYMin(-angleMaxCorrection);
    pidAngularVelocity.setYMin(-angularVelocityMaxCorrection);
    pidAngle.setYMax(angleMaxCorrection);
    pidAngularVelocity.setYMax(angularVelocityMaxCorrection);
}

void Quadrocopter::processCorrection()
{
    torqueAutomaticCorrection = RVector3D();

    switch(reactionType)
    {
    //    case ReactionAngularVelocity:
    //        torqueAutomaticCorrection = getAngularVelocityCorrection(angularVelocity, DeltaT.getTimeDifferenceSeconds());
    //        break;

    //    case ReactionAcceleration:
    //        torqueAutomaticCorrection = getAccelerationCorrection(angle, accelDataRaw);
    //        break;

    case ReactionAngle:
        torqueAutomaticCorrection = getAngleCorrection(angle, DeltaT.getTimeDifferenceSeconds());
        break;
    }
}

void Quadrocopter::processMotors()
{
    MController->setTorque(getTorques());
}

void Quadrocopter::iteration()
{
    if(dtMax < dt) dtMax = dt;

    if(MPU->getNewData()) // on each MPU data packet
    {
        myLed.setState(0);

        { // Serial
            processSerialRx();
            myLed.setState(10);
            processSerialTx();
        }

        myLed.setState(20);

        tCount.setTime();
        { // Corrections, Motors
            dt = DeltaT.getTimeDifferenceSeconds();

            processCorrection();
            DeltaT.setTime();
            processMotors();
        }
        calculationsTime = tCount.getTimeDifferenceSeconds();
        myLed.setState(30);

        tCount.setTime();
        { // Sensors
            MPU->iteration();
            processSensorsData();
        }
        sensorsTime = tCount.getTimeDifferenceSeconds();
        myLed.setState(100);

        MPU->resetNewData();
    }
}

void Quadrocopter::MPUInterrupt()
{
    MPU->processInterrupt();
}

RVector3D Quadrocopter::getTorques()
{
    return(torqueAutomaticCorrection);
}
