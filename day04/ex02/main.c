#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "isr.h"

 volatile uint8_t   interrupt_time;
 volatile uint8_t   val;

void    setup(void) {
    // Led as output
    DDRB |= (1 << DDB0);
    DDRB |= (1 << DDB4);
    // Setting flag for interrupt on int0, on port D, where sw1 is solded
    EIMSK |= (1 << INT0);
    // Interrupt will be triggered as long as button is low
    // With regs ISC01 and ISC00 in EICRA at 0

    // enable interrupt on pin change (2nd vector)
    PCICR |= 1 << PCIE2;
    // enable interrupt on d4 change
    PCMSK2 |= 1<< PCINT20;
}

void    set_led(uint8_t val) {
    if 
    PORTB |= val;
}

// SW2 : PD4 => PCINT20
FT_ISR(PCINT2_vect) {
    // clearing flag early in interrupt
    PCIFR &= ~(1 << PCIF2);

    while (PORTD & (1 << PORTD4)) {

    }
    if (val > 0) {
        --val;
    }
    set_led();
    _delay_ms(10);
}

// SW1
FT_ISR(INT0_vect) {
    // clearing flag early in interrupt
    EIFR &= ~(1 << INTF0);
    if (val < 15) {
        ++val;
    }
    set_led();
    // nothing in main loop, means the debounce cannot be elsewhere then..
    _delay_ms(10);
}

int main(void) {
    setup();
    sei();
    interrupt_time = 0;
    button_pressed = 0;
    
    static uint8_t current_time = 0;
    while (42) { }
}