#include <avr/io.h>

int main(void)
{
    // Writing 0 to port b0 to set pin's drive to low (will activate when value is one)
    // No bitshift is required as the first led is on the first bit 
    PORTB = 0x00;
    // Writing 1 to data direction b0 to configure it as an output
    DDRB = 0x01;
    // Writing 1 to pin b0
    PINB = 0x01;
}