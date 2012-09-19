#include "Wire.h"
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

