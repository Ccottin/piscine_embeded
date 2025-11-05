#include "lib_adc.h"
#include "lib_led.h"

// big data slice for the NoVerflow
void    update_color(uint32_t rv1) {
    uint8_t val;

    // change rv1 val to be contained between 0 and 255
    val = rv1 * 255 / 1023;
    wheel(val);
}

void    clear_leds(void) {
    PORTB &= 0b11101000;
}

// big data slice for the NoVerflow
void    update_gauge(uint32_t rv1) {
    uint8_t percent;

    // change rv1 val to pourcentage
    percent = rv1 * 100 / 1023;
    clear_leds();
    if (percent >= 25) {
        PORTB |= 1 << PORTB0;
    }
    if (percent >= 50) {
        PORTB |= 1 << PORTB1;
    }
    if (percent >= 75) {
        PORTB |= 1 << PORTB2;
    }
    if (percent == 100) {
        PORTB |= 1 << PORTB4;
    }
}

void    setup(void) {
    //leds as output
    DDRB |= 7 << DDB0 | 1 << DDB4;
}

int main(void) {
    uint16_t    rv1;

    setup();
    init_rgb();
    setup_adc(AVCC);
    select_source(RV1);
    while (42) {
        rv1 = start_conversion();
        update_gauge(rv1);
        update_color(rv1);
    }
}