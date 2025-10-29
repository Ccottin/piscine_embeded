NAME		:= pgm

CC			:= avr-gcc
OBJ_COPY	:= avr-objcopy
AVRDUDE		:= avrdude

F_CPU		:= 16000000UL
CFLAGS		:= -mmcu=atmega328p -DF_CPU=$(F_CPU) -Os
FFLAGS		:= -c arduino -p m328p -b 115200 -P /dev/ttyUSB0

SRC			:= main.c
OBJ			:= $(patsubst %.c, %.bin, $(SRC))
HEX			= $(patsubst %.bin, %.hex, $(OBJ))

$(NAME) : hex flash

all : $(NAME)

hex :
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ)
	$(OBJ_COPY) -j .text -j .data -O ihex $(OBJ) $(HEX)

flash : $(HEX)
	$(AVRDUDE) $(FFLAGS) -U flash:w:$< 

clean :
	rm -rf main.hex main.bin

.PHONY : all hex flash clean