#include "lib_adc.h"

void    setup_adc(uint8_t voltage_src) {
    // Setting voltage reference to given arg
    ADMUX |= voltage_src << REFS0;
    // ajust result to left, so we get a 8-bit result
    ADMUX |= (1 << ADLAR);
    // Enable ADC module
    ADCSRA |= 1 << ADEN; 
    // The ADC need a time clock to do the conversion. It is required that
    // it is contained between 50Khz and 200Khz. We'll use the CPU one, meaning 
    // we'll have to prescale the input by 128 to make it fit (yields 125)
    ADCSRA |= 7 << ADPS0;
}

void        select_source(uint8_t source) {
    // clear the 4 bits of source selection 
    ADMUX &= 0b11110000;
    // set source 
    ADMUX |= source;
}

// polling method to convert a value 
uint16_t     start_conversion(void) {
    // initiate a conversion
    ADCSRA |= 1 << ADSC;

    // the bit will remain one until the end of the conversion
    while (ADCSRA & (1 << ADSC)) { }
    // or use an interrupt :) ADCSRA = 1 << ADIE
    // Return value read on register of the result (on 10 bit)
    return (ADCH);
}