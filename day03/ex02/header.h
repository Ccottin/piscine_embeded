#ifndef HEADER_H
# define HEADER_H
# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>

# define CYCLE 62500

void    turn_red_pwm_on(void);
void    turn_red_pwm_off(void);
void    turn_green_pwm_on(void);
void    turn_green_pwm_off(void);
void    turn_blue_pwm_on(void);
void    turn_blue_pwm_off(void);

uint8_t red_is_set(void);
uint8_t green_is_set(void);
uint8_t blue_is_set(void);

void    set_duty_cycle_red(uint8_t total_time, uint8_t percentage);
void    set_duty_cycle_green(uint8_t total_time, uint8_t percentage);
void    set_duty_cycle_blue(uint8_t total_time, uint8_t percentage);

#endif

