#include "DAC8512.h"
#include <avr/io.h>
#include <avr/delay.h>

#ifdef DEBUG_DAC

DAC8512::DAC8512()
{
    CMDDDR = &DDRB;
    CMDPORT = &PORTB;
}

void DAC8512::init()
{
    *CMDDDR |= CS | CLK | SDI | LD | CLR;
    *CMDPORT |= SDI | CLK | CS | CLR;
    *CMDPORT |= LD;
    *CMDPORT &= ~CLR;

    dac_delay();

    *CMDPORT |= CLR;
}

void DAC8512::send(unsigned int data)
{
    unsigned int x = 12; //bits

    *CMDPORT |= LD | CLR;
    dac_delay();
    *CMDPORT &= ~CS;

    do
    {
        x--;
        *CMDPORT &= ~CLK;

        if(data & (1 << x))
            *CMDPORT |= SDI;
        else
            *CMDPORT &= ~SDI;

        dac_delay();
        *CMDPORT |= CLK;
        dac_delay();
    }
    while(x);

    *CMDPORT &= ~SDI;
    *CMDPORT |= CS;
    dac_delay();
    *CMDPORT &= ~LD;
    dac_delay();
    *CMDPORT |= LD;
    dac_delay();
}

#endif
