#include <avr/io.h>

int main(void)
{
    // led 1 as output
    DDRB |= (1 << DDB1);
    // 
    TCCR1B |= (3 << CS10);
    while (1) { }
}