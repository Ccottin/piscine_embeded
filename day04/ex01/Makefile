NAME		:= pgm

CC			:= avr-gcc
OBJ_COPY	:= avr-objcopy
AVRDUDE		:= avrdude

F_CPU		:= 16000000UL
UART_BAUDRATE	:= 115200

CFLAGS		:= -mmcu=atmega328p -DF_CPU=$(F_CPU) -DUART_BAUDRATE=$(UART_BAUDRATE) -Os
FFLAGS		:= -c arduino -p m328p -b $(UART_BAUDRATE) -P /dev/ttyUSB0

SRC			= $(wildcard *.c)
OBJ			= $(SRC:.c=.bin)
ELF			= $(NAME).elf
HEX			= $(NAME).hex

$(NAME) : hex flash

all : $(NAME)

%.bin: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# call obj var, that calls implicitly %.bin var
hex : $(OBJ)
#	linking files together
	$(CC) $(CFLAGS) $(OBJ) -o $(ELF)
	$(OBJ_COPY) -j .text -j .data -O ihex $(ELF) $(HEX)

flash : $(HEX)
	$(AVRDUDE) $(FFLAGS) -U flash:w:$< 

clean :
	rm -rf $(HEX) $(ELF) $(OBJ)

.PHONY : all hex flash clean