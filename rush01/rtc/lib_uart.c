#include "lib_uart.h"

void    uart_init(void)
{
    // Configuring the baud rate of UART connection
    UBRR0H = (unsigned char)(BAUD_PRESCALLER >> 8);
    UBRR0L = (unsigned char)(BAUD_PRESCALLER);
    // Enable Transmitter & Receiver.
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
    // Setting up frame format.
    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
}

void    uart_tx(char c)
{
    // Wait for empty trasmit buffer 
    while(!(UCSR0A & (1 << UDRE0))) { }
    // Data register accessor to send & receive data
    UDR0 = (uint8_t)c;
 
}

void    uart_printstr(const char* str)
{
    int i;

    i = 0;
    // just a loop to send char by char (:
    while(str[i] != 0)
    { 
        uart_tx(str[i]);
        ++i;
    }
}

uint8_t get_number_size_dec(uint16_t nb) {
    uint8_t size = 0;

    while (nb > 9) {
        nb = nb / 10;
        ++size;
    }
    // should start at index 0 anyway
    return (size);
}

uint8_t get_number_size_hex(uint16_t nb) {
        uint8_t size = 0;

    while (nb > 15) {
        nb = nb /16;
        ++size;
    }
    // should start at index 0 anyway
    return (size);
}

void    uart_printnbr_8bits(uint8_t nb)
{
    uint8_t str[4] = {0, 0, 0, 0};
    uint8_t i = get_number_size_dec(nb);

    while (nb > 9) {
        str[i--] = ((nb % 10) + 48);
        nb = nb / 10;
    }
    str[i] = nb + 48;
    uart_printstr(str);
}

void    uart_printnbr_16bits(uint16_t nb)
{
    uint8_t str[6] = {0, 0, 0, 0, 0, 0};
    uint8_t i = get_number_size_dec(nb);
    while (nb > 9) {
        str[i--] = ((nb % 10) + 48);
        nb = nb / 10;
    }
    str[i] = (uint8_t)(nb + 48);
    uart_printstr(str);
}

void    uart_printnbr_hex_8bits(uint8_t nb)
{
    uint8_t str[3] = {0, 0, 0};
    uint8_t i = get_number_size_hex(nb);
    uint8_t c;

    while (nb > 15) {
        c = nb % 16;
        if (c < 10)
            str[i--] = c + 48;
        else
            str[i--] = c + 87;
        nb = nb / 16;
    }
    if (nb < 10)
            str[i] = nb + 48;
        else
            str[i] = nb + 87;
    uart_printstr(str);
}

void    uart_printnbr_hex_16bits(uint16_t nb)
{
    uint8_t str[5] = {0, 0, 0, 0, 0};
    uint8_t i = get_number_size_hex(nb);
    uint8_t c;

    while (nb > 15) {
        c = nb % 16;
        if (c < 10)
            str[i--] = c + 48;
        else
            str[i--] = c + 87;
        nb = nb / 16;
    }
    if (nb < 10)
            str[i] = nb + 48;
        else
            str[i] = nb + 87;
    uart_printstr(str);
}

char    uart_rx(void)
{
    // Wait for RXC0 flag, meaning there is data received 
     while(!(UCSR0A & (1 << RXC0))) { }
     return (UDR0);
}

char    ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while (s1[i] && s2[i])
    {
        if (s1[i] == s2[i])
            ++i;
        else
            return (0);
    }
    if (s1[i] == 0 && s2[i] == 0)
        return (1);
    return (0);
}

void    uart_getstr(char* str, char size)
{
    int     i;
    char    c;

    i = 0;
      while (i < size) {
        c = uart_rx();
        if (c == '\r') // char send by screen when enter is used
            break;
        else if (c == 127)
        {
            if (i > 0) {
                --i;
                uart_tx(8);     // backspace
                uart_tx(' ');   // print a space char
                uart_tx(8);     // backspace to move cursor back
            }
        }
        else {
            uart_tx(c);
            str[i] = c;
            ++i;
        }
    }
    str[i] = 0;
}