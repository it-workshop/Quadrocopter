#include "Quadrocopter.h"

void Quadrocopter::processSerialRx()
{
    MSerial->receiveCommand();

    if(MSerial->getCommand() == 'n')
    {
        MSerial->dropCommand();
        reset();
    }
}

void Quadrocopter::processSerialTx()
{
    if(MSerial->getCommand() == 'p')
    {
        const double serial_gyroscope_coefficient = 0.08;

        // reading

        // torque_manual_correction
        for(int i = 0; i < 3; i++)
            MSerial->readDouble(-1, 1, torqueManualCorrection.value_by_axis_index(i), 2);

        //force
        MSerial->waitForByte();
        if(MSerial->getReadError()) return;

        MController->set_force(MSerial->read() / 100.);

        //reaction_type
        MSerial->waitForByte();
        if(MSerial->getReadError()) return;

        reactionType = (reactionType_) (MSerial->read() - '0');

        double tDouble;
        //PID angle coefficients
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pid_angle.set_Kp(tDouble);
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pid_angle.set_Ki(tDouble);
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pid_angle.set_Kd(tDouble);

        //PID angular velocity coefficients
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pid_angular_velocity.set_Kp(tDouble);
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pid_angular_velocity.set_Ki(tDouble);
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pid_angular_velocity.set_Kd(tDouble);

        MSerial->bufferInit();

        // writing 28 bytes

        MSerial->RVector3D_write(torqueAutomaticCorrection + torqueManualCorrection, MySerial::PRINT_RAW);
        MSerial->RVector3D_write(angle, MySerial::PRINT_RAW, MySerial::USE_2D);

        MSerial->RVector3D_write((angularVelocity * serial_gyroscope_coefficient), MySerial::PRINT_RAW);
        MSerial->RVector3D_write(accelData, MySerial::PRINT_RAW);
        MSerial->RVector3D_write(angularVelocityCorrection, MySerial::PRINT_RAW);
        MSerial->RVector3D_write(accelerationCorrection, MySerial::PRINT_RAW);
        MSerial->RVector3D_write(angleCorrection, MySerial::PRINT_RAW);

        //motors
        for (unsigned i = 0; i < 4; i++)
            MSerial->bufferAdd(100 * MController->get_speed(torqueAutomaticCorrection + torqueManualCorrection, i));

        //dt
        for (int si = 2; si >= 0; si--)
            MSerial->bufferAdd((((unsigned long long) (dt * 1E6)) & (0xff << 8 * si)) >> (8 * si));

        //reaction type
        MSerial->bufferAdd(reactionType + '0');

        MSerial->bufferWrite();
        MSerial->dropCommand();
    }
}
