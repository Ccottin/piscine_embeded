#include "lib_i2c.h"
#include "lib_uart.h"
#include <util/delay.h>

volatile uint8_t     status;

int     main(void) {
    i2c_init();
    uart_init();
    i2c_start();
    _delay_ms(20);
    uart_printstr("\r\n");
    uart_printnbr_hex_8bits(status);
    i2c_transmitter_send(0x38);
    _delay_ms(20);

    uart_printstr("\r\n");
    uart_printnbr_hex_8bits(status);
    i2c_write('c');
    _delay_ms(20);
    uart_printstr("\r\n");
    uart_printnbr_hex_8bits(status);
    i2c_stop();
}