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

Quadrocopter* quadro;
void setup()
{
    quadro = new Quadrocopter;
}

void loop()
{
    quadro->iteration();
}

