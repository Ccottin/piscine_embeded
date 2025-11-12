#ifndef LIB_UART_H
# define LIB_UART_H

# include <avr/io.h>

# define BAUD_PRESCALLER ((F_CPU / 16) + UART_BAUDRATE / 2) / UART_BAUDRATE - 1

# define RED "\033[31m"
# define REGULAR "\e[0m"

void    uart_init(void);

void    uart_tx(char c);
char    uart_rx(void);

uint8_t    ft_strcmp(uint8_t *s1, uint8_t *s2);

void    uart_getstr(uint8_t* str);
void    uart_printstr(const char *str);

void    uart_printnbr_hex_8bits(uint8_t nb);
void    uart_printnbr_hex_16bits(uint16_t nb);

void    uart_print_eeprom_address(uint16_t nb);

uint8_t    ascii_to_hex_8bit(uint8_t *str);
uint16_t    ascii_to_hex_16bit(uint8_t *str);

#endif