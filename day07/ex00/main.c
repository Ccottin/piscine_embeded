#include "lib_uart.h"

/*
    Doc!
    -> p.29
    1Kbytes data memory on its own dedicated space
    endurance of at least 100,000 write/erase cycle. ( :c )

    Write access cannot be at anytime
        -> specific procedure must be followed
    
When the EEPROM is read, the CPU is halted for four clock cycles before the next instruction
    is executed. (per byte i believe)
When the EEPROM is written, the CPU is halted for two clock cycles before the next instruction
    is executed.

    always disable interrupts ( if there are some ) before writting to eeprom, there is a change
    to corrupt write when it happens -> p.32, Bit 1 – EEPE: EEPROM Write Enable, Caution paragraph

*/

// Doc provides us a neet example on page 35 :)
uint8_t eeprom_read(uint16_t address) {
    /* Wait for completion of previous write */
    while(EECR & (1 << EEPE)) { }
    /* Set up address register -> p.31 part 8.6.1 describes address as 10 bits */
    EEAR = address;
    /* Start eeprom read by writing EERE, hardware wiil set it down when done */
    EECR |= (1 << EERE);
    /* Return data from Data Register */
    return (EEDR);
}

uint16_t eeprom_read_twice(uint16_t address) {
    uint16_t    ret;

    ret = eeprom_read(address) << 8;
    ret |= eeprom_read(address + 1);

    return (ret);
}

/*
    address ranges from 0000 to 1023 -> p.31 part 8.6.1
*/
void    read_entire_eeprom(void) {
    uint16_t    address;

    address = 0;
    // should be the maximum value of address
    while (address < 1024) {
        if (address % 16 == 0) {
            uart_printstr("\r\n");
            uart_print_eeprom_address(address);
        }
        uart_printstr(" ");
        uart_printnbr_hex_16bits(eeprom_read_twice(address));
        address += 2;
    }
}


int main(void) {
    uart_init();
    read_entire_eeprom();
    while (42) { }
}