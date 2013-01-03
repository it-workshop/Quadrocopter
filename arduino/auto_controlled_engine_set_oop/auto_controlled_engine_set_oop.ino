#include "Wire.h"
#include "Quadrocopter.h"
#include <FlexiTimer2.h>

Quadrocopter* quadro;

void setup()
{
    quadro = new Quadrocopter;
    QuadrocopterInterruptInit();
    interrupts();
    sei();
}

void loop()
{
    quadro->iteration();
}

