#include "Quadrocopter.h"
#include "ComplementaryFilter.cpp"
#include "LowPassFilter.cpp"

void Quadrocopter::processSerialCommand()
{
    if(MSerial->getCommand() == 'n')
    {
        MSerial->dropCommand();
        reset();
    }
    else if(MSerial->getCommand() == 'a')
    {
        MSerial->dropCommand();
        MSerial->bufferInit();
        MSerial->bufferAdd("Quadrocopter arduino program\n");
        MSerial->RVector3DWrite(angle * 180 / 3.1415, MySerial::PRINT_TAB);
        MSerial->bufferAdd('\n');
        MSerial->bufferWrite();
        MSerial->dropCommand();
    }
    else if(MSerial->getCommand() == 'h')
    {
        MSerial->toggleSendAutomaticly();
//        MSerial->dropCommand();
    }
    else if(MSerial->isSendAutomaticlyEnabled())
    {
        MSerial->bufferInit();
        MSerial->RVector3DWrite(angle * 180 / 3.1415, MySerial::PRINT_TAB);
        MSerial->bufferAdd('\n');
        MSerial->bufferWriteN();
    }

    else if(MSerial->getCommand() == 'p'
            && MSerial->bytesAvailable() >= serialReadN)
    {
        // reading 24 bytes

        // torque_manual_correction (+6)
        for(int i = 0; i < 3; i++)
            MSerial->readDouble(-1, 1, angleManualCorrection.valueByAxisIndex(i), 2);

        //force (+1)
        MController->setForce(MSerial->read() / 100.);

        //reaction_type (+1)
        reactionType = (reactionType_) (MSerial->read() - '0');

        double tDouble;
        //PID angle coefficients (+6)
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngle.setKp(tDouble);
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngle.setKi(tDouble);
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngle.setKd(tDouble);

        //PID angular velocity coefficients (+6)
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngularVelocity.setKp(tDouble);
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngularVelocity.setKi(tDouble);
        MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngularVelocity.setKd(tDouble);

        //Periods for filters (+4)
        MSerial->readDouble(0, 100, tDouble, 2); accelData.setPeriod(tDouble);
        MSerial->readDouble(0, 100, tDouble, 2);


        // writing 38 bytes
        MSerial->bufferInit();

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
            //MSerial->bufferAdd((((unsigned long long) (dt * 1E6)) & (0xff << 8 * si)) >> (8 * si)); //+3
            MSerial->bufferAdd((((unsigned long long) (dtMax * 1E6)) & (0xff << 8 * si)) >> (8 * si)); //+3

        //reaction type
        MSerial->bufferAdd(reactionType + '0'); //+1

        MSerial->writeDouble(0, 20, voltage, 2); //+2

        MSerial->bufferWrite();
        MSerial->dropCommand();
    }
    else
    {
        MSerial->dropCommand();
        MSerial->flush();
    }
}

void Quadrocopter::processSerialInterrupt()
{
    if(MSerial->getCommand())
        processSerialCommand();
    else
        MSerial->receiveCommand();
}
