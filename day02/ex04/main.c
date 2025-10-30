#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD_PRESCALLER ((F_CPU / 16) + UART_BAUDRATE / 2) / UART_BAUDRATE - 1

#define USERNAME "spectre\0"
#define PASSWORD "coucou\0"

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

void    config(void) {
    // Leds as output
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
    DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD3);
}

void    enter_string(char* str, char masked)
{
    int     i;
    char    c;

    i = 0;
      while (i < 10) {
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
            str[i] = c;
            if (masked) {
                uart_tx('*');
            } else {
                uart_tx(c);
            }
            ++i;
        }
    }
    str[i] = 0;
}

void    approuved(void)
{
    uart_printstr("\n\rHello there (:\n");
    for (int i = 5 ; i > 0; i--) {
        PORTB |= (1 << PORTB0);
        PORTD |= (1 << PORTD3);
        _delay_ms(100);
        PORTB |= (1 << PORTB1);
        PORTB &= ~(1 << PORTB0);
        PORTD |= (1 << PORTD5);
        _delay_ms(100);
        PORTB |= (1 << PORTB2);
        PORTB &= ~(1 << PORTB1);
        PORTD &= ~(1 << PORTD3);
        _delay_ms(100);
        PORTB |= (1 << PORTB4);
        PORTB &= ~((1 << PORTB2));
        PORTD &= ~(1 << PORTD5);
        PORTD |= (1 << PORTD6);
        _delay_ms(100);
        PORTD |= (1 << PORTD3);
        PORTB &= ~((1 << PORTB4));
        _delay_ms(100);
    }
    PORTB &= ~((1 << PORTD3) | (1 << PORTD5));
}

void    reject(void) {
    uart_printstr("\n\rBad combinaison username/password\n");
    for (int i = 4 ; i > 0; i--) {
        PORTB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB4);
        PORTD |= (1 << PORTD5);
        _delay_ms(500);
        PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB4));
        PORTD &= ~((1 << PORTD5));
        _delay_ms(500);
    }
}

int     main(void)
{
    char    username[11];
    char    password[11];

    uart_init();
    config();
    while (42) {
        uart_printstr("\rEnter your login:\n\r\tusername: ");
        enter_string(username, 0);
        uart_printstr("\n\r\tpassword: ");

        enter_string(password, 1);

        if (ft_strcmp(USERNAME, username) && ft_strcmp(PASSWORD, password)) {
            approuved();
        }
        else {
            reject();
        }
    }
}