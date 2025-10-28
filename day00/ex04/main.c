#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    char    value;

    value = 0;
    
    // Setting Port to 0 (tristate) so it is not a resistance pulled up
    PORTD &= ~(0 << PORTD2); // SW1 -> increments
    PORTD &= ~(0 << PORTD4); // SW2 -> decrements
    // Setting data direction of dr2 so its an input
    DDRD &= ~(0 << DDD2);    // SW1
    DDRD &= ~(0 << DDD4);    // SW2

    // Configuring leds as outputs
    DDRB |= (1 << DDB0);
    DDRB |= (1 << DDB1);
    DDRB |= (1 << DDB2);
    DDRB |= (1 << DDB4);

    while (1)
    {
        // On button sw1 press, increments value
        if (!(PIND & (1 << PIND2)) && value < 15) {
            value++;
            // Empty loop to increment the counter only once per press
            while (!(PIND & (1 << PIND2))) { }
        }
        // On button sw2 press, increments value
        else if (!(PIND & (1 << PIND4)) && value != 0) {
            value--;
            while (!(PIND & (1 << PIND4))) { }
        }
        
        if (value & 1)
            PORTB |= (1 << PORTB0);
        else
            PORTB &= ~(1 << PORTB0);
        if (value & 2)
            PORTB |= (1 <<PORTB1);
        else
            PORTB &= ~(1 <<PORTB1);
        if (value & 4)
            PORTB |= (1 <<PORTB2);
        else
            PORTB &= ~(1 <<PORTB2);
        if (value & 8)
            PORTB |= (1 <<PORTB4);
        else
            PORTB &= ~(1 <<PORTB4);
     
        
        _delay_ms(20);
    }

}