#ifndef UART_HEADER
# define UART_HEADER

# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>

# define BAUD_PRESCALLER ((F_CPU / 16) + UART_BAUDRATE / 2) / UART_BAUDRATE - 1

void    uart_init(void);
void    uart_tx(char c);
void    uart_printstr(const char* str);
char    uart_rx(void);
char    ft_strcmp(char *s1, char *s2);
void    uart_getstr(char* str, char size);

#endif