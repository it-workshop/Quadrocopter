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

#ifdef DEBUG_DAC
    myLed = MPU->myLed;
#else
    myLed = InfoLED(13, InfoLED::PWM);
    myLed.setState(0);
#endif
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
#ifdef DEBUG_DAC
        myLed.setState(0);
#endif

        { // Serial
            processSerialRx();
            myLed.setState(5);
            processSerialTx();
        }

#ifdef DEBUG_DAC
        myLed.setState(20);
#endif

        tCount.setTime();
        { // Sensors
            MPU->iteration();
            processSensorsData();
        }
        sensorsTime = tCount.getTimeDifferenceSeconds();

#ifdef DEBUG_DAC
        myLed.setState(80);
#endif

        tCount.setTime();
        { // Corrections, Motors
            dt = DeltaT.getTimeDifferenceSeconds();

            processCorrection();
            DeltaT.setTime();
            processMotors();
        }
        calculationsTime = tCount.getTimeDifferenceSeconds();


#ifdef DEBUG_DAC
        myLed.setState(100);
#endif

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
