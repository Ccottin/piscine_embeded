#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    char state;

    // Writing 0 to port b0 to set pin's drive to low (will activate when value is one)
    // No bitshift is required as the first led is on the first bit
    PORTB = 0x00;
    // Writing 1 to data direction b0 to configure it as an output
    DDRB = 0x01;

    // Setting Port to 0 (tristate) so it is not a resistance pulled up
    PORTD &= ~(0 << PORTD2);
    // Setting data direction of dr2 so its an input
    DDRD &= ~(0 << DDD2);
    while (1)
    {
        // button is pressed
         if (!(PIND & (1 << PIND2)))
            // change the drive of the pin, so it will turn on led 
            PORTB = 0x01;
        else
            // change the drive of the pin, so it will turn off led 
            PORTB = 0x00;
    }
    _delay_ms(20);
}
