#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t is_pressed = 0;

void    setup(void) {
    // Led as output
    DDRB |= (1 << DDB0);
    // Setting flag for interrupt on int0, on port D, where sw1 is solded
    EIMSK |= (1 << INT0);
    // Interrupt will be triggered on falling edge of the button, meaning when button is pressed
    EICRA |= (1 << ISC01);
}

ISR(INT0_vect) {
    // blink
    is_pressed = 1;
    // delay for debounce, it is not safe to put delay in interrupts when there is 
    //more code, but since our only goal is to trigger the led this should be fine
}

int main(void) {
    setup();
    // enable interrupt

    sei();
    while (42) {
        if (is_pressed == 1) {
            _delay_ms(200);
            PORTB ^= (1 << PORTB0);
            is_pressed = 0;
        }
     }
}