#include <avr/io.h>

/* 16000000 hz bivided by a prescalor of 256 gives 62500, so a second will be 62500 increments of a timer
We want the led to be on for half a second, so 31250 is the value at wich led should be toggled*/
int main(void)
{
    // led 1 as output
    DDRB |= (1 << DDB1);
    // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 256 ticks
    TCCR1B |= (1 << CS12);
    // setting the comp value 
    OCR1A = 31250;
    // setting WGM to Clear Timer on Compare (CTC)
    TCCR1B |= (1 << WGM12);
    // Setting OC1A to Toggle everytime a compare match
    TCCR1A |= (1 << COM1A0);
    while (1) { }
}