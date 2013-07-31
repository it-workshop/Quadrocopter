#include "Arduino.h"
#include "Quadrocopter.h"
#include "PID.h"
#include <avr/delay.h>

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
    MPU = new MPU6050DMP;

#ifdef DEBUG_DAC
    myLed = MPU->myLed;
#else
    myLed = InfoLED(13, InfoLED::PWM);
    myLed.setState(0);
#endif

#ifdef DEBUG_FREQ_PIN
    freqLed = InfoLED(DEBUG_FREQ_PIN, InfoLED::DIGITAL);
#endif

#ifdef DEBUG_MPUBYTES_PIN
    mpuBytesLed = InfoLED(DEBUG_MPUBYTES_PIN, InfoLED::DIGITAL);
#endif

    this->reset();

    MPU->initialize();
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

    MPU->resetFIFO();
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

#ifdef DEBUG_FREQ_PIN
    freqLed.changeDigitalState();
#endif

#ifdef DEBUG_MPUBYTES_PIN
    mpuBytesLed.setState(MPU->bytesAvailableFIFO() > 42);
#endif

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
