#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lib_timer.h"

void    set_duty_cycle_ledPB1(uint16_t total_time, uint8_t percentage) {
    // sets OCR1A to the given value
    OCR1A = (total_time * (percentage / 10)) / 10;
}

void    setup_timer_0(void) {
    // setting clear on compare mode
    TCCR0A |= 1 << WGM01;
    // predivider of 1024
    TCCR0B |= 1 << CS02 | 1 << CS00;
    // value of compare clean
    OCR0A = 122;

    // Setting interrupt on compare 
    TIMSK0 |= 1 << OCIE0A;
}

// to do = change wave mode to up & down then link the led on the timer value
void    setup_led() {
    // led 1 as output
    DDRB |= (1 << DDB1);
    // enable clock on timer 1
    TCCR1B |= (1 << CS10);
    // setting WGM to enable Fast Pulse Widht Modulation mode
    TCCR1B |= (1 << WGM12) | (1 << WGM13);
    TCCR1A |= (1 << WGM11);
    // setting the top value to be a second
    ICR1 = 62500;
    // Setting OC1A to set the led on when it goes over OCR1A, and off when its bottom
    TCCR1A |= (1 << COM1A1);
    // Starts with a led to low
    set_duty_cycle_ledPB1(62500, 50);
}

ISR(TIMER0_COMPA_vect) {
    PORTB ^= 1 << PORTB0;
    static uint8_t val = 1;
    set_duty_cycle_ledPB1(62500, val++);
    if (val == 100){
        val = 0;
    }
}

int main(void) {
    setup_led();
    sei();
    while (42) { }
}