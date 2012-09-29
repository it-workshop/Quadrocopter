#include "Quadrocopter.h"

Quadrocopter::Quadrocopter()
{
    MSerial = new MySerial;

    const int DefaultMotorPins[4] = {3, 9, 10, 11};
    MController = new MotorController(DefaultMotorPins);

    int DefaultAccelPins[3] = {A0, A1, A2};
    Accel = new Accelerometer(DefaultAccelPins);

    Gyro = new Gyroscope();

    this->reset();
}

void Quadrocopter::reset()
{
    angle = RVector3D();
    angularAcceleration = RVector3D();
    accelData = RVector3D();
    torqueAutomaticCorrection = RVector3D();
    torqueManualCorrection = RVector3D();

    MController->setForce(0);
    MController->setTorque(RVector3D());

    pidAngle.reset();
    pidAngularVelocity.reset();

    accelerometerXi = RVector3D(0, 0, 0);

    pidAngle.setKpKiKd(0.2, 0, 0.1);
    pidAngularVelocity.setKpKiKd(0.2, 0, 0.1);
    pidAngle.setYMin(- MPI / 4);
    pidAngularVelocity.setYMin(- MPI / 4);
    pidAngle.setYMax(MPI / 4);
    pidAngularVelocity.setYMax(MPI / 4);
}

void Quadrocopter::processSensorsData()
{
    if(reactionType == ReactionAngle || reactionType == ReactionAcceleration)
        accelData = Accel->getReadings();
    else accelData = RVector3D();

    if(reactionType != ReactionNone)
    {
        double angularVelocityAlpha = dt / (dt + angularVelocityPeriod / (2 * MPI));
        angularVelocity = angularVelocity * (1 - angularVelocityAlpha) + Gyro->getReadings() * angularVelocityAlpha;
    }
    else angularVelocity = RVector3D();

    if(reactionType == ReactionAngle)
    {
        if(DeltaT.getTimeIsset())
        {
            /*angularAcceleration = (angularVelocity - angularVelocityPrev) / dt;
            accelData.x -= (angularAcceleration.y * gyroToAcc.z - angularAcceleration.z * gyroToAcc.y) / g;
            accelData.y -= (angularAcceleration.z * gyroToAcc.x - angularAcceleration.x * gyroToAcc.z) / g;
            accelData.z -= (angularAcceleration.x * gyroToAcc.y - angularAcceleration.y * gyroToAcc.x) / g;*/


            RVector3D accelAngle = accelData.angleFromProjections();

            // alpha coefficient for low-pass filter
            double angleAlpha = dt / (dt + anglePeriod / (2 * MPI));

            // low-pass filter
            angle.x = (angle.x + angularVelocity.x * dt) * (1 - angleAlpha) + accelAngle.x * angleAlpha;
            angle.y = (angle.y + angularVelocity.y * dt) * (1 - angleAlpha) + accelAngle.y * angleAlpha;

            // sometimes some stuff happen
            for(unsigned i = 0; i < 2; i++)
                if (!(angle.valueByAxisIndex(i) >= -MPI && angle.valueByAxisIndex(i) <= MPI))
                    angle.valueByAxisIndex(i) = 0;
        }

        angularVelocityPrev = angularVelocity;
    }
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
