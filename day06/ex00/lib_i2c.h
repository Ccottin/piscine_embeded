#ifndef LIB_I2C_H
# define LIB_I2C_H

#include <avr/io.h>
#include <util/twi.h>
# include "lib_uart.h"

# define I2C_FREQ 100000
# define I2C_PRESQVAL 1

// commands of termocensor definitions
# define AHT20_INIT 0xBE
# define AHT20_SOFTRST 0xBA
# define AHT20_STATUS 0x71
# define AHT20_ADR 0x38
/* Both of next commands are described in part 5.4 of doc */
# define AHT20_CALIBR (uint8_t[]) {0xBE, 0x08, 0x00}
# define AHT20_DATA (uint8_t[]) {0xAC, 0x33, 0x00}


// in volatile in case at some point we'll need this in an interrupt
extern volatile uint8_t status;

void    i2c_init(void);
void    i2c_start(void);
void    i2c_stop(void);
void    i2c_write(unsigned char data);
void    i2c_read(void);

void    print_hex_value(char c);

void    i2c_send_adr_as_sender(uint8_t adr);
void    i2c_send_adr_as_receiver(uint8_t adr);

uint8_t    i2c_read_and_return(void);
uint8_t    i2c_read_and_return_nack();

void    uart_write_status(void);
void    i2c_multiwrite(uint8_t *data, uint8_t size);
void    i2c_multiread(uint8_t *data, uint8_t size);


#endif