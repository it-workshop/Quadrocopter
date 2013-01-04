#include "Wire.h"
#include "I2Cdev.h"
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

