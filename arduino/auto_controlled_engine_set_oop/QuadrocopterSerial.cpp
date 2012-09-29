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
        // reading

        // torque_manual_correction
        for(int i = 0; i < 3; i++)
            MSerial->readDouble(-1, 1, torqueManualCorrection.valueByAxisIndex(i), 2);

        //force
        MSerial->waitForByte();
        if(MSerial->getReadError()) return;

        MController->setForce(MSerial->read() / 100.);

        //reaction_type
        MSerial->waitForByte();
        if(MSerial->getReadError()) return;

        reactionType = (reactionType_) (MSerial->read() - '0');

        double tDouble;
        //PID angle coefficients
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngle.setKp(tDouble);
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngle.setKi(tDouble);
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngle.setKd(tDouble);

        //PID angular velocity coefficients
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngularVelocity.setKp(tDouble);
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngularVelocity.setKi(tDouble);
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngularVelocity.setKd(tDouble);

        MSerial->bufferInit();

        // writing 36 bytes

        MSerial->RVector3D_write(getTorques(), MySerial::PRINT_RAW); // +6
        MSerial->RVector3D_write(angle, MySerial::PRINT_RAW, MySerial::USE_2D); // +4

        MSerial->RVector3D_write(angularVelocity, MySerial::PRINT_RAW); // +6
        MSerial->RVector3D_write(accelData, MySerial::PRINT_RAW); // +6

        MSerial->RVector3D_write(torqueManualCorrection, MySerial::PRINT_RAW); // +6

        //motors
        for (unsigned i = 0; i < 4; i++)
            MSerial->bufferAdd(100 * MController->getSpeed(getTorques(), i)); // +4

        //dt
        for (int si = 2; si >= 0; si--)
            MSerial->bufferAdd((((unsigned long long) (dt * 1E6)) & (0xff << 8 * si)) >> (8 * si)); //+3

        //reaction type
        MSerial->bufferAdd(reactionType + '0'); //+1

        MSerial->bufferWrite();
        MSerial->dropCommand();
    }
    else if(MSerial->getCommand() == 'a')
    {
        MSerial->dropCommand();

        MSerial->bufferInit();
        MSerial->bufferAdd("Quadrocopter arduino program");
        MSerial->bufferAdd('\n');
        MSerial->bufferWrite();
    }
    else if(MSerial->getCommand() == 'h')
    {
        MSerial->toggleSendAutomaticly();
        MSerial->dropCommand();
    }
    else if(MSerial->isSendAutomaticlyEnabled())
    {
        MSerial->bufferInit();
        MSerial->RVector3D_write(Accel->getRawReadings(), MySerial::PRINT_TAB);
        MSerial->bufferAdd('\n');
        MSerial->bufferWriteN();
    }
}
