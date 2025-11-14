#ifndef ISR_H
# define ISR_H

# include <avr/interrupt.h>

/* Doc! 

    Calling an isr is "just" calling the associated isr_vector, linked by linker 
    into the vector table (often starts at 0x00 but i could not find it explicitly YET :} )
    This vector's  is already defined by hardware/compiler ; it is just a pointer to 
    some place in the memory where your real function is.
    
    Define __attributes__ for compiler : signal is used to tell program to do things differently
    when entering a function called by ISR.
    First, save registers states. The diable global interrupts, so nothing could interrupt the
    code's exec.
    Then, at the end of the interrupt, RETI is instructed (and not RET), restauring global interrupts.
    The registers previous states is also restaured
    then used & externally visible will prevent compiler from optimizing it
    
    Then Used is to tell compiler to not optimise/remove the function, even if it is not called explicitly
    Finally externally_visible is here to tell pretty much the same to the linker

    https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
    https://android.googlesource.com/toolchain/avr-libc/+/edcf5bc1c8da8cc4c8b560865d2a54b73c1b51d3/avr-libc-1.7.1/include/avr/interrupt.h

    */
    
# define FT_ISR(vector) \
    void vector(void)   __attribute__((signal, used, externally_visible)); \
    void vector(void)

#endif