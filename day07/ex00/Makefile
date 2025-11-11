NAME		:= pgm

CC			:= avr-gcc
OBJ_COPY	:= avr-objcopy
AVRDUDE		:= avrdude

F_CPU		:= 16000000
UART_BAUDRATE	:= 115200

CFLAGS		:= -Wall -Wextra -Werror -mmcu=atmega328p -DF_CPU=$(F_CPU) -DUART_BAUDRATE=$(UART_BAUDRATE) -Os
FFLAGS		:= -c arduino -p m328p -b $(UART_BAUDRATE) -P /dev/ttyUSB0

SRC			= $(wildcard *.c)
OBJ			= $(SRC:.c=.bin)
BIN			= $(NAME).bin
HEX			= $(NAME).hex

$(NAME) : hex flash

all : $(NAME) show

# rule for compiling
%.bin: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# call obj var, that calls implicitly %.bin var
hex : $(OBJ)
#	linking files together
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)
	$(OBJ_COPY) -j .text -j .data -O ihex $(BIN) $(HEX)

flash : $(HEX)
	$(AVRDUDE) $(FFLAGS) -U flash:w:$< 

clean :
	rm -rf $(HEX) $(BIN) $(OBJ)

show :
	screen /dev/ttyUSB0 115200

re : clean all

.PHONY : all hex flash clean