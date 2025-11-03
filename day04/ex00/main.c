#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lib_timer.h"

volatile uint8_t   interrupt_time = 0;

void    setup(void) {
    // Led as output
    DDRB |= (1 << DDB0);
    // Setting flag for interrupt on int0, on port D, where sw1 is solded
    EIMSK |= (1 << INT0);
    // Interrupt will be triggered on falling edge of the button, meaning when button is pressed
    EICRA |= (1 << ISC01);
}

ISR(INT0_vect) {
    cli();
    // disable interrupt on button to prevent debounce
    EICRA &= ~(1 << ISC01);
    interrupt_time = time_counter;
    // enable interrupts again to time counter increase
    sei();
    PORTB ^= 1 << PORTB4;
}

int main(void) {
    
    setup();
    milisecond_counter();
    // enable interrupt
    sei();
    uint8_t current_time = 0;
    
    while (42) {
        // wait for interrupt time update
        while (current_time == interrupt_time) { }
        PORTB ^= 1 << PORTB0;
        // on overflow case, the operation stays valid as its cyclic
        while (current_time - interrupt_time > 20) {
            current_time = time_counter;
            _delay_ms(2);
        }

        cli();
        // set flag to read next button input
        EICRA |= (1 << ISC01);
        sei();
    }
}