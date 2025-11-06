#ifndef LIB_I2C_H
# define LIB_I2C_H

#include <avr/io.h>
#include <util/twi.h>
# include "lib_uart.h"

# define I2C_FREQ 100000UL
# define I2C_PRESQVAL 1

// in volatile in case at some point we'll need this in an interrupt
extern volatile     uint8_t status;

void    i2c_init(void);
void    i2c_start(void);
void    i2c_stop(void);
void    i2c_write(unsigned char data);
void    i2c_read(void);
void    print_hex_value(char c);
void    i2c_transmitter_send(uint8_t adr);
void    i2c_transmitter_receive(uint8_t adr);
#endif