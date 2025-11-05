#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define CYCLE 62500


void    setup_timer_0(void) {
    TCCR0A |= 1 << WGM01 | 1 << WGM00 ;
    // TCCR0B |=  1 << WGM02;
    // predivider of 1024
    TCCR0B |= 1 << CS02 | 1 << CS00;
    // value of compare clean
    OCR0A = 125;
    // Setting interrupt on compare
    TIMSK0 |= 1 << TOIE0;
}

// to do = change wave mode to up & down then link the led on the timer value
void    setup_timer_1_led() {
    // led 1 as output
    DDRB |= (1 << DDB1);
    // enable clock on timer 1
    TCCR1B |= (1 << CS10);
    // enable wgm phase correct on 8 bit mode
    TCCR1A |= (1 << WGM10);
    // setting the top value to be a half a second
    ICR1 = CYCLE;
    // Setting OC1A to set the led on when it goes over ICR1, and off when its bottom
    TCCR1A |= (1 << COM1A1);
}

ISR(TIMER0_OVF_vect) {
    OCR1A = TCNT1;
}

// ISR(TIMER0_COMPA_vect) {
//     OCR1A = TCNT1;
// }

int main(void) {
    setup_timer_1_led();
    setup_timer_0();
    sei();
    while (42) { }
}