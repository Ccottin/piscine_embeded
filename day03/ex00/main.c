#include "header.h"

void    setup(void)
{
    // rgb led as output
    DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD3);
    // DDRB = 0x01;
}

void    set_timer(void)
{
    // Setting fast PWM timer 0
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // Setting fast PWM timer 0 & enable clock
    TCCR0B |= (1 << CS00);
    // TCCR0B |= (1 << WGM02) | (1 << CS00);
    
    // Setting fast PWM timer 2
    TCCR2A |= (1 << WGM21) | (1 << WGM20);
    // Setting fast PWM timer 0 & enable clock
    TCCR2B |= (1 << CS20);

    // cutting the duty cycle, so we won't become blind during this execrice (:
    set_duty_cycle_red(255, 30);
    set_duty_cycle_green(255, 30);
    set_duty_cycle_blue(255, 30);

    /* setting up timer 1 as the clock cycle we'll use to switch leds*/
    // predivider of 256 ticks
    TCCR1B |= (1 << CS12);
    // setting CTC to send a flag when OCR1A is reached
    TCCR1B |= (1 << WGM12);
    // setting OCR1A to be a second
    OCR1A = CYCLE;
    // Enable interrupts when the flag OCIF1A is set, meaning a Compare A match has been done
    TIMSK1 |= (1 << OCIE1A);
}

// Interrupt definition to switch leds every second
ISR(TIMER1_COMPA_vect)
{
    if (red_is_set()) {
        turn_red_pwm_off();
        turn_green_pwm_on();
    }
    else if (green_is_set()) {
        turn_green_pwm_off();
        turn_blue_pwm_on();
    } else if (blue_is_set()) {
        turn_blue_pwm_off();
        turn_red_pwm_on();
    }
}

int main(void)
{
    turn_red_pwm_on();
    setup();
    set_timer();
    sei();

    while (42) { }
}                                                                                                        