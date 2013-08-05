#include "altitudeSensor.h"

float altitudeSensor::altitude()
{
  temp = myBarometer->bmp085GetTemperature(myBarometer->bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first
  press = myBarometer->bmp085GetPressure(myBarometer->bmp085ReadUP());//Get the temperature
  alt = myBarometer->calcAltitude(press); //Uncompensated caculation - in Meters 
  //atm = press / 101325;


  return filter_alt =
    filter_coeff * alt + (1 - filter_coeff) * filter_alt;

    //return alt0;
}
