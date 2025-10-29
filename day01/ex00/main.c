#include <avr/io.h>

int main(void) {
    unsigned long int count;

    // led 1 as output
    DDRB |= (1 << DDB1);
    count = 0;

    while (1) {
        // change led1 drive to turn it off
        PORTB &= ~(1 << PORTB1);
        while (count < 250000) { 
            count++;
        }
        // change led1 drive to turn it on
        PORTB |= (1 << PORTB1);
        while (count > 0) { 
            count--;
        }

    }
}