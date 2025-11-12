#include "lib_i2c.h"

int main(void) {
    i2c_init();
    uart_init();
    
    i2c_start();
    i2c_write_status();
    i2c_send_adr_as_sender(0x38);
    i2c_write_status();
    // send a status command
    i2c_write(AHT20_STATUS);
    i2c_write_status();
    i2c_stop();
    // No status is returned after Stop (:
        while (42) { }
    }