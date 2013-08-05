/*
	URMSerial1.cpp - URM 37 Control Library Version 1.5.0
	Author: Miles Burton, miles@mnetcs.com
	Copyright (c) 2009 Miles Burton All Rights Reserved

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


	Ultrasound Sensor
	------------------
	* URM V3.2 ultrasonic sensor TTL connection with Arduino
	* Reads values (0-300) from an ultrasound sensor (3m sensor)
	* and writes the values to the serial->port.
	* Pin12 (Arduino). Pin 1 VCC (URM V3.2)
	* GND (Arduino) . Pin 2 GND (URM V3.2)
	* Pin11 (Arduino) . Pin 7 (URM V3.2)
	* Pin 0 (Arduino) . Pin 9 (URM V3.2)
	* Pin 1 (Arduino) . Pin 8 (URM V3.2)
	* www.yerobot.com
	* Last modified 20/04/2009
	*
	* URM37 Library by Miles Burton 2009.
	* Original code from Yerobot/DFRobot (thanks guys!)

*/

#include "URMSerial.h"
#include <Arduino.h>

// Serial "constants"
#define Header 0
#define HighByte 1
#define LowByte 2
#define Sum 3

#define SignedIntUpper 32767
#define SignedIntLower -32768

#define REQUEST_TIMEOUT 10000

URMSerial1::URMSerial1(void)
{
}

// See whether the serial port has sufficient data for a 'reading'
bool URMSerial1::hasReading(void)
{
	// We require at least 3 bytes before it can be considered a valid reading
	return (Serial1.available()>3);
}

// Read in serial data (max four bytes)
void URMSerial1::readSerial(void)
{
    for(int i=0;i<4;i++) serialData[i] = Serial1.read();
}

// Recieve distance from URM device
int URMSerial1::getMeasurement(int& value)
{
  if(!hasReading()) return NOTREADY;
  
  if(reqTimeout()) return TIMEOUT;
  

	// Read in data
	readSerial();

    // Validate the reading type
    switch(serialData[Header]) // Three types of command responses. 0x11 is a temperature, 0x22 Distance, 0x33 EEPROM
    {
     case 0x22:
        return processDistance(value);

     break;
     case 0x11:
        return processTemperature(value);
     break;
    }

    // Strange return result!
	distanceRequested = temperatureRequested = false; // This is a bug fix. If an error was returned (conrrupt byte array), reset distance measurements so we can re-request
    value = 0;
    return ERROR;
}

void URMSerial1::forceReset(void)
{
	distanceRequested = temperatureRequested = false; // This is a bug fix. If an error was returned (conrrupt byte array), reset distance measurements so we can re-request
}


// Process the 4 byte serial data and see whether the device reported an error (happens occationally)
bool URMSerial1::hasError()
{
    return((serialData[HighByte] & serialData[LowByte])==255); // If the High and Low byte are both 255: error
}

int URMSerial1::processDistance(int& value)
{
    distanceRequested = false;

	// If the serial data returned an error, quit
	if(hasError()) return ERROR;

	


    // 1x16bit value wrapped over two bytes
    // HighByte must be multiplied by 255 to represent the correct byte
    // Both are added together to calculate the value in Centimeters (I believe)
    value = serialData[HighByte]*255+serialData[LowByte];

	return DISTANCE;
}

int URMSerial1::processTemperature(int& value)
{
    temperatureRequested = false;

	// If the serial data returned an error, quit
	if(hasError()) return ERROR;

    // If 0, the value is the same regardless
    if(serialData[LowByte]==0)
    {
        value = 0;
    }else{

		// Below 0
		if(serialData[HighByte]==0x255)
		{
			value = -serialData[LowByte];
		}else{ // above zero
			value = serialData[LowByte]; // Consider below zero 1111
		}
	}

	return TEMPERATURE;
    //value = value >> 1; // Note: Bad practise, decimal value truncated
}

// Has the current request timed out?
bool URMSerial1::reqTimeout()
{
    if(temperatureRequested || distanceRequested)
    {
        int tDelta = millis() - timeRequested;
        // If we've waited to long
        if(tDelta>REQUEST_TIMEOUT || tDelta < 0) // If the delta is greater than our timeout or if it has wrapped around
        {
			
            return true;
        }else{
		
            return false;
        }
    }else{
	
        // No pending requests
        return true; // Return true anyway
    }
}

// send the temperature request to the serial port [*note, no delay]
bool URMSerial1::requestMeasurement(int mType)
{
    if(!reqTimeout()) return false;

    timeRequested = millis(); // store request time

    // Select appropriate memory addresses
    switch(mType)
    {
         case TEMPERATURE:
            sendCommand(temperatureMeasurementCommand);
            temperatureRequested = true;
            return true;
         break;
         case DISTANCE:
            sendCommand(distanceMeasurementCommand);
            distanceRequested = true;
            return true;
         break;
         default:
         // Do nothing
            return false;
         break;
    }
}

int URMSerial1::requestMeasurementOrTimeout(int mType, int& value)
{
    // Request measurement or fail
    if(!requestMeasurement(mType))
    {
        return NOTREADY;
    }

    // Wait until we either timeout or we have a reading
    while(!reqTimeout() && !hasReading())
    {
        delay(10); // Wait a bit
    }

    // Request has timed out, FAIL
    if(reqTimeout()) return TIMEOUT;

    // Pass control to getMeasurement which will return the requested measurement
    return getMeasurement(value);

}

void URMSerial1::sendCommand(uint8_t pCommand[4])
{
    // Send command
    for(int i=0;i<4;i++)  Serial1.write(pCommand[i]);
    //Serial.write(0x67);
}


// Setup our serial-> connection
void URMSerial1::begin(uint8_t rxPin, uint8_t txPin, long baud)
{
    // Initalise measurements
	distanceRequested = false;
	temperatureRequested = false;

    // See data sheet for details of command
	distanceMeasurementCommand[Header] = 0x22;
	distanceMeasurementCommand[HighByte] = 0x00;
	distanceMeasurementCommand[LowByte] = 0x00;
	distanceMeasurementCommand[Sum] = 0x22;

	// See data sheet for details of command
	temperatureMeasurementCommand[Header] = 0x11;
	temperatureMeasurementCommand[HighByte] = 0x00;
	temperatureMeasurementCommand[LowByte] = 0x00;
	temperatureMeasurementCommand[Sum] = 0x11;

    // Initalise serial data
    for(int i=0; i<4;i++) serialData[i]=0;

    // Initalise the software serial to handle our serial communication
    // Please note: using software serial over hardware-serial has some potential issues.
	//Serial.begin();
}