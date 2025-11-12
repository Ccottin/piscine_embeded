#ifndef PARSE_H
# define PARSE_H

# include "lib_uart.h"


void    cmd_selector(uint8_t* uart_input);
uint8_t eeprom_read(uint16_t address);
void    eeprom_write(uint16_t address, uint8_t val);

#endif