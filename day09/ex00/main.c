#include "lib_i2c.h"

// will blik led a 1kh frequency with a duty cycle at 10%
void    second_counter(void) {
    // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 256 ticks
    TCCR1B |= (1 << CS12);
    // setting WGM to enable Fast Pulse Widht Modulation mode
    TCCR1B |= (1 << WGM12) | (1 << WGM13);
    TCCR1A |= (1 << WGM11);
    // setting the top value to be a second
    ICR1 = 62500 - 1;
    // PWM value is time of activity * percentage / 100 - 1
    OCR1A = 6250 - 1;
}

/*  Led d9 is connected to i/o0_3 port on the I2C expander
    i2c expender got an address thats possible to configure using
    switches on sw5 (on board). For now, we'll set it up on 000
    documentation could be found here : https://assets.nexperia.com/documents/data-sheet/PCA9555.pdf
*/
/* To communicate with expander, the first byte after periph address should be a command byte (p.6 on doc)
    It is used to determine in which registers we'll write/read.
    for now we'll need IO0_0 pin in output mode, so it should be command 2 according to table page 6

    We also need port0_3 to be in output, this can be done with register 6 (p.8)

    */

void    turn_on_d9(void) {
    i2c_start();
    i2c_send_adr_as_sender(EXP_ADR);
    i2c_write(OUPUT_PORT0);
    // setting bit 3 to 0, turning led on (p.5) 
    i2c_write(0b11110111);

    i2c_stop();
}

void    turn_off_d9(void) {
    i2c_start();
    i2c_send_adr_as_sender(EXP_ADR);
    i2c_write(OUPUT_PORT0);
    // setting bit 3 to 1, turning led off (p.5) 
    i2c_write(0b11111111);

    i2c_stop();
}

/* by default, pins are set to 1 -> input, so we'll configure them to be as output by setting them to 0 (p.6)*/
void    d9_as_output(void) {
    i2c_start();
    i2c_send_adr_as_sender(EXP_ADR);
    i2c_write(CONFIG_PORT0);
    // setting bit 3 to 0, so its an output
    i2c_write(0b11110111);
    i2c_stop();
}

int     main(void) {
    i2c_init();
    uart_init();
    d9_as_output();
    second_counter();

    while (42) {
        // at overflow, turn on led
        while (!(TIFR1 & (1 << TOV1))) { }
        TIFR1 |= (1 << TOV1);
        turn_on_d9();

        // when compare flag is risen, turn off led
        while (!(TIFR1 & (1 << OCF1A))) { }
        // clearing interrupt flag of timer 1
        TIFR1 |= (1 << OCF1A);
        turn_off_d9();
    }
}