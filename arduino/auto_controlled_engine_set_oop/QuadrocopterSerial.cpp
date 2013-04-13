#include "Quadrocopter.h"
#include "ComplementaryFilter.h"
#include "LowPassFilter.h"
#include "PID.h"

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
            MSerial->readDouble(-1, 1, angleManualCorrection.valueByAxisIndex(i), 2);

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

        //Periods for filters
        MSerial->readDouble(0, 100, tDouble, 2); accelData.setPeriod(tDouble);
        MSerial->readDouble(0, 100, tDouble, 2);

        MSerial->bufferInit();

        // writing 38 bytes

        MSerial->RVector3DWrite(getTorques(), MySerial::PRINT_RAW); // +6
        MSerial->RVector3DWrite(angle, MySerial::PRINT_RAW, MySerial::USE_2D); // +4

        MSerial->RVector3DWrite(angularVelocity, MySerial::PRINT_RAW); // +6
        MSerial->RVector3DWrite(accelData.getValue(), MySerial::PRINT_RAW); // +6

        MSerial->RVector3DWrite(angleManualCorrection, MySerial::PRINT_RAW); // +6

        //motors
        for (unsigned i = 0; i < 4; i++)
            MSerial->bufferAdd(100 * MController->getSpeed(getTorques(), i)); // +4

        //dt
        for (int si = 2; si >= 0; si--)
            MSerial->bufferAdd((((unsigned long long) ((calculationsTime + sensorsTime) * 1E6)) & (0xff << 8 * si)) >> (8 * si)); //+3

        //reaction type
        MSerial->bufferAdd(reactionType + '0'); //+1

        MSerial->writeDouble(0, 20, voltage, 2); //+2

        MSerial->bufferWrite();
        MSerial->dropCommand();
        return;
    }
    else if(MSerial->getCommand() == 'a' || MSerial->isSendAutomaticlyEnabled())
    {
        MSerial->dropCommand();

        MSerial->bufferInit();
        MSerial->bufferAdd("Quadrocopter arduino program; angle is ");
        MSerial->RVector3DWrite(angle * 180 / M_PI, MySerial::PRINT_TAB);
        MSerial->bufferAdd("; sensorsTime = ");
        MSerial->writeNumber(sensorsTime * 1000);
        MSerial->bufferAdd(" ms; calculationsTime = ");
        MSerial->writeNumber(calculationsTime * 1000);
        MSerial->bufferAdd(" ms");
        MSerial->bufferAdd('\n');
        MSerial->bufferWrite();
        return;
    }
    else if(MSerial->getCommand() == 'h')
    {
        MSerial->toggleSendAutomaticly();
        MSerial->dropCommand();
    }
//    else if(MSerial->getCommand() == 'i')
//    {
//        MPU->attachFIFOInterrupt();
//        MSerial->dropCommand();
//    }
//    else if(MSerial->getCommand() == 'm')
//    {
//        MPU->iteration();
//        MSerial->dropCommand();
//    }
    else if(MSerial->isSendAutomaticlyEnabled())
    {
        MSerial->bufferInit();
        MSerial->bufferAdd('\n');
        MSerial->bufferWrite();
        return;
    }
}
