#include "Quadrocopter.h"
#include "ComplementaryFilter.cpp"
#include "LowPassFilter.cpp"

void Quadrocopter::processSerialRx()
{
    MSerial->receiveCommand();

    if(MSerial->getCommand() == 'n')
    {
        MSerial->dropCommand();
        reset();
    }
}

bool Quadrocopter::processSerialTx()
{
    if(MSerial->getCommand() == 'p')
    {
        // reading

        // torque_manual_correction
        for(int i = 0; i < 3; i++)
            MSerial->readDouble(-1, 1, angleManualCorrection.valueByAxisIndex(i), 2);

        //force
        MSerial->waitForByte();
        if(MSerial->getReadError()) return(1);

        MController->setForce(MSerial->read() / 100.);

        //reaction_type
        MSerial->waitForByte();
        if(MSerial->getReadError()) return(1);

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
        MSerial->readDouble(0, 100, tDouble, 2); directionalCosines.setPeriod(tDouble);

        MSerial->bufferInit();

        // writing 38 bytes

        MSerial->RVector3D_write(getTorques(), MySerial::PRINT_RAW); // +6
        MSerial->RVector3D_write(angle, MySerial::PRINT_RAW, MySerial::USE_2D); // +4

        MSerial->RVector3D_write(angularVelocity, MySerial::PRINT_RAW); // +6
        MSerial->RVector3D_write(accelData.getValue(), MySerial::PRINT_RAW); // +6

        MSerial->RVector3D_write(angleManualCorrection, MySerial::PRINT_RAW); // +6

        //motors
        for (unsigned i = 0; i < 4; i++)
            MSerial->bufferAdd(100 * MController->getSpeed(getTorques(), i)); // +4

        //dt
        for (int si = 2; si >= 0; si--)
            MSerial->bufferAdd((((unsigned long long) (dt * 1E6)) & (0xff << 8 * si)) >> (8 * si)); //+3

        //reaction type
        MSerial->bufferAdd(reactionType + '0'); //+1

        MSerial->writeDouble(0, 20, voltage, 2); //+2

        MSerial->bufferWrite();
        MSerial->dropCommand();
        return(1);
    }
    else if(MSerial->getCommand() == 'a')
    {
        MSerial->dropCommand();

        MSerial->bufferInit();
        MSerial->bufferAdd("Quadrocopter arduino program");
        MSerial->bufferAdd('\n');
        MSerial->bufferWrite();
        return(1);
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
        return(1);
    }
    return(0);
}
