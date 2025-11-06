#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "isr.h"
#include "lib_uart.h"

static volatile uint8_t   val;

void    setup(void) {
    // Led as output
    DDRB |= (1 << DDB0);
    DDRB |= (1 << DDB1);
    DDRB |= (1 << DDB2);
    DDRB |= (1 << DDB4);

    // enable interrupt on pin change (2nd vector)
    PCICR |= 1 << PCIE2;
    // enable interrupt on d4 and d2 change
    PCMSK2 |= 1<< PCINT20 | 1 << PCINT18;
}

void    set_led(void) {
    // clear leds bits
    PORTB &= 0b11101000;
    if (val < 8)
        PORTB |= val;
    else
        PORTB |= (val + 8);
}

// tout ce qu'il ne faudra jamais faire dans un interrupt (:
FT_ISR(PCINT2_vect) {
    // clearing flag early in interrupt
    PCIFR &= ~(1 << PCIF2);
    // sw1 = increment
    if (!(PIND & (1 << PIND2)) && val < 15)
        ++val;
    // sw2 = decrement
    else if (!(PIND & (1 << PIND4)) && val != 0)
        --val;
    // while button is pressed, wait
    while (!(PIND & (1 << PIND4)) && !(PIND & (1 << PIND2))) { }
    set_led();
    _delay_ms(10);
}

int main(void) {
    setup();
    sei();
    val = 0;
    while (42) { }
}