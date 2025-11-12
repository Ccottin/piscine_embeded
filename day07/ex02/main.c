#include "lib_uart.h"
#include "parse.h"

uint8_t eeprom_read(uint16_t address) {
    /* Wait for completion of previous read/write */
    while(EECR & (1 << EEPE)) { }
    /* Set up address register -> p.31 part 8.6.1 describes address as 10 bits */
    EEAR = address;
    /* Start eeprom read by writing EERE, hardware will set it down when done */
    EECR |= (1 << EERE);
    /* Return data from Data Register */
    return (EEDR);
}

/*
    when writting to EEprom, all values are set at 0xff at beginning ( meaning the 'clean' value is 1, not 0 )
    registers EEPM0 and EEPM1 are used to set the mode of writting. you can erase & write, or write, or erase.
    The value should be cleared (setted back to 0xFF) before being written ;
    because an only write command is only capable of setting bits from 1 to 0, but not from 0 to 1 (what the erase command does)
    by default, EEPM0 and EEPM1 are set to default 0 and 0 , which means erase and write
    This is why we'll leave thoses values as is // top of page 32
*/
void eeprom_write(uint16_t address, uint8_t val) {
    /* Wait for completion of previous read/write */
    while(EECR & (1<<EEPE)) { }
    /* Set up address and Data Registers */
    EEAR = address;
    EEDR = val;
    /* Triggers write flag (Master Write Eeprom enable), see reg 8.6.3 p.31, should be done before setting EEPE */
    EECR |= (1 << EEMPE);
    /* Start eeprom write by setting EEPE */
    EECR |= (1 << EEPE);
}

uint16_t eeprom_read_twice(uint16_t address) {
    uint16_t    ret;

    ret = eeprom_read(address) << 8;
    ret |= eeprom_read(address + 1);

    return (ret);
}

// void    read_entire_eeprom(uint8_t *eeprom_data) {
//     uint16_t    address;

//     address = 0;
//     // should be the maximum value of address
//     while (address < 1024) {
//         eeprom_data[address] = uart_printnbr_hex_8bits(eeprom_read(address));
//         ++address;
//     }
//     eeprom_data[address] = 0;
// }

void    try_write_byte(uint16_t address, uint8_t val) {
    uint8_t byte;

    if (address > 1023) {
        uart_printstr("Address is too big :c\r\n");
        return ;
    }
    byte = eeprom_read(address);
    // if bytes are differents, write in memory (:
    if (val != byte) {
        uart_printnbr_hex_8bits(val);
        eeprom_write(address, val);
        uart_printstr("\r\n");
    }
}

int main(void) {
    uint8_t     uart_input[73];
    
    uart_init();

    while (42) {
        for (uint8_t i = 0 ; i < 73 ; i++) {
            uart_input[i] = 0;
        }
        uart_getstr(uart_input);
        if (!ft_strcmp(uart_input, 0)) {
            uart_printstr("\r\n");
            cmd_selector(uart_input);
            uart_printstr("\r\n");
        }
    }
}

// todo : fix le bugg dans l ex00 avec le display de laddresse aui est f3 au lieu d etre 3f
// more test on the thingy