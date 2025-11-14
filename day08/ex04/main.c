#include <avr/io.h>
#include <util/delay.h>
#include "lib_uart.h"
#include "isr.h"

#define RED (uint8_t[3]) { 0xff, 0x00, 0x00 }
#define GREEN (uint8_t[]) { 0x00, 0xff, 0x00 }
#define BLUE (uint8_t[]) { 0x00, 0x00, 0xff }
#define YELLOW (uint8_t[]) { 0xff, 0xff, 0x00 }
#define CYAN (uint8_t[]) { 0x00, 0xff, 0xff }
#define MAGENTA (uint8_t[]) { 0xff, 0x00, 0xff }
#define WHITE (uint8_t[]) { 0xff, 0xff, 0xff }

/* datasheet for led = 
    https://www.pololu.com/file/0J1234/sk9822_datasheet.pdf
    p.2 -> max frequency = 30Mhz, as our mcu got 16Mhz
    Golbal value is used for brightness

*/
/*  
    MOSI -> pb3
    sck -> pb5
    cs  -> pb2
    Miso is not required
*/

volatile uint8_t led_frames[3][4];


void    milisecond_counter(void) {
     // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 8 ticks
    TCCR1B |= 1 << CS11;
    // CTC mode ; top will be OCR1A
    TCCR1A |= 1 << WGM12;
    // Top value set to be a milisecond -> 16mhz / 64 / 1000
    OCR1A = 2000;
    // Enable interrupts on timer 1, then interrupts on CTC match for OCR1a 
}


// code as specified p.172
void    spi_init(void) {
    // p.172 set mosi, cs & sck as output
    DDRB |= (1 << DDB2) | (1 << DDB5) | (1 << DDB3);
    // Enable spi bus as master ; spr0 & friends stays untouched (no prescaler required)
    // p.176 -> SPE = SPI enable,
    // -> MSTR = master enable
    SPCR = (1 << SPE) | (1 << MSTR);
  

    // data mode will stay CPOL=0 and CPHA = 0 so no touchy regs CPOL & CPHA in SPCR
    // Also no touchy DORD fot data order (little or smol endian) p.176
    // Default values be : CPOL=0 && CPHA = 0 ; DORD = 0 -> MSB first
}

void spi_kill(void)
{
    SPCR &= ~(1 << SPE);
}


void    spi_send(uint8_t data) {
    // write data to reg; p.178
    SPDR = data;
    // wait for transfer complete p.177
    while (!(SPSR & (1 << SPIF))) { }
}

// start_frame = 32bits of 0 -> 4*8 (doc p.4)
void    send_start_frame(void) {
    for (uint8_t i = 0; i < 4; i++) {
        spi_send(0);
    }
}

// end_frame = 32bits of 1 -> 4*8 (doc p.4) 
void    send_stop_frame(void) {
    for (uint8_t i = 0; i < 4; i++) {
        spi_send(0xff);
    }
}

// Huglyyyyy
void    send_led_frames(void) {
        for (uint8_t i = 0; i < 3; i++) {
            for (uint8_t j = 0 ; j < 4; j++) {
                spi_send(led_frames[i][j]);
            }
    }
}

// a frame of led should be : global value, then blue, then green, then red
void    build_led_frame(uint8_t index, uint8_t glob, uint8_t r, uint8_t g, uint8_t b) {
    if (index > 2)
        return ;
    led_frames[index][0] = 0b11100000 | glob;
    led_frames[index][1] = b;
    led_frames[index][2] = g;
    led_frames[index][3] = r;
}

void    reset_led(uint8_t index) {
    build_led_frame(index, 0, 0, 0, 0);
}

// muh
uint8_t hexa_to_integer(char first, char second) {
    if (first >= 'A' && first <= 'F') {
        second -= 55; // - 65 + 10
    } else if (first >= '0' && first <= '9') {
        first -= 48;
    }
     if (second >= 'A' && second <= 'Z') {
        second -= 55; // - 65 ('A') + 10
    } else if (second >= '0' && second <= '9') {
        second -= 48;
    }
    return(first * 16) + second;
}

void wheel(uint8_t pos)
{
    pos = 255 - pos;
    if (pos < 85) {
        for (uint8_t i  = 0 ; i < 3 ; i++) {
            build_led_frame(i, 0b00001, 255 - pos * 3, 0, pos * 3);
    }
    } else if (pos < 170) {
        pos = pos - 85;
        for (uint8_t i  = 0 ; i < 3 ; i++) {
            build_led_frame(i, 0b00001, 0, pos * 3, 255 - pos * 3);
    }
    } else {
        pos = pos - 170;
        for (uint8_t i  = 0 ; i < 3 ; i++) {
            build_led_frame(i, 0b00001, pos * 3, 255 - pos * 3, 0);
    }
    }
}

uint8_t is_hex(char c) {
    uint8_t hex_chars[] = { 'A', 'B', 'C', 'D', 'E', 'F', \
                        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 0 };
    uint8_t i;

    i = 0;
    while (hex_chars[i]) {
        if (c == hex_chars[i])
            return (1);
        ++i;
    }
    return (0);
}


void    reset_leds(void) {
    reset_led(0);
    reset_led(1);
    reset_led(2);
}


char    parse_color(char *color_code, uint8_t *magic) {
   
    int     i;

    if (ft_strcmp(color_code, "#FULLRAINBOW"))
    {
        // Enable interrupts on timer 1, then interrupts on CTC match for OCR1a 
        TIMSK1 |= 1 << ICIE1 | 1 << OCIE1A;
        *magic = 1;
        return (0);
    }
    if (*magic == 1) {
        TIMSK1 &= ~(1 << ICIE1 | 1 << OCIE1A);
        reset_leds();
        *magic = 0;
    }
    if (color_code[0] != '#')
        return (1);
    i = 1;
    while (color_code[i] && is_hex(color_code[i]) && i < 7) {   // check if rgbval is correct
        i++;
    }
    if (i != 7)
        return (1);
    if (!color_code[i] || color_code[i] != 'D' || color_code[i + 2] != 0) // checking char D and last index
        return (1);
    if (!(color_code[i + 1] == '6' || color_code[i + 1] == '7' || color_code[i + 1] == '8'))
        return (1);
    build_led_frame((color_code[i + 1] - 48 - 6), 0b00001, hexa_to_integer(color_code[1], color_code[2]), \
        hexa_to_integer(color_code[3], color_code[4]), \
        hexa_to_integer(color_code[5], color_code[6]));
    return (0);
}

FT_ISR(TIMER1_COMPA_vect) {
    static uint8_t i = 0;

    TIFR1 |= (1 << OCF1A);
    wheel(i);
    ++i;
    send_start_frame();
    send_led_frames();
    send_stop_frame();
}

int     main(void) {
    char     color_code[13];
    uint8_t     magic = 0;

    
    sei();
    spi_init();
    uart_init();
    milisecond_counter();
    reset_leds();
    while (42) {
        uart_getstr(color_code);
        if (parse_color(color_code, &magic))
            uart_printstr(" X Invalid input.\r\n");
        else
        {
            send_start_frame();
            send_led_frames();
            send_stop_frame();
            uart_printstr(" -> Done.\r\n");
            
        }
    }
}