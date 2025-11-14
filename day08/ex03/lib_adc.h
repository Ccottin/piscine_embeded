#ifndef LIB_ADC_H
# define LIB_ADC_H

# include <avr/io.h>
# include <util/delay.h>

// is selected by writting 0000 to the 4 LSB in reg ADMUX
#define RV1 0   // 0b0000
// Light Dependent Resistor -> a resistance that decreasewhen its brighter
// is selected by writting 0001 to the 4 LSB in reg ADMUX
#define LDR 1   // 0b0001
// is selected by writting 0010 to the 4 LSB in reg ADMUX
//Negative Temperature Coefficient Thermistor -> a resistance that decrese resistance when its hotter
#define NTC 2   // 0b0010
// internal mcu's temperature reader
#define TEMP 8  // 0b1000

// source of power for ADC -> avcc, so 5v i think
#define AVCC 1  // 0b01
// source of power for ADC -> internal, 1.1v
#define INTERNAL 3 // 0b11 

void        setup_adc(uint8_t voltage_src);
void        select_source(uint8_t source);
uint16_t    start_conversion(void);

#endif