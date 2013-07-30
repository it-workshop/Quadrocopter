#include "Quadrocopter.h"
#include "Arduino.h"
#include "PID.h"
#ifndef __arm__
    #include <avr/delay.h>
#endif

Quadrocopter::Quadrocopter()
{
    DefaultVSensorPin = A3;
    reactionType = ReactionNone;

#ifdef DEBUG_DAC
    DefaultMotorPins[0] = 3;
    DefaultMotorPins[1] = 5;
    DefaultMotorPins[2] = 6;
    DefaultMotorPins[3] = 9;
#else
    DefaultMotorPins[0] = 3;
    DefaultMotorPins[1] = 9;
    DefaultMotorPins[2] = 10;
    DefaultMotorPins[3] = 11;
#endif

    MSerial = new MySerial;
    MController = new MotorController(DefaultMotorPins);
    VSensor = new VoltageSensor(DefaultVSensorPin, DefaultVSensorMaxVoltage);
    MyMPU = new MPU6050DMP;

#ifdef DEBUG_DAC
    myLed = MPU->myLed;
#else
    myLed = InfoLED(13, InfoLED::PW);
    myLed.setState(0);
#endif
    this->reset();

    MyMPU->initialize();
    interrupts();
}

void Quadrocopter::reset()
{
    angle = RVector3D();
    torqueAutomaticCorrection = RVector3D();
    angleManualCorrection = RVector3D();

    MController->setForce(0);
    MController->setTorque(RVector3D());

    pidAngle.reset();

    voltage = 0;
    dtMax = 0;

    pidAngle.setKpKiKd(0, 0, 0);
    pidAngle.setYMin(-angleMaxCorrection);
    pidAngle.setYMax(angleMaxCorrection);

    pidAngle.setPMin(-angleMaxCorrection * 5);
    pidAngle.setPMax( angleMaxCorrection * 5);

    pidAngle.setIMin(-angleMaxCorrection);
    pidAngle.setIMax( angleMaxCorrection);

    pidAngle.setDMin(-angleMaxCorrection * 1.5);
    pidAngle.setDMax( angleMaxCorrection * 1.5);
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

    if(MyMPU->getNewData()) // on each MPU data packet
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
            MyMPU->iteration();
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

        MyMPU->resetNewData();
    }
}

void Quadrocopter::MPUInterrupt()
{
    MyMPU->processInterrupt();
}

RVector3D Quadrocopter::getTorques()
{
    return(torqueAutomaticCorrection);
}
