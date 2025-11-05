#ifndef ISR_H
# define ISR_H

# include <avr/interrupt.h>
// define attributes for compiler : signal is used to save registers states,
// then used & externally visible will prevent compiler/linker from optimizing/removing it
# define FT_ISR(vector) \
    void vector(void)   __attribute__((signal, used, externally_visible)); \
    void vector(void)

# define FT_ISR_ATR(vector, attribute) \
void vector(void)   __attribute__((signal, used, externally_visible, attribute)); \
void vector(void)

#endif