#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)
void    uart_init(void)
{

}

void    uart_tx(char c)
{

}

int main(void)
{

}