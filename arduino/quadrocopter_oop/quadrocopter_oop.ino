#include "Wire.h"
#include "I2Cdev.h"
#include "Definitions.h"
#ifdef _arch_arm_
    #include "pwm01.h"
#endif
#ifdef USE_COMPASS
    #include "HMC5883L.h"
#endif
#include "MPU6050_6Axis_MotionApps20.h"
#include "Quadrocopter.h"
#include "PWMInput.h"

Quadrocopter* quadro;
void setup()
{
    //quadro = new Quadrocopter;
    PWMInit();
    Serial.begin(115200);
}
void loop()
{
    //quadro->iteration();
    for(int i = 0; i < PINSN; i++)
    {
        Serial.print(PWMGetValue(i), 10);
        Serial.print("\t");
    }
    Serial.print("\n");
    delay(100);
}

