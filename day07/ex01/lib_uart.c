#include "lib_uart.h"

void    uart_init(void)
{
    // Configuring the baud rate of UART connection
    UBRR0H = (unsigned char)((BAUD_PRESCALLER) >> 8);
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

uint8_t get_number_size_hex(uint16_t nb) {
        uint8_t size = 0;

    while (nb > 15) {
        nb = nb /16;
        ++size;
    }
    // should start at index 0 anyway
    return (size);
}

void    uart_printnbr_hex_8bits(uint8_t nb)
{
    // everybody should be displayed 🥳
    char str[3] = {48, 48, 0};
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

uint8_t    ascii_to_hex_8bit(uint8_t *str) {
    uint8_t ret;
    uint8_t i;

    ret = 0;
    i = 0;
    while (str[i] && str[i + 1]) {
        if (str[i] >= '0' && str[i] <= '9')
            ret = (ret + (str[i] - 48)) * 16;  
        else
            ret = (ret + (str[i] - 87)) * 16;
        ++i;
    }
    if (str[i] && (str[i] >= '0' && str[i] <= '9'))
        ret = ret + (str[i] - 48);  
    else if (str[i])
        ret = ret + (str[i] - 87);

    return (ret);
}

// I might become super lazy
uint16_t    ascii_to_hex_16bit(uint8_t *str) {
    uint16_t ret;
    uint8_t i;

    ret = 0;
    i = 0;
    while (str[i] && str[i + 1]) {
        if (str[i] >= '0' && str[i] <= '9')
            ret = (ret + (str[i] - 48)) * 16;  
        else
            ret = (ret + (str[i] - 87)) * 16;
        ++i;
    }
    if (str[i] && (str[i] >= '0' && str[i] <= '9'))
        ret = ret + (str[i] - 48);  
    else if (str[i])
        ret = ret + (str[i] - 87);

    return (ret);

}

void    uart_printnbr_hex_16bits(uint16_t nb)
{
    // everybody should be displayed 🥳
    char str[5] = {48, 48, 48, 48, 0};
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


void    uart_print_eeprom_address(uint16_t nb)
{
    // fill the entire string (except the last 0) with char 0, so it will be displayed in hexdump
    char str[9] = {48, 48, 48, 48, 48, 48, 48, 48, 0};
    uint8_t i = 6 - get_number_size_hex(nb); // starts from the end of string
    uint8_t c;

    while (nb > 15) {
        c = nb % 16;
        if (c < 10)
            str[i++] = c + 48;
        else
            str[i++] = c + 87;
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

uint8_t    ft_strcmp(uint8_t *s1, uint8_t *s2)
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

void    uart_getstr(uint8_t* str)
{
    int     i;
    char    c;

    i = 0;
    while (42) {
        c = uart_rx();
        if (c == '\r') { // char send by screen when enter is used
            break;
        }
        else if (c == 127)  // delete char command
        {
            if (i > 0) {
                --i;
                uart_tx(8);     // backspace
                uart_tx(' ');   // print a space char
                uart_tx(8);     // backspace to move cursor back
            }
        }
        else if ( i <= 32 &&                        // Input size should be smaller then 32
                    // Checking no unexpected char are putted
                    ((c >= '0' && c <= '9') ||  \
                    (c >= 'a' && c <= 'f') || \
                    ( c == ' '))) {
            uart_tx(c);
            str[i] = c;
            ++i;
        }
    }
    str[i] = 0;
}