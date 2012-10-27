#include "Quadrocopter.h"

Quadrocopter::Quadrocopter()
{
    MSerial = new MySerial;
    MController = new MotorController(DefaultMotorPins);
    Accel = new Accelerometer(DefaultAccelPins);
    Gyro = new Gyroscope();
    VSensor = new VoltageSensor(DefaultVSensorPin, DefaultVSensorMaxVoltage);

    this->reset();
}

void Quadrocopter::reset()
{
    angle = RVector3D();
    angularAcceleration = RVector3D();
    accelData = RVector3D();
    accelDataFiltered = RVector3D();
    torqueAutomaticCorrection = RVector3D();
    torqueManualCorrection = RVector3D();

    MController->setForce(0);
    MController->setTorque(RVector3D());

    pidAngle.reset();
    pidAngularVelocity.reset();

    accelerometerXi = RVector3D(0, 0, 0);

    voltage = 0;

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
        torqueAutomaticCorrection = getAccelerationCorrection(angle, accelData);
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
    dt = DeltaT.getTimeDifferenceSeconds();
    DeltaT.setTime();

    processSensorsData();
    processCorrection();
    processSerialRx();
    processMotors();
    processSerialTx();
}

RVector3D Quadrocopter::getTorques()
{
    RVector3D res;

    if(reactionType == ReactionAngularVelocity)
        res += torqueManualCorrection;

    res += torqueAutomaticCorrection;

    return(res);
}
