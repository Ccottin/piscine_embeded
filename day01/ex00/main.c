#include <avr/io.h>

/* it seems that we are connected to an external crystal oscillatorm according to
Avrdude's low fuse all being set to 0. It seems out of bounds to use them, so we'll leave it this way.
External crystal is probably 16Mhz (c7 and c8 on boardmap).
low fuse at 0 also means the frequency is divided by 8 ; so the frequency is around 2Mhz.
It is able to yield 2 Mips (million of instruction per second), so if we want the led to be 
turned on and off for 1/2 second, we should give the mcu around 4 million instructions before it switches.
Last but not least, our loop is taking 4 instructions to increment, so 4 cycles.
2 000 000 / 4 -> 500 000 instruction per second ; 
so it will take 250 000 for count to make it to half a second
 */

int main(void) {
    unsigned long int count;

    // led 1 as output
    DDRB |= (1 << DDB1);
    count = 0;

    while (1) {
        // change led1 drive to turn it off
        PORTB &= ~(1 << PORTB1);
        while (count < 250000) { 
            count++;
        }
        // change led1 drive to turn it on
        PORTB |= (1 << PORTB1);
        while (count > 0) { 
            count--;
        }

    }
}