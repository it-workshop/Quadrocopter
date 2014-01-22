#include <Arduino.h>

//#define USE_COMPASS // else using heading potentiometer

//#define DEBUG

#include <I2Cdev.h>

#ifdef USE_COMPASS
#include <HMC5883L.h>
#include <Wire.h>

const int APINS_N = 3;
const int APINS[APINS_N] = {A0, A1, A3};
#else 
const int APINS_N = 4;
const int APINS[APINS_N] = {A0, A1, A3, A4};
#endif

const int DPINS_N = 1;
const int DPINS[DPINS_N] = {A2};

const int SERIAL_SPEED = 9600;

const char C_REQUEST = 'r';

#define infoLedPin 13

float heading = 0;

#ifdef USE_COMPASS
HMC5883L compass;
// compass's position on the joystick, while usual hold
// x looks left
// y looks forward
// z looks down
int error = 0;

int heading10000 = 0;

inline float sqr(float x) {
    return x * x;
}

inline double sign (double x) {
    return x >=0 ? 1 : -1;
}

#endif

inline void transmitInt(int t_int) {
    int t_low, t_high;
    t_low = t_int & 0xff;
    t_high = t_int >> 8;

    Serial.write(t_high);
    Serial.write(t_low);

}

void setup()
{
    //power for key
    //    pinMode(8, OUTPUT);
    //    digitalWrite(8, HIGH);
    //    pinMode(9, OUTPUT);
    //    digitalWrite(9, HIGH);
    
    //    pinMode(6, OUTPUT);
    //    digitalWrite(6, LOW);
    //    pinMode(7, OUTPUT);
    //    digitalWrite(7, LOW);
    
    
    for(int i = 0; i < APINS_N; i++)
        pinMode(APINS[i], INPUT);

    for(int i = 0; i < DPINS_N; i++)
        pinMode(DPINS[i], INPUT_PULLUP);

    Serial.begin(SERIAL_SPEED);
#ifdef USE_COMPASS
    Wire.begin();

    compass.setScale(1.3); // Set the scale of the compass.
    compass.setMeasurementMode(MEASUREMENT_CONTINUOUS); // Set the measurement mode to Continuous
#endif
}

void loop()
{
    static int i, t_int;
    char c;

    
#ifndef DEBUG
    if(Serial.available() > 0)
    {
        c = Serial.read();
        
        
        if(c == C_REQUEST)
#endif
        {
            for(i = 0; i < APINS_N - 1; i++)
            {
                t_int = analogRead(APINS[i]);
                
#ifdef DEBUG
                Serial.print(t_int);
                Serial.print("\t");
#else
                transmitInt(t_int);
#endif

            }

            //Serial.write(1);
            
            for(i = 0; i < DPINS_N; i++)
            {
                t_int = !digitalRead(DPINS[i]);
                
                //info led
                digitalWrite(infoLedPin, t_int ? HIGH: LOW);
                
#ifdef DEBUG
                Serial.print(t_int);
                Serial.print("\t");
#else
                Serial.write(t_int);
#endif
            }

#ifdef USE_COMPASS
            MagnetometerRaw raw = compass.readRawAxis();
            MagnetometerScaled cal; //calibrated
            cal.XAxis = 0.0017221638 * (raw.XAxis - 25.358562);
            cal.YAxis = 0.0016232947 * (raw.YAxis + 137.75926);
            //cal.ZAxis = 0.0019491249 * (raw.ZAxis - 376.08139);
            heading = atan2(cal.XAxis, cal.YAxis);
            //heading = atan2(cal.XAxis, sign(cal.YAxis)*sqrt(sqr(cal.ZAxis) + sqr(cal.YAxis)));


            if(heading < 0)
                heading += 2*PI;

            // Check for wrap due to addition of declination.
            if(heading > 2*PI)
                heading -= 2*PI;

            heading10000 = heading * 10000;
    #ifdef DEBUG
                /*Serial.print(raw.XAxis);
                        Serial.print("\t");
                        Serial.print(raw.YAxis);
                        Serial.print("\t");
                        Serial.print(raw.ZAxis);
                        Serial.print("\t");*/
                Serial.print(cal.XAxis);
                Serial.print("\t");
                Serial.print(cal.YAxis);
                Serial.print("\t");
                Serial.print(cal.ZAxis);
                Serial.print("\t");
                //Serial.print(sqrt(sqr(cal.XAxis) + sqr(cal.YAxis) + sqr(cal.ZAxis)));
                //Serial.print("\t");

                Serial.print(heading);

    #else
                transmitInt(heading10000);
    #endif
#else
            //310 910
            heading = (analogRead(A4)) / 1023. * 2 * PI;
#endif
            if(heading < 0)
                heading += 2*PI;
            if(heading > 2*PI)
                heading -= 2*PI;

#ifdef DEBUG
			Serial.print(analogRead(A4));
            Serial.print("\t");
            Serial.print(heading);
#else
            transmitInt(heading * 10000);
#endif


            
#ifdef DEBUG
            Serial.print("\n");
#endif
        }
#ifdef DEBUG
        delay(100);
#endif
#ifndef DEBUG
    }
#endif
}
