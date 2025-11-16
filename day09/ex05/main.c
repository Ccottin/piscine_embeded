#include "lib_i2c.h"
#include <util/delay.h>

void    timer(void) {
     // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 256 ticks
    TCCR1B |= (1 << CS12);
    // CTC mode ; top will be OCR1A
    TCCR1B |= (1 << WGM12);
    // Top value set to be 1 second
    OCR1A = 62500;
}
/* 
    https://assets.nexperia.com/documents/data-sheet/PCA9555.pdf
*/


void    write_to_expander(uint8_t reg, uint8_t data) {
    i2c_start();
    i2c_send_adr_as_sender(EXP_ADR);
    i2c_write(reg);
    // setting bit 3 to 0, turning led on (p.5) 
    i2c_write(data);

    i2c_stop();
}

// p.8 : sending two datas will set the register, then its pair -> two sets in one send
void    write_to_expander_twice(uint8_t reg, uint8_t data, uint8_t data2) {
    i2c_start();
    i2c_send_adr_as_sender(EXP_ADR);
    i2c_write(reg);
    i2c_write(data);
    i2c_write(data2);
    // setting bit 3 to 0, turning led on (p.5) 
    i2c_stop();
}

// pointless complexity
void    display_four_numbers(uint8_t *seg) {
    uint8_t tab[10] = { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE };
    uint8_t reg[4] = { SEG1, SEG2, SEG3, SEG4 };
    for (uint8_t i = 0 ; i < 4 ; i++) {
        write_to_expander_twice(OUPUT_PORT0, reg[i], tab[seg[i]]);
        write_to_expander_twice(OUPUT_PORT0, reg[i], RESET);
    }
}

// setup everybody as output
void    setup_segment_display(void) {
    write_to_expander(CONFIG_PORT0, SEG_OUTPUT0);
    write_to_expander(CONFIG_PORT1, SEG_OUTPUT1);
}

int     main(void) {
    uint16_t    val;
    uint16_t    temp_val;
    uint8_t     seg_val[4] = { 0, 0, 0, 0 };

    i2c_init();
    timer();
    setup_segment_display();
    // settings leds as output. Button sw3 is already configured as input
    val = 0;
    while (42) {
        if (TIFR1 & (1 << OCF1A)) {
            // clearing interrupt flag of timer 1
            TIFR1 |= (1 << OCF1A);
            ++val;
            if (val == 10000) {
                val = 0;
            }
            temp_val = val;
            for (uint8_t i = 0; i < 4 ; i++) {
                // start at the end
                seg_val[3 - i] = temp_val % 10;
                temp_val = temp_val / 10;
            }
        }
        display_four_numbers(seg_val);
    }
}