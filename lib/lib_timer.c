#include "lib_timer.h"

// don't forget : this will overflow kickly. 
// solutions -> a bigger variable (still overflows)
// be aware & write a condition (for comapring with a previous value for example)
// write a generic function that detects overflow & handle behavior?
// might overhead, since this would take a comp in the isr
// stays at an atomic value -> prevent datarace


// a simple counter to increment a global value every second (counter 1)
void    second_counter(void) {
    time_counter = 0;
     // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 256 ticks
    TCCR1B |= 1 << CS12;
    // CTC mode ; top will be OCR1A
    TCCR1A |= 1 << WGM12;
    // Top value set to be a second
    OCR1A = 62500;
    // Enable interrupts on timer 1, then interrupts on CTC match for OCR1a 
    TIMSK1 |= 1 << ICIE1 | 1 << OCIE1A;
}

// shit. well for now we don't need both at same time; so lets say that mili and second cannot
// be used at once ; because of prescalling value :D
// En vrai de vrai je pourrais le mettre sur un autre timer
void    milisecond_counter(void) {
    time_counter = 0;
     // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 8 ticks
    TCCR1B |= 1 << CS11;
    // CTC mode ; top will be OCR1A
    TCCR1A |= 1 << WGM12;
    // Top value set to be a milisecond -> 16mhz / 64 / 1000
    OCR1A = 2000;
    // Enable interrupts on timer 1, then interrupts on CTC match for OCR1a 
    TIMSK1 |= 1 << ICIE1 | 1 << OCIE1A;
}

ISR(TIMER1_COMPA_vect) {
    ++time_counter;
    if (time_counter == 255)
        PORTB ^= 1 << PORTB0;

}