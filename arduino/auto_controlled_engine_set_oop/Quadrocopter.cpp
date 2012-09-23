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
    angleCorrection = RVector3D();
    angularVelocityCorrection = RVector3D();
    angularAcceleration = RVector3D();
    accelData = RVector3D();
    torqueAutomaticCorrection = RVector3D();
    torqueManualCorrection = RVector3D();

    MController->set_force(0);
    MController->set_torque(RVector3D());

    pid_angle.reset();
    pid_angular_velocity.reset();

    accelerometerXi = RVector3D(0.5, 0.5, 0);

    pid_angle.set_KpKiKd(0.2, 0, 0.1);
    pid_angular_velocity.set_KpKiKd(0.2, 0, 0.1);
    pid_angle.set_y_min(- MPI / 4);
    pid_angular_velocity.set_y_min(- MPI / 4);
    pid_angle.set_y_max(MPI / 4);
    pid_angular_velocity.set_y_max(MPI / 4);
}

void Quadrocopter::processSensorsData()
{
    accelData = Accel->getReadings();
    angularVelocity = Gyro->getReadings();

    if(DeltaT.getTimeIsset())
    {
        angularAcceleration = (angularVelocity - angularVelocityPrev) / dt;
        /*accelData.x -= (angularAcceleration.y * gyroToAcc.z - angularAcceleration.z * gyroToAcc.y) / g;
        accelData.y -= (angularAcceleration.z * gyroToAcc.x - angularAcceleration.x * gyroToAcc.z) / g;
        accelData.z -= (angularAcceleration.x * gyroToAcc.y - angularAcceleration.y * gyroToAcc.x) / g;*/

        RVector3D accelAngle = accelData.angle_from_projections();

        // alpha coefficient for low-pass filter
        double angleAlpha = dt / (dt + anglePeriod / (2 * MPI));

        // low-pass filter
        angle.x = (angle.x + angularVelocity.x * dt) * (1 - angleAlpha) + accelAngle.x * angleAlpha;
        angle.y = (angle.y + angularVelocity.y * dt) * (1 - angleAlpha) + accelAngle.y * angleAlpha;

        // sometimes some stuff happen
        for(unsigned i = 0; i < 2; i++)
            if (!(angle.value_by_axis_index(i) >= -MPI && angle.value_by_axis_index(i) <= MPI))
                angle.value_by_axis_index(i) = 0;
    }

    angularVelocityPrev = angularVelocity;
}

void Quadrocopter::processCorrection()
{
    torqueAutomaticCorrection = RVector3D();

    angularVelocityCorrection = get_angular_velocity_correction(angularVelocity, DeltaT.getTimeDifferenceSeconds());
    accelerationCorrection = get_acceleration_correction(angle, accelData);
    angleCorrection = get_angle_correction(angle, DeltaT.getTimeDifferenceSeconds());

    switch(reactionType)
    {
    case ReactionAngularVelocity:
        torqueAutomaticCorrection = angularVelocityCorrection;
        break;

    case ReactionAcceleration:
        torqueAutomaticCorrection = accelerationCorrection;
        break;

    case ReactionAngle:
        torqueAutomaticCorrection = angleCorrection;
        break;
    }
}

void Quadrocopter::processMotors()
{
    MController->set_torque(get_torques());
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

RVector3D Quadrocopter::get_torques()
{
    RVector3D res;

    if(reactionType == ReactionAngularVelocity)
        res += torqueManualCorrection;

    res += torqueAutomaticCorrection;

    return(res);
}
