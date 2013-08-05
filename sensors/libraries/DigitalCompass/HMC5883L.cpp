/*****************************************************************************/	
//	Function:	 Cpp file for HMC5883L
//  Hardware:    Grove - 3-Axis Digital Compass
//	Arduino IDE: Arduino-1.0
//	Author:	 FrankieChu		
//	Date: 	 Jan 10,2013
//	Version: v1.0
//	by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/

#include <Arduino.h> 
#include "HMC5883L.h"

HMC5883L::HMC5883L()
{
    m_Scale = 1;
    requested = false;
}

void HMC5883L::requestRawData()
{
    if(!requested)
    {
        requested = true;
        request(DATA_REGISTER_BEGIN, RAW_READ_BYTES);
    }
}

bool HMC5883L::rawDataReady()
{
    return(requested && Wire1.available() >= RAW_READ_BYTES);
}

MagnetometerRaw HMC5883L::readRawAxis()
{
  uint8_t* buffer = read(6);
  MagnetometerRaw raw = MagnetometerRaw();
  raw.XAxis = (buffer[0] << 8) | buffer[1];
  raw.ZAxis = (buffer[2] << 8) | buffer[3];
  raw.YAxis = (buffer[4] << 8) | buffer[5];
  requested = false;
  return raw;
}

MagnetometerScaled HMC5883L::readScaledAxis()
{
  MagnetometerRaw raw = readRawAxis();
  MagnetometerScaled scaled = MagnetometerScaled();
  scaled.XAxis = raw.XAxis * m_Scale;
  scaled.ZAxis = raw.ZAxis * m_Scale;
  scaled.YAxis = raw.YAxis * m_Scale;
  return scaled;
}

int HMC5883L::setScale(float gauss)
{
	uint8_t regValue = 0x00;
	if(gauss == 0.88)
	{
		regValue = 0x00;
		m_Scale = 0.73;
	}
	else if(gauss == 1.3)
	{
		regValue = 0x01;
		m_Scale = 0.92;
	}
	else if(gauss == 1.9)
	{
		regValue = 0x02;
		m_Scale = 1.22;
	}
	else if(gauss == 2.5)
	{
		regValue = 0x03;
		m_Scale = 1.52;
	}
	else if(gauss == 4.0)
	{
		regValue = 0x04;
		m_Scale = 2.27;
	}
	else if(gauss == 4.7)
	{
		regValue = 0x05;
		m_Scale = 2.56;
	}
	else if(gauss == 5.6)
	{
		regValue = 0x06;
		m_Scale = 3.03;
	}
	else if(gauss == 8.1)
	{
		regValue = 0x07;
		m_Scale = 4.35;
	}
	else
		return ERRORCODE_1_NUM;
	
	// Setting is in the top 3 bits of the register.
	regValue = regValue << 5;
	write(CONFIGURATION_REGISTERB, regValue);
}

int HMC5883L::setMeasurementMode(uint8_t mode)
{
	write(MODE_REGISTER, mode);
}

void HMC5883L::write(int address, int data)
{
  Wire1.beginTransmission(HMC5883L_ADDRESS);
  Wire1.write(address);
  Wire1.write(data);
  Wire1.endTransmission(false);
}

uint8_t* HMC5883L::read(int length)
{
  uint8_t buffer[length];
  for(uint8_t i = 0; i < length; i++)
  {
      buffer[i] = Wire1.read();
  }
  Wire1.endTransmission(false);

  return buffer;
}

void HMC5883L::request(int address, int length)
{
    Wire1.beginTransmission(HMC5883L_ADDRESS);
    Wire1.write(address);
    Wire1.endTransmission(false);

    Wire1.beginTransmission(HMC5883L_ADDRESS);
    Wire1.requestFrom(HMC5883L_ADDRESS, length, false);
}

char* HMC5883L::getErrorText(int errorCode)
{
	if(ERRORCODE_1_NUM == 1)
		return ERRORCODE_1;
	
	return "Error not defined.";
}
