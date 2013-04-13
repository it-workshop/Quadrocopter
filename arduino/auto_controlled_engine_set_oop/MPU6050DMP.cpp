// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class using DMP (MotionApps v2.0)
// 6/21/2012 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2012-06-21 - added note about Arduino 1.0.1 + Leonardo compatibility error
//     2012-06-20 - improved FIFO overflow handling and simplified read process
//     2012-06-19 - completely rearranged DMP initialization code and simplification
//     2012-06-13 - pull gyro and accel data from FIFO packet instead of reading directly
//     2012-06-09 - fix broken FIFO read sequence and change interrupt detection to RISING
//     2012-06-05 - add gravity-compensated initial reference frame acceleration output
//                - add 3D math helper file to DMP6 example sketch
//                - add Euler output and Yaw/Pitch/Roll output formats
//     2012-06-04 - remove accel offset clearing for better results (thanks Sungon Lee)
//     2012-06-01 - fixed gyro sensitivity to be 2000 deg/sec instead of 250
//     2012-05-30 - basic DMP initialization working

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "MPU6050DMP.h"
#include "TimerCount.h"
#include "Quadrocopter.h"

bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

extern Quadrocopter *quadro;

void dmpDataReady()
{
    interrupts();
    mpuInterrupt = true;
    quadro->MPUInterrupt();
}

float* MPU6050DMP::getAngleXYZ()
{
    tfloat[0] = -ypr[2];
    tfloat[1] = +ypr[1];
    tfloat[2] = +ypr[0];
    return(tfloat);
}

float *MPU6050DMP::getAngularVelocityXYZ()
{
    tfloat[0] = -av[0] * gyroMulConstRad;
    tfloat[1] = -av[1] * gyroMulConstRad;
    tfloat[2] = -av[2] * gyroMulConstRad;
    return(tfloat);
}

void MPU6050DMP::attachFIFOInterrupt()
{
    // enable Arduino interrupt detection
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

}

int MPU6050DMP::bytesAvailableFIFO()
{
    return(mpu.getFIFOCount());
}

void MPU6050DMP::resetNewData()
{
    newData = false;
}

bool MPU6050DMP::getNewData()
{
    return(newData);
}

void MPU6050DMP::initialize() {
    myLed = InfoLED(A0, InfoLED::DAC);

    // reset YPR data
    ypr[0] = ypr[1] = ypr[2] = 0;
    av[0] = av[1] = av[2] = 0;

    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize device
    mpu.initialize();

    // load and configure the DMP
    devStatus = mpu.dmpInitialize();
    
    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        mpu.setDMPEnabled(true);

        attachFIFOInterrupt();

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        dmpReady = true;

        //Serial.print("MPU init ok\n");
    }
    //else Serial.print("MPU init failed\n");
    newData = false;
}

bool MPU6050DMP::notBusy()
{
    return(!mpuInterrupt && fifoCount < packetSize);
}

void MPU6050DMP::processInterrupt()
{
    newData = true;
}

void MPU6050DMP::iteration()
{
    if(!dmpReady) return;

    if(!mpu.dmpPacketAvailable()) return;
    myLed.setState(0);

    mpuIntStatus = mpu.getIntStatus();
    myLed.setState(10);

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        #ifdef MPUDEBUG
            Serial.println(F(" #OVERFLOW!# \n"));
        #endif
        myLed.setState(100);
        mpu.resetFIFO();

        // otherwise, check for DMP data ready interrupt (this should happen frequently)
    }
    else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        myLed.setState(20);
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
        myLed.setState(30);
        //if(fifoCount < packetSize) return;

        // removing leftovers
        if(fifoCount % 42 != 0)
        {
            #ifdef MPUDEBUG
                Serial.print(" #ERROR# ");
            #endif
            mpu.getFIFOBytes(fifoBuffer, fifoCount % 42);
        }
        myLed.setState(40);

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        myLed.setState(50);

        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGyro(av, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        #ifdef MPUDEBUG
            getAngleXYZ();
            for(int i = 0; i < 3; i++)
            {
                if(tfloat[i] > 0) Serial.print("+");
                Serial.print(tfloat[i]);
                Serial.print("\t");
            }
            getAngularVelocityXYZ();
            for(int i = 0; i < 3; i++)
            {
                if(tfloat[i] > 0) Serial.print("+");
                Serial.print(tfloat[i]);
                Serial.print("\t");
            }
            Serial.print("\n");
        #endif
        newData = true;
        myLed.setState(100);
    }
    myLed.setState(100);
}
