#include "Quadrocopter.h"
#include <FlexiTimer2.h>
#include <avr/interrupt.h>

extern Quadrocopter* quadro;

void QuadrocopterInterruptCall()
{
    quadro->processSensorsInterrupt();
}

void QuadrocopterInterruptInit()
{
    FlexiTimer2::set(10, 1./1000, QuadrocopterInterruptCall); // call every ms
    FlexiTimer2::start();
}
