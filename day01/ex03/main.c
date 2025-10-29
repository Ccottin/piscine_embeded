#include <avr/io.h>
#include <util/delay.h>

#define RATE 62500

/* Fun fact : if you modify CS12 to CS10 (prescaler from 256 to 0), the led will go on and of so fast the buttons will
actually reduce the intensity of the led */

int main(void)
{
    int     ratio;

    PORTD &= ~(1 << PORTD2); // SW1 -> increments
    PORTD &= ~(1 << PORTD4); // SW2 -> decrements

    // led 1 as output
    DDRB |= (1 << DDB1);
    // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 256 ticks
    TCCR1B |= (1 << CS12);
    // setting WGM to enable Fast Pulse Widht Modulation mode
    TCCR1B |= (1 << WGM12) | (1 << WGM13);
    TCCR1A |= (1 << WGM11);
    // setting the top value to be a second
    ICR1 = RATE;
    // Starts at led duty cycle of 10%
    OCR1A = RATE / 10;
    // Setting OC1A to set the led on when it goes over OCR1A, and off when its bottom
    TCCR1A |= (1 << COM1A1);
    // Starts with a 1 ratio
    ratio = 1;

    while (1) {
          // On button sw1 press, increments value
        if (!(PIND & (1 << PIND2)) && ratio < 10) {
            ratio++;
            // PWM value is time of activity * percentage / 100 
            OCR1A = (RATE * ratio) / 10; 
            // Empty loop to increment the counter only once per press
            while (!(PIND & (1 << PIND2))) { }
        }
        // On button sw2 press, increments value
        else if (!(PIND & (1 << PIND4)) && ratio > 1) {
            ratio--;
            // PWM value is time of activity * percentage / 100 
            OCR1A = (RATE * ratio) / 10; 
            while (!(PIND & (1 << PIND4))) { }
        }
        // debounce
        _delay_ms(20);

    }
}