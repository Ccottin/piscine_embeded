#include "uart_header.h"
#include "led_header.h"

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

    set_duty_cycle_red(0, 0);
    set_duty_cycle_green(0, 0);
    set_duty_cycle_blue(0, 0);
    
    turn_red_pwm_on();
    turn_green_pwm_on();
    turn_blue_pwm_on();
}

// tres sale mais temps limitey
uint8_t hexa_to_integer(char first, char second) {
    if (first >= 'A' && first <= 'F') {
        second -= 65; // - 65 + 10
    } else if (first >= 'a' && first <= 'f') {
        first -= 97; // - 97 + 10
    } else if (first >= '0' && first <= '9') {
        first -= 48;
    }
     if (second >= 'A' && second <= 'Z') {
        second -= 55; // - 65 ('A') + 10
    } else if (second >= 'a' && second <= 'z') {
        second -= 97; // - 97 ('a') + 10
    } else if (second >= '0' && second <= '9') {
        second -= 48;
    }
    return(first * 16) + second;
}

char    parse_color(char *color_code) {
    int     i;

    if (color_code[0] != '#')
    i = 1;
    while (color_code[i] &&
        (color_code[i] >= 'a' && color_code[i] <= 'f') ||
        (color_code[i] >= 'A' && color_code[i] <= 'F') ||
        (color_code[i] >= '0' && color_code[i] <= '9'))
        i++;
    while (color_code[i])
        i++;
    if (i != 7)
        return (1);
    set_duty_cycle_red(hexa_to_integer(color_code[1], color_code[2]), 50);
    set_duty_cycle_green(hexa_to_integer(color_code[3], color_code[4]), 50);
    set_duty_cycle_blue(hexa_to_integer(color_code[5], color_code[6]), 50);
    
    return (0);
}

int main(void)
{
    char    color_code[10];

    setup();
    init_rgb();
    uart_init();
    while (42) {
        uart_getstr(color_code, 9);
        if (parse_color(color_code))
            uart_printstr("Invalid color code.\r\n");
        else
            uart_printstr(" -> Done.\r\n");
    }
}                                                                                                        