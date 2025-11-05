#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define TOP 62500

void    setup_timer_0(void) {
    // predivider of 1024 -> should overflow 62 times per sec
    TCCR0B |= 1 << CS02 | 1 << CS00;
    // Clear Timer on compare
    TCCR0A |= 1 << WGM01;
    // should send 200 interrupts per seconds
    OCR0A = 78;
    // Setting interrupt on compare match A
    TIMSK0 |= 1 << OCIE0A;
}



void    setup_timer_1(void) {
    // enable clock timer 1
    TCCR1B |= 1 << CS12;
    // enable fast pwm, top at ICR1, compare on OCR1A
    TCCR1B |= 1 << WGM12 | 1 << WGM13;
    TCCR1A |= 1 << WGM10;
    // setting top value
    ICR1 = TOP;
    // clears OC1a at compare match, set at bottom
    TCCR1A |= 1 << COM1A1;
    OCR1A = 1;
}

void    setup(void) {
    // led output
    DDRB |= 1 << DDB1;
    setup_timer_1();
    setup_timer_0();
    sei();
}

ISR(TIMER0_COMPA_vect) {
    static uint8_t  count = 0;
    static uint8_t  dir = 1;

    OCR1A = (TOP * (dir / 10)) / 10;

    if (count == 100 || count == 0) {
        dir = !dir;
    }
    if (dir) {
        --count;
    } else {
        ++count;
    }
}

int     main(void) {
    setup();
    while (42) { }
}