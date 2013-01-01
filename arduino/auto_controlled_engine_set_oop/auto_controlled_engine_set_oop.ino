#include "Wire.h"
#include "Quadrocopter.h"

Quadrocopter* quadro;

#define FASTADC 1
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void setup()
{
    quadro = new Quadrocopter;

#if FASTADC
    // set prescale to 16
    cbi(ADCSRA,ADPS2);
    cbi(ADCSRA,ADPS1);
    cbi(ADCSRA,ADPS0);
#endif
}

void loop()
{
    quadro->iteration();
}

