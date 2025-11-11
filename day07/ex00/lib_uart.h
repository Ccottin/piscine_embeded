#ifndef LIB_UART_H
# define LIB_UART_H

# include <avr/io.h>

# define BAUD_PRESCALLER ((F_CPU / 16) + UART_BAUDRATE / 2) / UART_BAUDRATE - 1

void    uart_init(void);

void    uart_tx(char c);

char    ft_strcmp(char *s1, char *s2);

void    uart_printstr(const char *str);

void    uart_printnbr_hex_8bits(uint8_t nb);
void    uart_printnbr_hex_16bits(uint16_t nb);

void    uart_print_eeprom_address(uint16_t nb);

#endif