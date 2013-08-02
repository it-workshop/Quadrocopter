#include <avr/io.h>

int main()
{
    DDRB = 0b0000;
    DDRD = 0b1111;
    while(1)
    {
		PORTD = PINB;
	}
    
}
