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

#ifndef MPU6050DMP_H
#define MPU6050DMP_H

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include <Arduino.h>
#include "Wire.h"
#include "InfoLED.h"
#include "Definitions.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

extern bool mpuInterrupt; // indicates whether MPU interrupt pin has gone high

class MPU6050DMP
{
static const unsigned int DIM = 3;
//#define MPUDEBUG
private:    
    // class default I2C address is 0x68
    // specific I2C addresses may be passed as a parameter here
    // AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
    // AD0 high = 0x69
    MPU6050 mpu;
    
    /* =========================================================================
       NOTE: In addition to connection 3.3v, GND, SDA, and SCL, this sketch
       depends on the MPU-6050's INT pin being connected to the Arduino's
       external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
       digital I/O pin 2.
     * ========================================================================= */
    
    /* =========================================================================
       NOTE: Arduino v1.0.1 with the Leonardo board generates a compile error
       when using Serial.write(buf, len). The Teapot output uses this method.
       The solution requires a modification to the Arduino USBAPI.h file, which
       is fortunately simple, but annoying. This will be fixed in the next IDE
       release. For more info, see these links:
    
       http://arduino.cc/forum/index.php/topic,109987.0.html
       http://code.google.com/p/arduino/issues/detail?id=958
     * ========================================================================= */
     
    
    // MPU control/status vars
    bool dmpReady;  // set true if DMP init was successful
    uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
    uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO
    uint8_t fifoBuffer[64]; // FIFO storage buffer
    static const double maxWait = 10e-3;

    static const double gyroMulConstRad = M_PI / 180 * 16.4;
    
    // orientation/motion vars
    Quaternion q;           // [w, x, y, z]         quaternion container
    VectorInt16 aa;         // [x, y, z]            accel sensor measurements
    VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
    VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
    VectorFloat gravity;    // [x, y, z]            gravity vector
    int16_t av[DIM];        // [p, q, r]            gyro sensor measurements
    float ypr[DIM];         // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
    float tfloat[DIM];
    //float acc[DIM];         // [x, y, z]            accel sensor measurements (float)
    bool newData;

public:
    MPU6050DMP();

    void initialize();
    bool notBusy();
    void iteration();
    void processInterrupt();

    float* getAngleXYZ();
    float* getAngularVelocityXYZ();
    //float* getAccelXYZ();

    void attachFIFOInterrupt();
    int bytesAvailableFIFO();
    void resetNewData();
    bool getNewData();
#ifdef DEBUG_DAC
    InfoLED myLed;
#endif
};

void dmpDataReady();

#endif
