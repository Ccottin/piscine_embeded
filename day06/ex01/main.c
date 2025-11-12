#include "lib_i2c.h"
#include "lib_uart.h"
#include <util/delay.h>
#include <stdlib.h>

volatile uint8_t     status;

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
    i2c_stop();
}

/* This function will follow steps described in 5.4.3 and 5.4.4.
    They are summarised into the small table at the end */
void    get_sensor_data(void) {
    uint8_t byte = 0;
    unsigned char data_byte[7] = {0, 0, 0, 0, 0, 0, 0};

    // Ask for data
    i2c_start();
    i2c_send_adr_as_sender(AHT20_ADR);
    i2c_multiwrite(AHT20_DATA, 3);
    // first delay of 80 ms
    _delay_ms(80);
    i2c_stop();

    i2c_start();
    i2c_send_adr_as_receiver(AHT20_ADR);
    // Wait for data ready
    while (!((byte & 1) == 0)) {
        byte = i2c_read_and_return();
        _delay_ms(10);
    }
    // Receive data
    // get first 5 data bytes
    i2c_multiread(data_byte, 6);
    // get the last one, but this time send no ack in exchange
    data_byte[6] = i2c_read_and_return_nack();
    // if we got time, do a check CRC to make sure the value is read
    // properly, is not just keep previous value
    // debug purpose =
    for (int i = 0 ; i < 7; i++) {
        print_hex_value((char)data_byte[i]);
        uart_tx(' ');
    }
    uart_printstr("\r\n");
}

int     main(void) {
    uart_init();

    i2c_init();
    calibrate_Aht20();
    while (42) {
        get_sensor_data();
        // 2.4. demands to read data every 2 secs
        _delay_ms(2000);
    }
    i2c_stop();
}