#include "Quadrocopter.h"
#include "Arduino.h"

extern Quadrocopter* quadro;

void serialEvent()
{
    quadro->processSerialInterrupt();
}
