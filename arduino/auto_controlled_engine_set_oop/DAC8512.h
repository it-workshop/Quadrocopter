#ifndef DAC8512_H
#define DAC8512_H

#include "Definitions.h"

#ifdef DEBUG_DAC

class DAC8512
{
private:
    //pins

    volatile unsigned char* CMDDDR;
    volatile unsigned char* CMDPORT;


    unsigned int CS = 1;
    unsigned int CLK = (1 << 2);
    unsigned int SDI = (1 << 3);
    unsigned int LD = (1 << 4);
    unsigned int CLR = (1 << 5);
    #define dac_delay() asm("nop")

public:
    DAC8512();
    void init();
    void send(unsigned int data);
};

#endif

#endif // DAC8512_H
