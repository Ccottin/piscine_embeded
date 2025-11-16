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

/* to read data from a register in the expander, follow steps described as 7.2 p.10 on PCA9555 doc */
uint8_t    read_from_expander(uint8_t reg) {
    uint8_t byte ;

    i2c_start();
    i2c_send_adr_as_sender(EXP_ADR);
    i2c_write(reg);
    // double start transmission : ready to read
    i2c_start();
    i2c_send_adr_as_receiver(EXP_ADR);
    byte = i2c_read();
    i2c_stop();
    return (byte);
}

int     main(void) {
    uint8_t value;

    value = 0;
    i2c_init();
    // settings leds as output. Button sw3 is already configured as input
    write_to_expander(CONFIG_PORT0, LEDS_OUTPUT);

    while (42) {
        // hopefully button pressed is active low?
        if (!(read_from_expander(0x00) & 1)) {
                if (value == 7)
                    value = 0;
                else
                    ++value;
                // Our led display will start at port0_1, and will be on 3 bits after. 
                // The value per default is 1, meaning it is active low, so we'll invert value of value (mdr)
                // then we'll send it (also don't forget to move it one bit to the left)
                write_to_expander(OUPUT_PORT0, ~(value << 1));
            // Empty loop to increment the counter only once per press
            while (!(read_from_expander(0x00) & 1)) { }
            //debounce
            _delay_ms(10); 

        }

    }
}