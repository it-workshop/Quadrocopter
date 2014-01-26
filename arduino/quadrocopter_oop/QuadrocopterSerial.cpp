#include "Quadrocopter.h"
#include "PID.h"
#include "PWMInput.h"

void Quadrocopter::processSerialGetCommand()
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

void Quadrocopter::processSerialDoCommand()
{
    if(MSerial->getCommand() == 'p')
    {
        if(needPCTx)
        {
            needPCTx = false;
#ifdef DEBUG_NO_TX_ARDUINO
            Serial.write('x');
#else
            MSerial->bufferInit();
#endif

#ifdef DEBUG_DAC
            myLed.setState(15);
#endif

#ifndef DEBUG_NO_TX_ARDUINO
            processSerialPCTx();
            MSerial->bufferWrite();
#endif

            MSerial->dropCommand();
        }
        else if(MSerial->bytesAvailable() >= serialReadN)
        {
#ifdef DEBUG_SERIAL_SECOND
            DEBUG_SERIAL_SECOND.print("rx ");
            DEBUG_SERIAL_SECOND.print(serialReadN);
            DEBUG_SERIAL_SECOND.print("/");
            DEBUG_SERIAL_SECOND.println(MSerial->bytesAvailable());
#endif
            // reading

#ifdef DEBUG_DAC
            myLed.setState(10);
#endif

            processSerialPCRx();

            // writing

            needPCTx = true;
        }
    }
    else if(MSerial->getCommand() == 'a'/* || MSerial->isSendAutomaticlyEnabled()*/)
    {
        MSerial->bufferInit();
        processSerialTextTx();
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

void Quadrocopter::processSerialPCRx()
{
    forceOverrideValue = MSerial->read() / 100.; // +1
    forceOverride = MSerial->read(); // +1

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
    MSerial->readDouble(0, 10, tDouble, 2); pidAngleZ.setPMinMax(tDouble);
    MSerial->readDouble(0, 10, tDouble, 2); pidAngleZ.setIMinMax(tDouble);
    MSerial->readDouble(0, 10, tDouble, 2); pidAngleZ.setDMinMax(tDouble);
#endif
}

void Quadrocopter::processSerialPCTx()
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
    MSerial->writeDouble(-1, 1, pidAngleZ.P, 1); // +1
    MSerial->writeDouble(-1, 1, pidAngleZ.I, 1); // +1
    MSerial->writeDouble(-1, 1, pidAngleZ.D, 1); // +1
#endif

#ifdef USE_COMPASS
    MSerial->writeDouble(0, 7, copterHeading, 2); // +2
    MSerial->writeDouble(0, 7, joystickHeading, 2); // +2
#endif

    MSerial->writeDouble(0, 100, MController->getForce() * 100, 1); // +1

    MSerial->writeDouble(-2, 2, angleManualCorrection.x, 2); // +2
    MSerial->writeDouble(-2, 2, angleManualCorrection.y, 2); // +2

    //motors
    for (unsigned i = 0; i < 4; i++)
        MSerial->bufferAdd(100 * MController->getSpeed(getTorques(), i)); // +4

    MSerial->writeDouble(0, 20, voltage, 1); //+1
}

void Quadrocopter::processSerialTextTx()
{
    MSerial->bufferAdd("A=");
    MSerial->RVector3DWrite(angle * 180 / M_PI, MySerial::PRINT_TAB);
    MSerial->bufferAdd("; S=");
    MSerial->writeNumber(sensorsTime * 1000);
    MSerial->bufferAdd("; B = ");
    MSerial->writeNumber(MyMPU->bytesAvailableFIFO());
    MSerial->bufferAdd("; V = ");
    MSerial->writeNumber(voltage);
    MSerial->bufferAdd("; _V_ = ");
    MSerial->writeNumber(VSensor->getRawValue());
#ifdef USE_COMPASS
    MSerial->bufferAdd("; H = ");
    MSerial->writeNumber(copterHeading * 180 / M_PI);

    MSerial->bufferAdd("; J = ");
    MSerial->writeNumber(joystickHeading * 180 / M_PI);

    MSerial->bufferAdd("; P = ");
    MSerial->writeNumber(MController->getForce());

    MSerial->bufferAdd("; _P_ = ");
    MSerial->writeNumber(1.0 * RA[2] / (RA[2] + RB[2]));

    MSerial->bufferAdd("; MX = ");
    MSerial->writeNumber(BMag.x * 100);
    MSerial->bufferAdd("; MY = ");
    MSerial->writeNumber(BMag.y * 100);
    MSerial->bufferAdd("; MZ = ");
    MSerial->writeNumber(BMag.z * 100);
    MSerial->bufferAdd("; XYval=");
    MSerial->writeNumber(sqrt(BMag.x * BMag.x + BMag.y * BMag.y) * 100);
    MSerial->bufferAdd("; val=");
    MSerial->writeNumber(BMag.module() * 100);
#endif
    MSerial->bufferAdd('\n');
}

void Quadrocopter::processJoystickRx()
{
    joystickHeading += Joystick->getAV() * dt;
    while(joystickHeading > 2 * M_PI)
        joystickHeading -= 2 * M_PI;
    while(joystickHeading < 0)
        joystickHeading += 2 * M_PI;
    angleManualCorrection.x = Joystick->getAngleX();
    angleManualCorrection.y = Joystick->getAngleY();

    if(forceOverride)
        MController->setForce(forceOverrideValue);
    else
        MController->setForce(Joystick->getPower());
}
