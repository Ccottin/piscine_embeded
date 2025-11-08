#include "lib_uart.h"
#include "lib_i2c.h"

#define RTC_ADR 0xA2

#define SEC_REGISTER 0x02
#define MIN_REGISTER 0x03
#define HOUR_REGISTER 0x04
#define DAY_REGISTER 0x05
#define MONTH_REGISTER 0x07
#define YEAR_REGISTER 0x08


uint8_t get_time_value(uint8_t register_adress) {
    uint8_t value;

    i2c_start();
    // write bit is already shifted in address of doc ::::)
    // i2c_send_adr_as_sender(RTC_ADR);
    i2c_write(RTC_ADR);
    // send register address (only 4 bytes of right will be read)
    i2c_write(register_adress);
    i2c_stop();

    i2c_start();
    // enter read mode
    i2c_write(RTC_ADR + 1);
    value = i2c_read_and_return_nack();
    i2c_stop();
     /* 
        Datasheet of PCF8563 -> page 6, describe all registers and how
        time values are stored. Most of it is in BCD, meaning data is stored on two units of
        four bit : one will hold unit, the other one the ten. 0001 0001 is read 11, not 17.
        This website holds a great example : https://www.digital-detective.net/binary-coded-decimal-timestamps/
    */
    return (value);
}

// sec example : (((0xf0 & value) >> 4) * 10) + (0x0f & value)

uint8_t    mode_9(uint8_t setup) {
    uint8_t hour;
    uint8_t min;
    uint8_t value;

    value = get_time_value(HOUR_REGISTER);
    // accessing the right bits as described p.8
    hour = (((0b00110000 & value) >> 4) * 10) + (0x0f & value);
    value = get_time_value(MIN_REGISTER);
    // accessing the right bits as described p.8
    min = (((0b01110000 & value) >> 4) * 10) + (0x0f & value);
    
    /**   Debug Purpose   ** */
    
    uart_printnbr_8bits(hour);
    uart_printstr(" : ");
    uart_printnbr_8bits(min);
    uart_printstr("    ");

    /**   End Debug Purpose   ** */

    return (setup);
}

uint8_t     mode_10(uint8_t setup) {
    uint8_t day;
    uint8_t month;
    uint8_t value;

    value = get_time_value(DAY_REGISTER);
    // accessing the right bits as described p.8
    day = (((0b00110000 & value) >> 4) * 10) + (0x0f & value);
    value = get_time_value(MONTH_REGISTER);
    // accessing the right bits as described p.9
    month = (((0b00010000 & value) >> 4) * 10) + (0x0f & value);
    
    /**   Debug Purpose   ** */
    
    uart_printnbr_8bits(day);
    uart_printstr(" / ");
    uart_printnbr_8bits(month);
    uart_printstr(" / ");

    /**   End Debug Purpose   ** */

    return (setup);
}

uint8_t     mode_11(uint8_t setup) {
    uint8_t year;
    uint8_t century;
    uint8_t value;

    value = get_time_value(YEAR_REGISTER);
    // accessing the right bits as described p.9
    year = (((0xf0 & value) >> 4) * 10) + (0x0f & value);
    // century flag is the first bit of month register (p.9)
    value = get_time_value(MONTH_REGISTER);
    century = value & (1 << 7);
    if (century)
        century = 20;
    else
        century = 19;

    /**   Debug Purpose   ** */

    uart_printnbr_8bits(century);
    uart_printnbr_8bits(year);
    
    /**   End Debug Purpose   ** */
    
    return (setup);
}

void set_time_value(uint8_t *time_input, uint8_t start_address, uint8_t nb_bytes) {

    i2c_start();
    // write bit is already shifted in address of doc ::::)
    // i2c_send_adr_as_sender(RTC_ADR);
    i2c_write(RTC_ADR);
    // send register address (only 4 bytes of right will be read)
    i2c_write(start_address);
    i2c_multiwrite(time_input, nb_bytes);
    i2c_stop();
}


/* first page of man : built in word address reg is automatically
    incremented after each write or read 
    Means -> from seconds to day, the memory is continuous, so we can just write to it 
    */
void    rtc_set_time(uint8_t *uart_input) {
    // to do -> a function to check if there is no invalid date/time
    uint8_t check[] = {00, 23, 05, 01};
    set_time_value(check, SEC_REGISTER, 4);
    // todo -> propre, set month, year and century
}




/***** functions to add to uart ******* */

uint8_t    uart_rx_non_blocking(uint8_t *uart_input, uint8_t len) {
    uint8_t c;

    if ((UCSR0A & (1 << RXC0)))
    {
        c = UDR0;
        // todo -> implementer parsing & error handling
        // implementer backspace & enter
        uart_input[len] = c;
        uart_tx(c);
        ++len;
    }
    return (len);
}

/**************************************** */

int main(void) {
    uint8_t value;
    uint8_t uart_input[20];
    uint8_t input_len = 0;

    uart_init();
    i2c_init();
    for (int i = 0 ; i < 20 ; i++)
        uart_input[i] = 0;
    while (42) {
        mode_9(0);
        mode_10(0);
        mode_11(0);
        uart_printstr("\r\n");
        input_len = uart_rx_non_blocking(uart_input, input_len);
        if (input_len == 20) { // minimal but sufficient for now
            rtc_set_time(uart_input);
            // beurk
            for (int i = 0 ; i < 20 ; i++)
                uart_input[i] = 0;
            input_len = 0;
        }
        _delay_ms(1000);
    }
}