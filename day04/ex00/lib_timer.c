#include "lib_timer.h"

volatile uint8_t time_counter = 0 ;

// don't forget : this will overflow kickly. 
// solutions -> a bigger variable (still overflows)
// be aware & write a condition (for comapring with a previous value for example)
// write a generic function that detects overflow & handle behavior?
// might overhead, since this would take a comp in the isr
// stays at an atomic value -> prevent datarace

// enum e_clock_select {
//     NO_CLOCK_SOURCE       = 0b000,
//     CLOCK_IO_DIVIDE_BY_1  = 0b001,
//     CLOCK_IO_DIVIDE_BY_8  = 0b010,
//     CLOCK_IO_DIVIDE_BY_64 = 0b011,
//     CLOCK_IO_DIVIDE_BY_8 = 0b010,
//     CLOCK_IO_DIVIDE_BY_8 = 0b010,

// };

// a simple counter to increment a global value every second (counter 1)
void    second_counter(void) {
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
     // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 8 ticks
    TCCR1B |= 1 << CS11;
    // CTC mode ; top will be OCR1A
    TCCR1B |= 1 << WGM12;
    // Top value set to be a milisecond -> 16mhz / 8 / 1000
    OCR1A = 1999;
    // Enable interrupts on timer 1, then interrupts on CTC match for OCR1a 
    TIMSK1 |= 1 << ICIE1 | 1 << OCIE1A;
}

FT_ISR(TIMER1_COMPA_vect) {
    ++time_counter;
}