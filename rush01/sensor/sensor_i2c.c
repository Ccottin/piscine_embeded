#include "lib_i2c.h"
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>

volatile uint8_t time_counter;

/*  
    all doc about thermosensor is in 
    https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf
*/

// calibrate first thing after sending the address & receive ack
void    calibrate_Aht20(void) {
    uint8_t byte;

    // Part 5.1 of doc : wait at top 20ms for sensor to be in idle state
    // also states to wait 40 ms after device power on before sending data so we'll wait 40
    _delay_ms(40);
    // maybe we can remove it

    i2c_start();
    // initiate transaction as sender
    i2c_send_adr_as_sender(AHT20_ADR);
    // ask for aht20 status
    i2c_write(AHT20_STATUS);

    //Send a double start condition to reset transaction on controller receive
    i2c_start();
    // enter connection in receive mode from aht20
    i2c_send_adr_as_receiver(AHT20_ADR);
    // set the status
    byte = i2c_read_and_return();
    /*
        5.4.1 -> chech bit [3] to make sure calibration is done
        if not, enter calibration command.
        The arguments for the command are then specified with 0x08 and 0x00, given by same datasheet
        Then a smol delay of 10 is required
    */
    if (byte & (1 << 3)) {
        //Send a double start condition to reset transaction on controller send
        i2c_start();
        i2c_send_adr_as_sender(AHT20_ADR);
        i2c_multiwrite(AHT20_CALIBR, 3);
        _delay_ms(10);
    }
}



void    display_value(uint8_t decimal, uint8_t virgule) {
    // i2c_print_digits(I2C_EXPANDER_ADDR, decimal / 10, decimal % 10, virgule / 10, virgule % 10);

}

void    convert_values(uint8_t *data) {
    float   temp;
    float   humidity;
    uint32_t    raw_temp;
    uint32_t    raw_humidity;

    // To convert values, we will follow the documentation about rteading and converting values
    // see 5.4 to read what offset, and 5.5 to convert offset afterwards
    // Everything should be stored into a uint32, as values are on 20 bits
    raw_humidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | ((uint32_t)data[3] >> 4);
    raw_temp = ((uint32_t)(data[3] & 0x0f) << 16) | ((uint32_t)data[4] << 8) |(uint32_t)data[5];

    // follow 5.5 formulas to translate from raw data to a float
    humidity = (((float)raw_humidity / 1048576.0) * 100.0);
    temp = (((float)raw_temp / 1048576.0) * 200.0 - 50.0);

    // Convert value from float to 2 uint, one for part before coma, one for part after
    // Get the full part of value, then substract it , then multiplies by 100 gets us 2 decimal precision
    uint8_t humidity_dec = (uint8_t)humidity;
    uint8_t humidity_vir = (uint8_t)((humidity - humidity_dec) * 100);
    uint8_t temp_dec = (uint8_t)temp;
    uint8_t temp_vir = (uint8_t)((humidity - temp_dec) * 100);

    display_value(temp_dec, temp_vir);
}

/* This function will follow steps described in 5.4.3 and 5.4.4.
    They are summarised into the small table at the end */
void    get_sensor_data(void) {
    uint8_t byte = 0;
    uint8_t data_byte[7] = {0, 0, 0, 0, 0, 0, 0};

    // Ask for data
    i2c_start();
    i2c_send_adr_as_sender(AHT20_ADR);
    i2c_multiwrite(AHT20_DATA, 3);
    _delay_ms(80);
    i2c_stop();

    i2c_start();
    i2c_send_adr_as_receiver(AHT20_ADR);
    // Wait for data ready
        // i think this condition is broken
    // uart_printstr("\r\n Data = ");
    while (!((byte & 1) == 0)) {
        byte = i2c_read_and_return();
        // uart_printnbr_hex_8bits(byte);
        // uart_printstr(" ");
        _delay_ms(80);
    }
        // uart_printstr("\r\n");
    // Receive data
    // get first 5 data bytes
    i2c_multiread(data_byte, 6);
    // get the last one, but this time send no ack in exchange
    data_byte[6] = i2c_read_and_return_nack();
    i2c_stop();
    // if we got time, do a check CRC to make sure the value is read
    // properly, is not just keep previous value
    convert_values(data_byte);
    
}


void    sensor_i2c_temp_celsius(void) {
    set_timer1_sensor();
    init_timer1();

    sei();

    i2c_init();
    calibrate_Aht20();
    
    get_sensor_data();
    set_timer1_wait();

    while (42) {
        // 2.4. demands to read data every 2 secs
        if (time_counter == 2)
        {
            set_timer1_sensor();
            get_sensor_data();
            set_timer1_wait();      
        }
    }
}

int main(void) {
    sensor_i2c_temp_celsius();
}