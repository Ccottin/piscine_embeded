#ifndef LIB_I2C_H
# define LIB_I2C_H

#include <avr/io.h>
#include <util/twi.h>
// # include "lib_uart.h"

# define I2C_FREQ 100000
# define I2C_PRESQVAL 1

// https://assets.nexperia.com/documents/data-sheet/PCA9555.pdf
# define EXP_ADR 0b0100000
// found page 5
# define CONFIG_PORT0 0x06
# define OUPUT_PORT0  0x02
# define INPUT_PORT0  0x00

# define D9_ON   0b11110111
# define D10_ON  0b11111011
# define D11_ON  0b11111101
# define DX_OFF  0b11111111

# define LEDS_OUTPUT 0b11110001

// in volatile in case at some point we'll need this in an interrupt
extern volatile uint8_t status;

void    i2c_init(void);
void    i2c_start(void);
void    i2c_stop(void);
void    i2c_write(unsigned char data);

uint8_t    i2c_read(void);
uint8_t    i2c_read_nack(void);

void    print_hex_value(char c);

void    i2c_send_adr_as_sender(uint8_t adr);
void    i2c_send_adr_as_receiver(uint8_t adr);

// void    i2c_write_status(void);
void    i2c_multiwrite(uint8_t *data, uint8_t size);
void    i2c_multiread(uint8_t *data, uint8_t size);


#endif