#include "lib_i2c.h"
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "lib_uart.h"


// commands of termocensor definitions
# define AHT20_INIT 0xBE
# define AHT20_SOFTRST 0xBA
# define AHT20_STATUS 0x71
# define AHT20_ADR 0x38
/* Both of next commands are described in part 5.4 of doc */
# define AHT20_CALIBR (uint8_t[]) {0xBE, 0x08, 0x00}
# define AHT20_DATA (uint8_t[]) {0xAC, 0x33, 0x00}
/*  
    all doc about thermosensor is in 
    https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf
*/


// stuff to put into the init of main -> 
//    calibrate_Aht20();
// and add this function to main
// calibrate first thing after sending the address & receive ack

/*********To add to i2c file ******* */

// void    i2c_multiwrite(uint8_t *data, uint8_t size) {
//     for (int i = 0; i < size; i++) {
//         i2c_write(data[i]);
//     }
// }
void    get_sensor_data(uint8_t *data_byte);


void    calibrate_Aht20(void) {
    uint8_t byte;
    // uint8_t clear_sensor_vals[7];

    // Part 5.1 of doc : wait at top 20ms for sensor to be in idle state
    // also states to wait 40 ms after device power on before sending data so we'll wait 40
    _delay_ms(40);
    // maybe we can remove it

    i2c_start();
    // initiate transaction as sender
    i2c_write(AHT20_ADR << 1);
    // ask for aht20 status
    i2c_write(AHT20_STATUS);

    //Send a double start condition to reset transaction on controller receive
    i2c_start();
    // enter connection in receive mode from aht20
    i2c_write(AHT20_ADR << 1 | 1);
    // set the status 
    // TODO == modifiy for i2c lib
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
        i2c_write(AHT20_ADR << 1);
        i2c_multiwrite(AHT20_CALIBR, 3);
        _delay_ms(10);
    }

    // Do 2 readings at startup in order to get corrects values
    // get_sensor_data(clear_sensor_vals);
    // get_sensor_data(clear_sensor_vals);
}

void    display_value(uint8_t decimal, uint8_t virgule) {
    uart_printnbr_8bits(decimal);
    uart_printstr((unsigned char *)",");
    uart_printnbr_8bits(virgule);
}

uint8_t*    convert_values(uint8_t *data, uint8_t mode, uint8_t *result) {
    float       computed_value;
    uint32_t    raw_temp;
    uint32_t    raw_humidity;

    // To convert values, we will follow the documentation about rteading and converting values
    // see 5.4 to read what offset, and 5.5 to convert offset afterwards
    // Everything should be stored into a uint32, as values are on 20 bits
    raw_humidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | ((uint32_t)data[3] >> 4);
    raw_temp = ((uint32_t)(data[3] & 0x0f) << 16) | ((uint32_t)data[4] << 8) |(uint32_t)data[5];

    // follow 5.5 formulas to translate from raw data to a float
    if (mode == 6)
        computed_value = (((float)raw_temp / 1048576.0) * 200.0 - 50.0);
    // This one is same as previous, w/ a little salt for Fareneith conversion
    else if (mode == 7)
        computed_value = ((((float)raw_temp / 1048576.0) * 200.0 - 50.0) * 1.8) + 32;
    else
        computed_value = (((float)raw_humidity / 1048576.0) * 100.0);

    // Convert value from float to 2 uint, one for part before coma, one for part after
    // Get the full part of value, then substract it , then multiplies by 100 gets us 2 decimal precision
    result[0] = (uint8_t)computed_value;
    result[1] = (uint8_t)((computed_value - result[0]) * 100);
    
    return (result);
}

/* This function will follow steps described in 5.4.3 and 5.4.4.
    They are summarised into the small table at the end */
void    get_sensor_data(uint8_t *data_byte) {
    uint8_t byte = 0;

    // Ask for data
    i2c_start();
    i2c_write(AHT20_ADR << 1);
    i2c_multiwrite(AHT20_DATA, 3);
    _delay_ms(80);
    i2c_stop();

    i2c_start();
    i2c_write(AHT20_ADR << 1 | 1);
    // Wait for data ready
    while (!((byte & 1) == 0)) {
        // TODO == change to be the as as i2c file
        byte = i2c_read_and_return();
        // check this if button is not responding
        _delay_ms(80);
    }
    // Receive data
    i2c_multiread(data_byte, 6);
    // get the last one, but this time send no ack in exchange
    // TODO == change to be the as as i2c file
    data_byte[6] = i2c_read_nack();
    i2c_stop();
    // if we got time, do a check CRC to make sure the value is read
    // properly, is not just keep previous value
}

// Mode 6 = displays temperature in Celsius
uint8_t mode6(uint8_t setup) {
    uint8_t data_byte[7];
    uint8_t celsius_result[2];
    
    get_sensor_data(data_byte);
    convert_values(data_byte, 6, celsius_result);
    display_value(celsius_result[0], celsius_result[1]);
    
    return (setup);
}

// Mode 7 = displays temperature in Farenheit
uint8_t mode7(uint8_t setup) {
    uint8_t data_byte[7];
    uint8_t fareneith_result[2];
    
    get_sensor_data(data_byte);
    convert_values(data_byte, 7, fareneith_result);
    display_value(fareneith_result[0], fareneith_result[1]);
    
    return (setup);
}

// Mode 8 = displays humidity in percentage
uint8_t mode8(uint8_t setup) {
    uint8_t data_byte[7];
    uint8_t humidity_result[2];
    
    get_sensor_data(data_byte);
    convert_values(data_byte, 8, humidity_result);
    display_value(humidity_result[0], humidity_result[1]);
    
    return (setup);
}


int main(void) {
    uint8_t secs = 0;
    // delay required by doc file prior starting i2c connection
    uart_init();
    i2c_init();
    _delay_ms(40);
    calibrate_Aht20();
    init_timer1(1);
    while (42) {
        if (TIFR1 & (1 << OCF1A)) {
            ++secs;
            TIFR1 |= (1 << OCF1A);
        }

        if (secs == 2) {
            mode7(0);
            uart_printstr((unsigned char *)"\r\n");
            secs = 0;
        }
    }
}