#include "RVector3D.h"
#include "Definitions.h"
#include "Arduino.h"

#ifndef GYROSCOPE_H
#define GYROSCOPE_H

class Gyroscope
{
private:
    // ITG3200 Register Defines
    static const unsigned char
        WHO   = 0x00,
        SMPL  = 0x15,
        DLPF  = 0x16,
        INT_C = 0x17,
        INT_S = 0x1A,
        TMP_H = 0x1B,
        TMP_L = 0x1C,
        GX_H  = 0x1D,
        GX_L  = 0x1E,
        GY_H  = 0x1F,
        GY_L  = 0x20,
        GZ_H  = 0x21,
        GZ_L  = 0x22,
        PWR_MGM = 0x3E;
    static const int GYRO_ADDRESS = 0x68;

    static const double Accuracy = 1E-1; // in radians / sec.
    static const int Axis = 3;
    static const double LsbPerDegPerSec = 14.375;

public:
    Gyroscope();

    char readByte(unsigned char address);

    int read(unsigned char addressh, unsigned char addressl);

    RVector3D getRawReadings();
    RVector3D getReadings();
};

#endif
