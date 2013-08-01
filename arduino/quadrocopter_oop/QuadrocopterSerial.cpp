#include "Quadrocopter.h"
#include "PID.h"
#include "Definitions.h"

void Quadrocopter::processSerialRx()
{
    if(MSerial->getCommand() == 0)
    {
        MSerial->receiveCommand();

        if(MSerial->getCommand() == 'n')
        {
            MSerial->dropCommand();
            reset();
        }
    }
}

char x[BN + 1];

void Quadrocopter::processSerialTx()
{
    if(MSerial->getCommand() == 'p')
    {
        if(MSerial->bytesAvailable() >= serialReadN)
        {
            // reading

            //myLed.setState(70);
//            for(int i = 0; i < BN; i++)
//                x[i] = MSerial->read();
#ifdef DEBUG_DAC
            myLed.setState(10);
#endif

            {
                // torque_manual_correction +4
                for(int i = 0; i < 2; i++)
                    MSerial->readDouble(-1, 1, angleManualCorrection.valueByAxisIndex(i), 2);

                //force +1
                MController->setForce(MSerial->read() / 100.);

                //reaction_type +1
                reactionType = (reactionType_) (MSerial->read() - '0');

                double tDouble;
                //PID angle coefficients X +6
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngle.setKp_x(tDouble);
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngle.setKi_x(tDouble);
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngle.setKd_x(tDouble);

                //PID angle minmax X +6
                MSerial->readDouble(0, 5, tDouble, 2); pidAngle.setPMinMax_x(tDouble);
                MSerial->readDouble(0, 5, tDouble, 2); pidAngle.setIMinMax_x(tDouble);
                MSerial->readDouble(0, 5, tDouble, 2); pidAngle.setDMinMax_x(tDouble);

                //PID angle coefficients Y +6
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngle.setKp_y(tDouble);
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngle.setKi_y(tDouble);
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngle.setKd_y(tDouble);

                //PID angle minmax Y +6
                MSerial->readDouble(0, 5, tDouble, 2); pidAngle.setPMinMax_y(tDouble);
                MSerial->readDouble(0, 5, tDouble, 2); pidAngle.setIMinMax_y(tDouble);
                MSerial->readDouble(0, 5, tDouble, 2); pidAngle.setDMinMax_y(tDouble);

#ifdef PID_USE_YAW
                //PID angle coefficients Z +6
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngularVelocity.setKp_z(tDouble);
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngularVelocity.setKi_z(tDouble);
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngularVelocity.setKd_z(tDouble);

                //PID angle minmax Z +6
                MSerial->readDouble(0, 5, tDouble, 2); pidAngularVelocity.setPMinMax_z(tDouble);
                MSerial->readDouble(0, 5, tDouble, 2); pidAngularVelocity.setIMinMax_z(tDouble);
                MSerial->readDouble(0, 5, tDouble, 2); pidAngularVelocity.setDMinMax_z(tDouble);
#endif
            }
#ifdef DEBUG_NO_TX_ARDUINO
            Serial.write('x');
#else
            MSerial->bufferInit();
#endif

#ifdef DEBUG_DAC
            myLed.setState(15);
#endif

#ifndef DEBUG_NO_TX_ARDUINO
//            for(int i = 0; i < BN; i++)
//                MSerial->bufferAdd(x[i]);
            // writing bytes
            {
                MSerial->writeDouble(-1, 1, getTorques().x, 1); // +1
                MSerial->writeDouble(-1, 1, getTorques().y, 1); // +1
                MSerial->writeDouble(-1, 1, getTorques().z, 1); // +1

                MSerial->writeDouble(-4, 4, angle.x, 1); // +1
                MSerial->writeDouble(-4, 4, angle.y, 1); // +1

                MSerial->writeDouble(-100, 100, angularVelocity.x, 1); // +1
                MSerial->writeDouble(-100, 100, angularVelocity.y, 1); // +1
                MSerial->writeDouble(-100, 100, angularVelocity.z, 1); // +1

                MSerial->writeDouble(-1, 1, pidAngle.PID_C[0].x, 2); // +1
                MSerial->writeDouble(-1, 1, pidAngle.PID_C[1].x, 2); // +1
                MSerial->writeDouble(-1, 1, pidAngle.PID_C[2].x, 2); // +1

                MSerial->writeDouble(-0.1, 0.1, pidAngle.PID_C[0].y, 1); // +1
                MSerial->writeDouble(-0.1, 0.1, pidAngle.PID_C[1].y, 1); // +1
                MSerial->writeDouble(-0.1, 0.1, pidAngle.PID_C[2].y, 1); // +1
#ifdef PID_USE_YAW
                MSerial->writeDouble(-0.1, 0.1, pidAngularVelocity.PID_C[0].z, 1); // +1
                MSerial->writeDouble(-0.1, 0.1, pidAngularVelocity.PID_C[1].z, 1); // +1
                MSerial->writeDouble(-0.1, 0.1, pidAngularVelocity.PID_C[2].z, 1); // +1
#endif

                //motors
                for (unsigned i = 0; i < 4; i++)
                    MSerial->bufferAdd(100 * MController->getSpeed(getTorques(), i)); // +4

                MSerial->writeDouble(0, 20, voltage, 2); //+2
            }

            MSerial->bufferWrite();
#endif
            MSerial->dropCommand();
        }
    }
    else if(MSerial->getCommand() == 'a'/* || MSerial->isSendAutomaticlyEnabled()*/)
    {
        MSerial->bufferInit();
        MSerial->bufferAdd("A=");
        MSerial->RVector3DWrite(angle * 180 / M_PI, MySerial::PRINT_TAB);
        MSerial->bufferAdd("; S=");
        MSerial->writeNumber(sensorsTime * 1000);
        MSerial->bufferAdd("; B = ");
        MSerial->writeNumber(MPU->bytesAvailableFIFO());
        MSerial->bufferAdd('\n');
        MSerial->bufferWrite();
        MSerial->dropCommand();
        return;
    }
    else MSerial->dropCommand();
//    else if(MSerial->getCommand() == 'h')
//    {
//        MSerial->toggleSendAutomaticly();
//        MSerial->dropCommand();
//    }
//    else if(MSerial->isSendAutomaticlyEnabled())
//    {
//        MSerial->bufferInit();
//        MSerial->bufferAdd('\n');
//        MSerial->bufferWrite();
//        return;
//    }
}
