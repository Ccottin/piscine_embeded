#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD_PRESCALLER ((F_CPU / 16) + UART_BAUDRATE / 2) / UART_BAUDRATE - 1

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

char uart_rx(void)
{
    // Wait for RXC0 flag, meaning there is data received 
     while(!(UCSR0A & (1 << RXC0))) { }
     return (UDR0);
}

int main(void)
{
    uart_init();
    while (42) { 
            uart_tx(uart_rx());
    }
}