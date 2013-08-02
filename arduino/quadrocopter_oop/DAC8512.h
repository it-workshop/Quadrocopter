#ifndef DAC8512_H
#define DAC8512_H

#include "Definitions.h"

// only AVR is supported
#ifdef _arch_avr_
    #ifdef DEBUG_DAC

    class DAC8512
    {
    private:
        //pins

        volatile unsigned char* CMDDDR;
        volatile unsigned char* CMDPORT;


        static const unsigned int CS = 1;
        static const unsigned int CLK = (1 << 2);
        static const unsigned int SDI = (1 << 3);
        static const unsigned int LD = (1 << 4);
        static const unsigned int CLR = (1 << 5);
        #define dac_delay() asm("nop")

    public:
        DAC8512();
        void init();
        void send(unsigned int data);
    };

    #endif
#endif

#endif // DAC8512_H
