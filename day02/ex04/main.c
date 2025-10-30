#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD_PRESCALLER ((F_CPU / 16) + UART_BAUDRATE / 2) / UART_BAUDRATE - 1

#define USERNAME "spectre"
#define PASSWORD "coucou"

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

void uart_printstr(const char* str)
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

// Interrupt definition to capitalize, then send back chars when data is detected. 
ISR(USART_RX_vect) 
{
    char    c;

    c = UDR0;
    if (c >= 'a' && c <= 'z') 
    {
        c -= 32;
    }
    else if (c >= 'A' && c <= 'Z') 
    {
        c += 32;
    }
    uart_tx(c);
}

void    config(void) 
{
    // Enable flag on rx data from uart
    UCSR0B |= (1 << RXCIE0);
}

int main(void)
{
    config();
    uart_init();
    // Enable interrupts
    sei();
    while (42) {
        uart_printstr("Enter your login:\nusername: ");
     }
}