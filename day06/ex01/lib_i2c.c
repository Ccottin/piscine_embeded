#include "lib_i2c.h"

volatile uint8_t     status = 0;

/*
page 222 -> the formula
change variables in equation
we get TWBR = ((CPUfreq / SCLfreq) - 16) / (2 * PresqVal)
asserting the prescaler value is one (for a start)
and replacing all the other values by the ones we know,
we get a result that fits into a byte ( < 255 )
We'll then stick to the prescaller value of one, and uses all other variables we know
Spoiler : this should give 72
*/

// This function will set the MCU's i2c at 100kHz
void    i2c_init(void) {
    // temp value for NoVerflow
    unsigned long   temp = ((F_CPU / I2C_FREQ) - 16) / (2 * I2C_PRESQVAL);
    // letting TWPS1 and TWPS0 in TWSR to 0, means prescaler value = 1
    // TWI Bit Rate register -> will generate the SCK clocks frequency
    TWBR = temp;
    // Enable TWI
    TWCR |= 1 << TWEN;
}

void    i2c_start(void) {
    // In order to start a transmition, bit TWSTA should be set ; then TWINT must
    // be cleared (setting to 1)
    TWCR = 1 << TWINT | 1 << TWSTA | 1 << TWEN;
    // Once TWINT is 0 again, it means the start is sent and it is possible to 
    // continue the process
    while ((TWCR & (1 << TWINT)) == 0) { }
    // updating status by masking the 3 LSB that are used for something else 
    status = 0b11111000 & TWSR;
}

/* P.256 -> a repeated start condition can be stransmitted insteadof a stop,
so the master will keep control of the bus.
    This means you only need to send a stop at the very end of program, as the MCU
    is the only main emettor     */
void    i2c_stop(void) {
    // Clear Twint and send TWSTO for stop condition. Twi will not be raised afterward
    TWCR = 1 << TWINT | 1 << TWSTO | 1 << TWEN;
}

void    i2c_write(unsigned char data){
    // loading data into receive/send buffer
    TWDR = data;
    // clearing flag, meaning there is data ready to be sent
    TWCR =  1 << TWINT | 1 << TWEN;
    // Wait for flag to be set
    while ((TWCR & (1 << TWINT)) == 0) { }
    // updating status by masking the 3 LSB that are used for something else 
    status = 0b11111000 & TWSR;
}

/*************************************************************8 */

// In order to initiate a transmission, the transmiter should send the receiver &-bits address
// shifted one bit to the left. The remaining bit is then used to tell if the transmission is in
// emission ( = write, = 0 ) or reception ( = read, = 1)
void    i2c_send_adr_as_sender(uint8_t rec_adr) {
    i2c_write(rec_adr << 1);
}

void    i2c_send_adr_as_receiver(uint8_t rec_adr) {
    i2c_write(rec_adr << 1 | 0x01);
}

uint8_t    i2c_read_and_return(void) {
    // clearing interrupt, meaning receive/send buffer has been read
    // adding TWEA to send an acknoledge bit afterwards
    TWCR = 1 << TWINT | 1 << TWEA | 1 << TWEN;
    // Wait for flag to be set
    while ((TWCR & (1 << TWINT)) == 0) { }
    // updating status by masking the 3 LSB that are used for something else 
    status = 0b11111000 & TWSR;
    return (TWDR);
}
// same as up there, but no ack byte are send since TWEA is disabled
uint8_t    i2c_read_and_return_nack() {
    TWCR = 1 << TWINT | 1 << TWEN;
    while ((TWCR & (1 << TWINT)) == 0) { }
    status = 0b11111000 & TWSR;
    return (TWDR);
}

void    i2c_multiwrite(uint8_t *data, uint8_t size) {
    for (int i = 0; i < size; i++) {
        i2c_write(data[i]);
    }
}

void    i2c_multiread(uint8_t *data, uint8_t size) {
        for (int i = 0; i < size; i++) {
        data[i] = i2c_read_and_return();
    }
}