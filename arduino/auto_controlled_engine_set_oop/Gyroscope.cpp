#include "Gyroscope.h"
#include "Arduino.h"

// I shall leave it for now
#include </usr/share/arduino/libraries/Wire/Wire.h>

Gyroscope::Gyroscope()
{
    Wire.begin();
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(0x3E);
    Wire.write(0x80);  // send a reset to the device
    Wire.endTransmission(); // end transmission
    
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(0x15);
    Wire.write((byte)0x00);   // sample rate divider
    Wire.endTransmission(); // end transmission
 
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(0x16);
    Wire.write(0x18); // ±2000 degrees/s (default value)
    Wire.endTransmission(); // end transmission
}

char Gyroscope::ITG3200Readbyte(unsigned char address)
{
    char data;
 
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();
    
    Wire.requestFrom(GYRO_ADDRESS, 1);
    if(Wire.available() > 0)
    {
        data = Wire.read();
    }
    
    return data;
    
    Wire.endTransmission();
}

int Gyroscope::ITG3200Read(unsigned char addressh, unsigned char addressl)
{
    long int data, t_data;

    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(addressh);
    Wire.endTransmission();
    
    Wire.requestFrom(GYRO_ADDRESS, 1);
    if(Wire.available() > 0)
    {
        t_data = Wire.read();
        data |= t_data << 8;
    }
    
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(addressl);
    Wire.endTransmission();
    
    if(Wire.available() > 0)
    {
        data |= Wire.read();
    }
    
    return data;
}

RVector3D Gyroscope::get_readings()
{
    RVector3D result = get_raw_readings();

    int i;

    for(i = 0; i < AXIS; i++)
    {
        result.value_by_axis_index(i) *= MPI / (180 * LSB_PER_DEG_PER_SEC);
        
        if(fabs(result.value_by_axis_index(i)) < ACCURACY)
            result.value_by_axis_index(i) = 0;
    }

    return(result);
}

RVector3D Gyroscope::get_raw_readings()
{
    RVector3D result;
    
    #ifdef DEBUG_NO_GYROSCOPE
    
        result = RVector3D();
    
    #else

        result.x = ITG3200Read(GX_H, GX_L);
        result.y = ITG3200Read(GY_H, GY_L);
        result.z = ITG3200Read(GZ_H, GZ_L);
        
    #endif
    
    return(result);
}
