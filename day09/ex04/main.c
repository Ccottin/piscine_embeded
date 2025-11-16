#include "lib_i2c.h"
#include <util/delay.h>

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

void    setup_segment_display(void) {
    write_to_expander(CONFIG_PORT0, SEG_OUTPUT0);
    write_to_expander(CONFIG_PORT1, SEG_OUTPUT1);
}

int     main(void) {    
    i2c_init();
    setup_segment_display();
    // settings leds as output. Button sw3 is already configured as input
    while (42) { 
        write_to_expander_twice(OUPUT_PORT0, SEG4, TWO);
        write_to_expander_twice(OUPUT_PORT0, SEG4, RESET);
        write_to_expander_twice(OUPUT_PORT0, SEG3, FOUR);
        write_to_expander_twice(OUPUT_PORT0, SEG3, RESET);
    }
}