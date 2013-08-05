#include <Wire.h>
#include "altitudeSensor.h"

altitudeSensor* altSensor;

void setup() {
  Serial.begin(9600);
  altSensor = new altitudeSensor;
}

void loop() {
  //Serial.println(altSensor->altitude());
  //Serial.print( "1\n" );
  
  altSensor->printData();
  

  delay(100);
}
