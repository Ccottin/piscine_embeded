#include "led_header.h"

/*
Red = PD5 -> OC0B
Green = PD6 -> OC0A
Blue = PD3 -> OC2B
*/

/* registers to connect/disconnect the output of the timer to leds */

/* Led R is solded to OC0B on timer 0 */
void    turn_red_pwm_on(void) {
    TCCR0A |= (1 << COM0B1);
}

void    turn_red_pwm_off(void) {
    TCCR0A &= ~(1 << COM0B1);
}

/* Led G is solded to OC0A on timer 0 */
void    turn_green_pwm_on(void) {
    TCCR0A |= (1 << COM0A1);
}

void    turn_green_pwm_off(void) {
    TCCR0A &= ~(1 << COM0A1);
}

/* Led B is solded to OC2B on timer 2 */
void    turn_blue_pwm_on(void) {
    TCCR2A |= (1 << COM2B1);
}

void    turn_blue_pwm_off(void) {
    TCCR2A &= ~(1 << COM2B1);
}

/* check if leds are on */

uint8_t red_is_set(void) {
    return (TCCR0A & (1 << COM0B1));
}

uint8_t green_is_set(void) {
    return (TCCR0A & (1 << COM0A1));
}

uint8_t blue_is_set(void) {
    return (TCCR2A & (1 << COM2B1));
}

/* no overflow (:  registers to access the duty cycle un pwm mode`*/
void    set_duty_cycle_red(uint8_t total_time, uint8_t percentage) {
    if (percentage > 100)
        percentage = 100;
    // Timer solded to red led
    OCR0B = (total_time * (percentage / 10)) / 10;
}

void    set_duty_cycle_green(uint8_t total_time, uint8_t percentage) {
    if (percentage > 100)
        percentage = 100;
    // Timer solded to green led
    OCR0A = (total_time * (percentage / 10)) / 10;
}

void    set_duty_cycle_blue(uint8_t total_time, uint8_t percentage) {
    if (percentage > 100)
        percentage = 100;
    // Timer solded to blue led
    OCR2B = (total_time * (percentage / 10)) / 10;
}