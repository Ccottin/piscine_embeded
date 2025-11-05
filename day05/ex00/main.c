#include <avr/io.h>
#include <util/delay.h>
#include "lib_uart.h"

void    setup(void) {
    // Setting voltage reference to AVCC
    ADMUX |= 1 << REFS0;
    // ajust result to left, so we get an 8-bit result
    ADMUX |= 1 << ADLAR;
    //ADMUX is also in charge of selecting input source, 
    // we want to read from ADC0 so we'll leave it this way

    // Enable ADC module
    ADCSRA |= 1 << ADEN; 
    // The ADC need a time clock to do the conversion. It is required that
    // it is contained between 50Khz and 200Khz. We'll use the CPU one, meaning 
    // we'll have to prescalle the input by 128 to make it fit (yields 125)
    // ADCSRA |= 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0;
    ADCSRA |= 7 << ADPS0;
}

// Conversion will always occurs in the same register. Even if multiples sources of 
// analog could be selected, conversion can only occur one at time
uint8_t    start_convertion(void) {
    // initiate a conversion
    ADCSRA |= 1 << ADSC;

    // the bit will remain one until the end of the conversion
    while (ADCSRA & (1 << ADSC)) { }
    // or use an interrupt :) ADCSRA = 1 << ADIE
    // Return value read on lower-byte register of the result
    return (ADCH);
}



int main(void) {
    uint8_t c;

    setup();
    uart_init();

    while (42) {
        c = start_convertion();
        uart_printnbr_hex_8bits(c);
        uart_printstr("\r\n");
        _delay_ms(20);
    }
}