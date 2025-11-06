#include "lib_i2c.h"

void    i2c_init(void) {
    // page 222 -> the formula
    TWBR |= 
    // Enable TWI
    TWCR |= 1 << TWEN;
}

void    i2c_start(void) {
    // In order to start a transmition, bit TWSTA should be set ; then TWINT must
    // be cleared (setting to 1)
    TWCR |= 1 << TWINT | 1 << TWSTA;
    // Once TWINT is 0 again, it means the start is sent and it is possible to 
    // continue the process
    while ((TWCR & (1 << TWINT)) == 0);
}

void    i2c_stop(void) {
}

void    i2c_write(unsigned char data){

}

void    i2c_read(void) {
}

void    print_hex_value(char c) {

}