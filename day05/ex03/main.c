#include "lib_adc.h"
#include "lib_uart.h"

int main(void) {
  int16_t   c;

    setup_adc(INTERNAL);
    select_source(TEMP);
    uart_init();

    while (42) {
      c = start_conversion();
      // doc claims the temperature sensor is linear 🤡 
      // so lets just use a linear equation 
      // https://www.netquote.it/nqmain/2011/04/arduino-nano-v3-internal-temperature-sensor/
      // thanks to Patrick's com, we got a way to calculate the thingy in integer onlys
      uart_printnbr_16bits(c - (c / 5) + (c / 51) - 266);
      uart_printstr("\r\n");
      _delay_ms(20);
    }
}