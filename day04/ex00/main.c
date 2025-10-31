#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void    setup(void) {
    // Led as output
    DDRB |= (1 << DDB0);
    // Setting flag for interrupt on pin change, on port D, where sw1 is solded
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT18);
}

ISR(PCINT0_vect) {
    PORTB |= (1 << PORTB0);
}

int main(void) {
    setup();
    // enable interrupt
    sei();
    while (42) { }
}