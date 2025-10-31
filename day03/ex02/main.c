#include "header.h"

void    setup(void)
{
    // rgb led as output
    DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD3);
    // DDRB = 0x01;
}

void init_rgb(void)
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

    turn_blue_pwm_on();
    turn_green_pwm_on();
    turn_red_pwm_on();
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    set_duty_cycle_red(r, 30);
    set_duty_cycle_green(g, 30);
    set_duty_cycle_blue(b, 30);
    
    // They both work, but the upper solution doesnt make you blind (:

    // set_duty_cycle_red(255, r);
    // set_duty_cycle_green(255, g);
    // set_duty_cycle_blue(255, b);
}

void wheel(uint8_t pos)
{
    pos = 255 - pos;
    if (pos < 85) {
    set_rgb(255 - pos * 3, 0, pos * 3);
    } else if (pos < 170) {
    pos = pos - 85;
    set_rgb(0, pos * 3, 255 - pos * 3);
    } else {
    pos = pos - 170;
    set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

int main(void)
{
    int i;

    setup();
    init_rgb();
    i = 0;
    while (42) {
        wheel(i);
        _delay_ms(50);
        ++i;
    }
}                                                                                                        