#include "parse.h"

uint8_t    parse_quote(uint8_t* input, uint8_t* output, uint8_t* i) {
    uint8_t j;

    *i = 0;
    if (input[*i] != '"' || (input[*i] == '"' && input[(*i) + 1] == '"'))
        return (1);
    ++(*i);
    while (input[*i] && input[*i] != '"' && *i < 31) {
        ++(*i);
    }
    if (input[*i] != '"')
        return (1);
    j = 1;
    while (j < *i) {
        output[j - 1] = input[j];
       ++j;
    }
    ++(*i); // lets put i right after quot
    return (0);
}

/***************** Print cmd ************************ */

// todo = afficher char 0
void    uart_printmemory(const char* str)
{
    int i;

    i = 0;
    // just a loop to send char by char (:
    while(i < 16)
    {
        if ((unsigned char)str[i] > 127)
            uart_tx('.');
        else
            uart_tx(str[i]);
        ++i;
    }
}

void    display_line(uint8_t *data) {
    uart_printstr(" |");
    uart_printmemory((char*)data);
    uart_printstr("|\r\n");
}

void    print_cmd(void) {
    uint16_t    address;
    uint8_t     value;
    uint8_t     i;
    uint8_t     display[17];

    i = 0;
    address = 0;
    // should be the maximum value of address
    while (address < 1024) {
       
        if (address % 16 == 0) {    // beginning of new line, setup
            uart_print_eeprom_address(address);
            uart_printstr(" ");
            for (i = 0 ; i < 17 ; i++)
                display[i] = 0;
            i = 0;
        }
        value = eeprom_read(address);
        display[i] = value;
        uart_printnbr_hex_8bits(value);

        if (address % 2) {
            uart_printstr(" ");
        }
        if (address % 16 == 15) {   // end of line, print
            display_line(display);
        }
        ++address;
        ++i;
    }
}

// also fix read command

/***************** Forget cmd ************************ */

void    forget_cmd(uint8_t* read_input) {
    (void)read_input;
}
//     uint8_t     eeprom_data[1025];
//     uint8_t     key[31];    //input size - quote size
//     uint8_t i;

//     i = 0;
//       if (parse_quote(read_input, key, &i)) {
//         uart_printstr("\r\nIllegal quote argument");
//         return ;
//     }
//     if (read_input[i] != 0) { // trailling char after quote! unvalid
//         uart_printstr("\r\nOne argument should be given");
//         return ;
//     }
//     (void)key;
//     (void)eeprom_data;
// }

/***************** Read cmd ************************ */



void    read_cmd(uint8_t* read_input) {
    uint8_t     eeprom_data[1025];
    uint8_t     key[31];    //input size - quote size
    uint8_t     val[31];    //input size - quote size
    uint16_t    i;
    uint8_t     j;      // pas tres grave s il overflow
    uint8_t     flag = 0;   // 0 == search for magic number ; 1 == compare key ; 2 == write value

    j = 0;
      if (parse_quote(read_input, key, &j)) {
        uart_printstr("\r\nIllegal quote argument");
        return ;
    }
    if (read_input[j] != 0) { // trailling char after quote! unvalid
        uart_printstr("\r\nOne argument should be given");
        return ;
    }
    i = 0;
    while (i < 1024) {
        eeprom_data[i] = eeprom_read(i);
        if (flag == 2) {
            val[j] = eeprom_data[i];
        }
        if (flag == 1 && !(eeprom_data[i] == key[j])) {
            flag = 0;
        }
        if (flag == 1 && key[j] == 0 && eeprom_data[i] == 0) {
            flag = 2;
        }
        if (eeprom_data[i] == 0xcc) {
            j = 0;
            flag = 1;
        }
        ++i;
        ++j;
    }
    if (i == 1024)
        uart_printstr("Empty\r\n");
    else {
        uart_tx('"');
        uart_printstr((char*)val);
        uart_tx('"');
    }
}

/***************** Write cmd ************************ */

uint8_t     ft_strlen(uint8_t *s) {
    uint8_t i;

    i = 0;
    while (s[i])
        ++i;
    return (i);
}

void    write_to_spot(uint8_t *key, uint8_t *val, uint8_t adrs) {
    uint8_t i;
    
    uart_printnbr_8bits(adrs);
    i = 0;
    eeprom_write(adrs++, 0xcc);
    while (key[i]) {
        eeprom_write(adrs, key[i]);
        ++adrs;
        ++i;
    }
    i = 0;
    eeprom_write(adrs++, 0);
    while (val[i]) {
        eeprom_write(adrs, val[i]);
        ++adrs;
        ++i;
    }
    eeprom_write(adrs, 0);
    uart_printstr("Done.");
}

void    find_a_spot(uint8_t *key, uint8_t *val) {
    uint8_t     eeprom_data[1025];
    uint16_t    i;          // index of eeprom
    uint8_t     j;          // index of key on match
    uint8_t     space;      // available space since last key/val
    uint8_t     flag = 0;   // 0 == search for magic number ; 1 == compare key ; 2 == write value ; 3 == got no\0
                            // 4 == got 1 \0 
    uint8_t     required_space = ft_strlen(key) + ft_strlen(val) + 3; // + 3 -> magical nb, one /0 after key, one /0 after val

    i = 0;
    j = 0;
    space = 0;
    while (i < 1024) {
        eeprom_data[i] = eeprom_read(i);
        if (flag == 1 && !(eeprom_data[i] == key[j]))   //data dont match with key, search for end of key
            flag = 3;
        if (flag == 1 && key[j] == 0 && eeprom_data[i] == 0) {  // key exists
            uart_printstr("Key already exist!\r\n");
            return ;
        }
        if (flag == 4 && eeprom_data[i] == 0)   // we found the second \0, back to searching magic number
            flag = 0;
        if (flag == 3 && eeprom_data[i] == 0)   // we found the first \0, looking for another
            flag = 4;
        if (eeprom_data[i] == 0xcc) {           // found a magic number, lets looke for key
            j = 0;
            flag = 1;
            space = 0;
        }
        else if (flag == 0) {                     // no magic number, incrementing space counter
            ++space;
            if (space == required_space) {
                write_to_spot(key, val, i + 1 - required_space);
                return ;
            }
        }
        ++i;
        ++j;
    }
    uart_printstr("No space left");
}

void    write_cmd(uint8_t* read_input) {
    uint8_t key[31];    //input size - quote size
    uint8_t val[31];
    uint8_t i;
    uint8_t y;

    i = 0;
    while (i < 31) {
        key[i] = 0;
        val[i] = 0;
        ++i;
    }
    i = 0;
    if (parse_quote(read_input, key, &i)) {
        uart_printstr("\r\nIllegal quote argument");
        return ;
    }
    y = i;
    if (read_input[i] != ' ' || parse_quote(&read_input[++i], val, &i)) {
        uart_printstr("\r\nIllegal quote argument");
        return ;
    }
    // adding new val to previous val
    y += i + 1; // adding one because of incr on line 55
    if (read_input[y] != 0) { // trailling char after quote! unvalid
        uart_printstr("\r\nTwo arguments should be given");
        return ;
    }
    uart_printstr("\r\nkey: ");
    uart_printstr((char*)key);
    uart_printstr("\r\nkval: ");
    uart_printstr((char*)val);
    find_a_spot(key, val);
}


void    cmd_selector(uint8_t* uart_input) {
    uint8_t i;
    uint8_t temp;

    i = 0;
    while (uart_input[i] && uart_input[i] != ' ') {
        ++i;
    }   // lets put i at the end of input
    temp = uart_input[i];   //temporary putting a 0 at the end so 
    uart_input[i] = 0;      // we can check if its a knowm cmd easily
    
    if (ft_strcmp(uart_input, (uint8_t*)"READ") && temp == ' ') {
        read_cmd(&uart_input[i + 1]);
    }
    else if (ft_strcmp(uart_input, (uint8_t*)"WRITE") && temp == ' ') {
        write_cmd(&uart_input[i + 1]);

    }
    else if (ft_strcmp(uart_input, (uint8_t*)"FORGET") && temp == ' ') {
        forget_cmd(&uart_input[i + 1]);
    }
    else if (ft_strcmp(uart_input, (uint8_t*)"PRINT") && temp == 0) {
        print_cmd();
    }
    else 
        uart_printstr("Invalid Command :c");
}