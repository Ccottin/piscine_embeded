#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD_PRESCALLER ((F_CPU / 16) + UART_BAUDRATE / 2) / UART_BAUDRATE - 1

void    uart_init(void)
{
    // Configuring the baud rate of UART connection
    UBRR0H = (unsigned char)(BAUD_PRESCALLER >> 8);
    UBRR0L = (unsigned char)(BAUD_PRESCALLER);
    // Enable Transmitter only.
    UCSR0B |= (1 << TXEN0);
    // Enable Receiver only. We don't need it for now
    // UCSR0B |= (1 << RXEN0);
    // Setting up frame format.
    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
}

void    uart_tx(char c)
{
    // Wait for empty trasmit buffer
    while(!(UCSR0A & (1 << UDRE0))) { };
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

void    set_timer(void) 
{
    // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 1024 ticks
    TCCR1B |= (1 << CS12) | (1 << CS10);
    //16000000 / 1024 = 15625 -> a second every this
    // setting the comp value to a double second
    OCR1A = 15625 * 2;
    // setting WGM to Clear Timer on Compare (CTC)
    TCCR1B |= (1 << WGM12);
    // Enable interrupt on output compare A. It will set OCF1A flag in register TIFR1 (p.145)
    TIMSK1 |= (1 <<  OCIE1A);
}

// Interrupt definition to send message every second. 
ISR(TIMER1_COMPA_vect) 
{
    uart_printstr("Hello World!\n\r");
}


int main(void)
{
    set_timer();
    uart_init();
    // Enable interrupts
    sei();
    while (42) { }
}