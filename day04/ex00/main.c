#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lib_timer.h"
#include "isr.h"

 volatile uint8_t   interrupt_time;
 volatile uint8_t   button_pressed;

void    setup(void) {
    // Led as output
    DDRB |= (1 << DDB0);
    DDRB |= (1 << DDB4);
    // Setting flag for interrupt on int0, on port D, where sw1 is solded
    EIMSK |= (1 << INT0);
    // Interrupt will be triggered as long as button is low
    // With regs ISC01 and ISC00 in EICRA at 0
}

FT_ISR(INT0_vect) {
    // clearing flag early in interrupt
    EIFR &= ~(1 << INTF0);
    if (button_pressed == 0) {
            PORTB ^= 1 << PORTB0;
    }
    interrupt_time = time_counter;
    button_pressed = 1;
}

int main(void) {
    setup();
    sei();
    milisecond_counter();
    interrupt_time = 0;
    button_pressed = 0;
    
    static uint8_t current_time = 0;
    while (42) {
        if (button_pressed) {
            while (time_counter - interrupt_time < 20) { }
            button_pressed = 0;
        }
    }
}