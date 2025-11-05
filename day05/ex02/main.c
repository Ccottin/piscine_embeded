#include <avr/io.h>
#include <util/delay.h>
#include "lib_uart.h"

// is selected by writting 0000 to the 4 LSB in reg ADMUX
#define RV1 0   // 0b0000
// Light Dependent Resistor -> a resistance that decreasewhen its brighter
// is selected by writting 0001 to the 4 LSB in reg ADMUX
#define LDR 1   // 0b0001
// is selected by writting 0010 to the 4 LSB in reg ADMUX
//Negative Temperature Coefficient Thermistor -> a resistance that decrese resistance when its hotter
#define NTC 2   // 0b0010

void    setup(void) {
    // Setting voltage reference to AVCC
    ADMUX |= 1 << REFS0;
    // ajust result to right, so we get an 10-bit result
    // ADMUX &= ~(1 << ADLAR);
    //ADMUX is also in charge of selecting input source, 
    // we want to read from ADC0 so we'll leave it this way

    // Enable ADC module
    ADCSRA |= 1 << ADEN; 
    // The ADC need a time clock to do the conversion. It is required that
    // it is contained between 50Khz and 200Khz. We'll use the CPU one, meaning 
    // we'll have to prescalle the input by 128 to make it fit (yields 125)
    ADCSRA |= 7 << ADPS0;
}

void        select_source(uint8_t source) {
    // clear the 4 bits of source selection 
    ADMUX &= 0b11110000;
    // set source 
    ADMUX |= source;
}

uint16_t     start_convertion(void) {
    // initiate a conversion
    ADCSRA |= 1 << ADSC;

    // the bit will remain one until the end of the conversion
    while (ADCSRA & (1 << ADSC)) { }
    // or use an interrupt :) ADCSRA = 1 << ADIE
    // Return value read on registers of the result
    return (ADC);
}

void    loop_values(void) {
  uint16_t c;

  for (uint8_t i = 0 ; i < 3 ; i++) {
    select_source(i);
    c = start_convertion();
    uart_printnbr_16bits(c);
    if (i == 2)
        break;
    uart_printstr(" , ");
  }
    uart_printstr("\r\n");
}

int main(void) {
    setup();
    uart_init();

    while (42) {
      loop_values();
      _delay_ms(20);
    }
}