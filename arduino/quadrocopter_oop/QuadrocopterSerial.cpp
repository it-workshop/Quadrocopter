#include "Quadrocopter.h"
#include "PID.h"

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
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngleX.Kp = (tDouble);
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngleX.Ki = (tDouble);
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngleX.Kd = (tDouble);

                //PID angle minmax X +6
                MSerial->readDouble(0, 5, tDouble, 2); pidAngleX.setPMinMax(tDouble);
                MSerial->readDouble(0, 5, tDouble, 2); pidAngleX.setIMinMax(tDouble);
                MSerial->readDouble(0, 5, tDouble, 2); pidAngleX.setDMinMax(tDouble);

                //PID angle coefficients Y +6
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngleY.Kp = (tDouble);
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngleY.Ki = (tDouble);
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngleY.Kd = (tDouble);

                //PID angle minmax Y +6
                MSerial->readDouble(0, 5, tDouble, 2); pidAngleY.setPMinMax(tDouble);
                MSerial->readDouble(0, 5, tDouble, 2); pidAngleY.setIMinMax(tDouble);
                MSerial->readDouble(0, 5, tDouble, 2); pidAngleY.setDMinMax(tDouble);

#ifdef PID_USE_YAW
                //PID AngularVelocityZ coefficients Z +6
                MSerial->readDouble(-0.5, 0.5, tDouble, 2); pidAngularVelocityZ.Kp = (tDouble);
                MSerial->readDouble(-0.5, 0.5, tDouble, 2); pidAngularVelocityZ.Ki = (tDouble);
                MSerial->readDouble(-0.5, 0.5, tDouble, 2); pidAngularVelocityZ.Kd = (tDouble);

                //PID AngularVelocityZ minmax Z +6
                MSerial->readDouble(0, 5, tDouble, 2); pidAngularVelocityZ.setPMinMax(tDouble);
                MSerial->readDouble(0, 5, tDouble, 2); pidAngularVelocityZ.setIMinMax(tDouble);
                MSerial->readDouble(0, 5, tDouble, 2); pidAngularVelocityZ.setDMinMax(tDouble);
#endif

#ifdef PID_USE_YAW_ANGLE
                //PID angle coefficients Z +6
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngleZ.Kp = (tDouble);
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngleZ.Ki = (tDouble);
                MSerial->readDouble(-1.5, 1.5, tDouble, 2); pidAngleZ.Kd = (tDouble);

                //PID angle minmax Z +6
                MSerial->readDouble(0, 5, tDouble, 2); pidAngleZ.setPMinMax(tDouble);
                MSerial->readDouble(0, 5, tDouble, 2); pidAngleZ.setIMinMax(tDouble);
                MSerial->readDouble(0, 5, tDouble, 2); pidAngleZ.setDMinMax(tDouble);
#endif

#ifdef USE_COMPASS
                // joystick heading +2
                MSerial->readDouble(0, 7, joystickHeading, 2);
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
                MSerial->writeDouble(-0.5, 0.5, getTorques().x, 1); // +1
                MSerial->writeDouble(-0.5, 0.5, getTorques().y, 1); // +1
                MSerial->writeDouble(-0.5, 0.5, getTorques().z, 1); // +1

                MSerial->RVector3DWrite(angle, MySerial::PRINT_RAW, MySerial::USE_2D); // +4

                MSerial->writeDouble(-100, 100, angularVelocity.x, 1); // +1
                MSerial->writeDouble(-100, 100, angularVelocity.y, 1); // +1
                MSerial->writeDouble(-100, 100, angularVelocity.z, 1); // +1

                MSerial->writeDouble(-0.1, 0.1, pidAngleX.P, 1); // +1
                MSerial->writeDouble(-0.1, 0.1, pidAngleX.I, 1); // +1
                MSerial->writeDouble(-0.1, 0.1, pidAngleX.D, 1); // +1

                MSerial->writeDouble(-0.1, 0.1, pidAngleY.P, 1); // +1
                MSerial->writeDouble(-0.1, 0.1, pidAngleY.I, 1); // +1
                MSerial->writeDouble(-0.1, 0.1, pidAngleY.D, 1); // +1

#ifdef PID_USE_YAW
                MSerial->writeDouble(-0.1, 0.1, pidAngularVelocityZ.P, 1); // +1
                MSerial->writeDouble(-0.1, 0.1, pidAngularVelocityZ.I, 1); // +1
                MSerial->writeDouble(-0.1, 0.1, pidAngularVelocityZ.D, 1); // +1
#endif

#ifdef PID_USE_YAW_ANGLE
                MSerial->writeDouble(-0.1, 0.1, pidAngleZ.P, 1); // +1
                MSerial->writeDouble(-0.1, 0.1, pidAngleZ.I, 1); // +1
                MSerial->writeDouble(-0.1, 0.1, pidAngleZ.D, 1); // +1
#endif

#ifdef USE_COMPASS
                MSerial->writeDouble(0, 7, copterHeading, 2); // +2
#endif

                //motors
                for (unsigned i = 0; i < 4; i++)
                    MSerial->bufferAdd(100 * MController->getSpeed(getTorques(), i)); // +4

                MSerial->writeDouble(0, 20, voltage, 1); //+1
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
        MSerial->writeNumber(MyMPU->bytesAvailableFIFO());
        MSerial->bufferAdd("; V = ");
        MSerial->writeNumber(voltage);
#ifdef USE_COMPASS
        MSerial->bufferAdd("; H = ");
        MSerial->writeNumber(copterHeading * 180 / M_PI);

        MSerial->bufferAdd("; MX = ");
        MSerial->writeNumber(BMag.x * 100);
        MSerial->bufferAdd("; MY = ");
        MSerial->writeNumber(BMag.y * 100);
        MSerial->bufferAdd("; MZ = ");
        MSerial->writeNumber(BMag.z * 100);
#endif
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
