//#include <Wire.h>
#include "Barometer.h"

class altitudeSensor
{ 
public:
	void printData()
	{
		Serial.print("alt0: ");
		Serial.print(alt0);
		Serial.print("; alt: ");
		Serial.print(altitude());
		Serial.print("; rel_alt: ");
		Serial.println(relative_altitude());
	}
	void initialize()
	{
		myBarometer = new Barometer;
		myBarometer->init();

		for (int x = 0; x < N; x ++)
		{
		    temp = myBarometer->bmp085GetTemperature(myBarometer->bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first
		    press = myBarometer->bmp085GetPressure(myBarometer->bmp085ReadUP());//Get the temperature
		    alt0 += myBarometer->calcAltitude(press); //Uncompensated caculation - in Meters
		}

		alt0 /= N;
  		filter_alt = alt = alt0;
	}

	altitudeSensor() {
		temp = 0;
		press = 0;
		atm = 0;
		alt0 = 0;

		initialize();
	}
	  
	float altitude();
	float relative_altitude()
	{
		return altitude() - alt0;
	}
	float filter_alt;

private:
	float temp;
	float press;
	float atm;
	float alt;
	float alt0;

	Barometer *myBarometer;

	static const float filter_coeff = 0.45;
	static const int N = 100;
};