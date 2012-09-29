#include "Gyroscope.h"
#include "Arduino.h"

// I shall leave it for now
#include </usr/share/arduino/libraries/Wire/Wire.h>

Gyroscope::Gyroscope()
{
    Wire.begin();
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(PWR_MGM);
    Wire.write(0x80);  // send a reset to the device
    Wire.endTransmission(); // end transmission

    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(SMPL);
    Wire.write((byte)0x00);   // sample rate divider
    Wire.endTransmission(); // end transmission

    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(DLPF);
    Wire.write(0x18); // ±2000 degrees/s (default value)
    Wire.endTransmission(); // end transmission
}

char Gyroscope::readByte(unsigned char address)
{
    char data;

    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();

    Wire.requestFrom(GYRO_ADDRESS, 1);

    if(Wire.available() > 0)
        data = Wire.read();

    return data;
}

int Gyroscope::read(unsigned char addressh, unsigned char addressl)
{
    int data = 0;

    data = readByte(addressh);
    data *= (1 << 8);
    data += (int) readByte(addressl);

    return data;
}

RVector3D Gyroscope::getReadings()
{
    RVector3D result = getRawReadings();

    for(int i = 0; i < Axis; i++)
    {
        result.valueByAxisIndex(i) *= MPI / (180 * LsbPerDegPerSec);

        if(fabs(result.valueByAxisIndex(i)) < Accuracy)
            result.valueByAxisIndex(i) = 0;
    }

    return(result);
}

RVector3D Gyroscope::getRawReadings()
{
    RVector3D result;

    #ifdef DEBUG_NO_GYROSCOPE

        result = RVector3D();

    #else

        result.x = read(GX_H, GX_L);
        result.y = read(GY_H, GY_L);
        result.z = read(GZ_H, GZ_L);

    #endif

    return(result);
}
