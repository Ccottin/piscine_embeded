#include <avr/io.h>
#include <util/delay.h>

#define RED (uint8_t[3]) { 0xff, 0x00, 0x00 }
#define GREEN (uint8_t[]) { 0x00, 0xff, 0x00 }
#define BLUE (uint8_t[]) { 0x00, 0x00, 0xff }
#define YELLOW (uint8_t[]) { 0xff, 0xff, 0x00 }
#define CYAN (uint8_t[]) { 0x00, 0xff, 0xff }
#define MAGENTA (uint8_t[]) { 0xff, 0x00, 0xff }
#define WHITE (uint8_t[]) { 0xff, 0xff, 0xff }

void    timer(void) {
     // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 256 ticks
    TCCR1B |= (1 << CS12);
    // CTC mode ; top will be OCR1A
    TCCR1B |= (1 << WGM12);
    // Top value set to be 1/4second
    OCR1A = (62500 / 4);
}

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
void    send_led_frames(uint8_t (*led_frames)[4]) {
        for (uint8_t i = 0; i < 3; i++) {
            for (uint8_t j = 0 ; j < 4; j++) {
                spi_send(led_frames[i][j]);
            }
    }
}

// a frame of led should be : global value, then blue, then green, then red
void    build_led_frame(uint8_t* frame, uint8_t glob, uint8_t r, uint8_t g, uint8_t b) {
    frame[0] = 0b11100000 | glob;
    frame[1] = b;
    frame[2] = g;
    frame[3] = r;
}

void    build_led_frame_but_macros(uint8_t* frame, uint8_t glob, uint8_t* val) {
    frame[0] = 0b11100000 | glob;
    frame[1] = val[2];
    frame[2] = val[1];
    frame[3] = val[0];
}

void    reset_led(uint8_t *led_tab) {
    build_led_frame(led_tab, 0, 0, 0, 0);
}

int     main(void) {
    uint8_t led_frame[3][4];
    uint8_t i = 0;

    spi_init();
    timer();
    while (42) {
    reset_led(led_frame[0]);
    reset_led(led_frame[1]);
    reset_led(led_frame[2]);
        if (i == 4)
            i = 255;    // will overflow and be 0 again
        else
            build_led_frame_but_macros(led_frame[i], 0b00010, BLUE);
        send_start_frame();
        send_led_frames(led_frame);
        send_stop_frame();
        TIFR1 |= (1 << TOV1);
        // busy wait overflow of timer 1
        while (!(TIFR1 & (1 << OCF1A))) { }
        // clearing interrupt flag of timer 1
        TIFR1 |= (1 << OCF1A);
        ++i;
    }
}