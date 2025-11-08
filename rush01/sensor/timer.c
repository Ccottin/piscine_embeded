#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile uint8_t time_counter;


void    init_timer1(void) {
    // Enable interrupts on timer 1, then interrupts on CTC match for OCR1a 
    TIMSK1 |= 1 << ICIE1 | 1 << OCIE1A; 
    // CTC mode ; top will be OCR1A
    TCCR1A |= 1 << WGM12;
}

// a simple counter to increment a global value every half second (counter 1)
void    set_timer1_sensor(void)
{
    time_counter = 0;
     // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 256 ticks
    TCCR1B |= 1 << CS12;
    // a tick every second
    OCR1A = 62499;
    //  then interrupts on CTC match for OCR1a 
}

void    set_timer1_wait(void)
{
    time_counter = 0;
     // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 8 ticks
    TCCR1B |= 1 << CS11;
    // Top value set to be a milisecond -> 16mhz / 64 / 1000
    OCR1A = 1999;


}

FT_ISR(TIMER1_COMPA_vect) {
    ++time_counter;
}