#include <avr/io.h>
#include <avr/interrupt.h>

/* BAUD_PRESCALLER (F_CPU / (16FU * UART_BAUDRATE)) - 1 -> page 182 table 20-1
Problem is : substracting one shoud be accurate if it is possible to round the value.
So we'll use a little trick here to "round" the value before substracting one to the thingy.
(a + b / 2) / b will allow us to have a rounded result, not a trunkated one
*/
#define BAUD_PRESCALLER ((F_CPU / 16) + UART_BAUDRATE / 2) / UART_BAUDRATE - 1

/* BY default, UART is asynchronous, but it can be set to synchronous with writing 1 to
UMSEL01 and UMSEL00. This requires a third wire to send clock informations. We don't have 
such a wire, so we'll stay async. */

void    uart_init(void)
{
    // Configuring the baud rate of UART connection
    UBRR0H = (unsigned char)(BAUD_PRESCALLER >> 8);
    UBRR0L = (unsigned char)(BAUD_PRESCALLER);
    // Enable Transmitter only.
    UCSR0B |= (1 << TXEN0);
    // Enable Receiver only. We don't need it for now
    // UCSR0B |= (1 << RXEN0);
    // Setting up frame format. 8n1 means 8 databits, no parity check, 1 bit stop
    // UCSR0C -> registers for config. By letting registers USBS0 and UPM0 to 0, 
    // there is one bit stop and no parity check. 
    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
}

void    uart_tx(char c)
{
    // Wait for empty trasmit buffer
    while(!(UCSR0A & (1 << UDRE0))) { };
    // Data register accessor to send & receive data
    UDR0 = (uint8_t)c;
 
}

void    set_timer(void) 
{
    // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 256 ticks
    TCCR1B |= (1 << CS12);
    // setting the comp value 
    OCR1A = 62500;
    // setting WGM to Clear Timer on Compare (CTC)
    TCCR1B |= (1 << WGM12);
    // Enable interrupt on output compare A. It will set OCF1A flag in register TIFR1 (p.145)
    TIMSK1 |= (1 <<  OCIE1A);
}

// Interrupt definition to send Z every second. 
ISR(TIMER1_COMPA_vect) 
{
    uart_tx('Z');
}