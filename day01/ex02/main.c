#include <avr/io.h>

/* Duty Cycle = temps d'activite sur un cycle donne. Ici, le cycle est defini par la 
valeur top, soit ICR1. LE OCR1A permet de faire une comparaison, quand le timer aura 
depasse ce montant, la led s'eteindra.*/
int main(void)
{
    // led 1 as output
    DDRB |= (1 << DDB1);
    // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 256 ticks
    TCCR1B |= (1 << CS12);
    // setting WGM to enable Fast Pulse Widht Modulation mode
    TCCR1B |= (1 << WGM12) | (1 << WGM13);
    TCCR1A |= (1 << WGM11);
    // setting the top value to be a second
    ICR1 = 62500;
    // PWM value is time of activity * percentage / 100
    OCR1A = 6250;
    // Setting OC1A to set the led off when it goes over OCR1A, and on when its bottom
    TCCR1A |= (1 << COM1A1);


    while (1) { }
}