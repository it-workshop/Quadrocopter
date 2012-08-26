#include "RVector3D.h"
#include "Definitions.h"
#include "Arduino.h"

#ifndef GYROSCOPE_H
#define GYROSCOPE_H

class Gyroscope
{
private:
    // ITG3200 Register Defines
    static const unsigned char WHO   = 0x00,
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
                               PWR_M = 0x3E;
    static const int GYRO_ADDRESS = 0x68;

    static const double ACCURACY = 1E-1; // in radians / sec.
    static const int AXIS = 3;
    static const double LSB_PER_DEG_PER_SEC = 14.375;

public:
    Gyroscope();

    char ITG3200Readbyte(unsigned char address);

    int ITG3200Read(unsigned char addressh, unsigned char addressl);

    RVector3D get_raw_readings();
    RVector3D get_readings();
};

#endif
