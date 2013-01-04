#include "Quadrocopter.h"
//#include "ComplementaryFilter.cpp"
#include "LowPassFilter.cpp"

Quadrocopter::Quadrocopter()
{
    MSerial = new MySerial;
    MController = new MotorController(DefaultMotorPins);
    VSensor = new VoltageSensor(DefaultVSensorPin, DefaultVSensorMaxVoltage);
    MPU = new MPU6050DMP;

    this->reset();

    MPU->initialize();
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
    case ReactionAngularVelocity:
        torqueAutomaticCorrection = getAngularVelocityCorrection(angularVelocity, DeltaT.getTimeDifferenceSeconds());
        break;

    case ReactionAcceleration:
        torqueAutomaticCorrection = getAccelerationCorrection(angle, accelDataRaw);
        break;

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
    if(/*MPU->notBusy()*/true)
    {
        dt = DeltaT.getTimeDifferenceSeconds();
        DeltaT.setTime();
        if(dtMax < dt) dtMax = dt;

        processSensorsData();
        processCorrection();
        processMotors();
        processSerialInterrupt();
    }
}

RVector3D Quadrocopter::getTorques()
{
    return(torqueAutomaticCorrection);
}
