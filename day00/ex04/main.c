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
        if (!(PIND & (1 << PIND2)) && value < 15)
            value++;
        // On button sw2 press, increments value
        else if (!(PIND & (1 << PIND4)) && value != 0)
            value--;
        
        if (value / 8 == 1) // if there is one time eight, we turn on the first light
            PORTB &= ~(1 <<PORTB4);
        else
            PORTB |= (1 <<PORTB4);
        if (value % 2 != 0) // Odd number -> turn on first led by changing if led is active high or low
            PORTB &= ~(1 << PORTB0);
        else
            PORTB |= (1 << PORTB0);

        if (value % 2 == 0) // Divisible by 0 -> we turn on the 2 led
            PORTB &= ~(1 <<PORTB1);
        else
            PORTB |= (1 <<PORTB1);

        if (value % 4 == 0) // Disisible by 4 -> we turn on the 4 led
            PORTB &= ~(1 <<PORTB2);
        else
            PORTB |= (1 <<PORTB2);

     
        
        _delay_ms(20);
    }

}