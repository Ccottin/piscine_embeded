#ifndef LIB_UART_H
# define LIB_UART_H

# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>

# define BAUD_PRESCALLER ((F_CPU / 16) + UART_BAUDRATE / 2) / UART_BAUDRATE - 1

void    uart_init(void);

void    uart_tx(char c);

void    uart_printstr(const char *str);

void    uart_printnbr_8bits(uint8_t nb);
void    uart_printnbr_16bits(uint16_t nb);
void    uart_printnbr_32bits(uint32_t nb);

void    uart_printnbr_hex_8bits(uint8_t nb);
void    uart_printnbr_hex_16bits(uint16_t nb);

#endif