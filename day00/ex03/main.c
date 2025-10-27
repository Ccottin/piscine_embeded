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
    PORTD &= ~(0x0 << PORTD2);
    // Setting data direction of dr2 so its an input
    DDRD &= ~(0x0 << DDD2);
    state = 0;
    while (1)
    {
        // checking if button is pressed
        if (!(PIND & (1 << PIND2)) != state) {
            state = !state;
            PINB = state;
        }
            _delay_ms(20);
    }
}