#ifndef LIB_TIMER_H
# define LIB_TIMER_H

# include <avr/io.h>
# include <avr/interrupt.h>

/* using lib timer will take over timer 1 */
// don't forget : this will overflow kickly. 
// solutions -> a bigger variable (still overflows)
// be aware & write a condition (for comapring with a previous value for example)
// write a generic function that detects overflow & handle behavior?
// might overhead, since this would take a comp in the isr
// stays at an atomic value -> prevent datarace
volatile uint8_t time_counter;

void    second_counter(void);
void    milisecond_counter(void);

#endif