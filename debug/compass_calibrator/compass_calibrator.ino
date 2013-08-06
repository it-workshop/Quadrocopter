#include <HMC5883L.h>
#include <Wire.h>

const int SERIAL_SPEED = 9600;

const char C_REQUEST = 'r';

HMC5883L compass;

#define infoLedPin 13

//#define DEBUG

inline void transmitInt(int t_int) {
	int t_low, t_high;
	t_low = t_int & 0xff;
    t_high = t_int >> 8;

	Serial.write(t_high);
    Serial.write(t_low);

}

void setup()
{       
    Serial.begin(SERIAL_SPEED);
	Wire.begin();
 
	compass.setScale(1.3); // Set the scale of the compass.
	compass.setMeasurementMode(MEASUREMENT_CONTINUOUS); // Set the measurement mode to Continuous
}

void loop()
{
	char c;

    
#ifndef DEBUG
    if(Serial.available() > 0)
    {
        c = Serial.read();
        
        
		if(c == C_REQUEST)
#endif
        {
            MagnetometerRaw raw = compass.readRawAxis();
			#ifdef DEBUG
				Serial.print(raw.XAxis);
				Serial.print("\t");
				Serial.print(raw.YAxis);
				Serial.print("\t");
				Serial.println(raw.ZAxis);

			#else
				transmitInt(raw.XAxis);
				transmitInt(raw.YAxis);
				transmitInt(raw.ZAxis);
			#endif

        }
        #ifdef DEBUG
		    delay(100);
		#endif
#ifndef DEBUG
    }
#endif
}
