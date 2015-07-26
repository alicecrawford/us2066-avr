MCU = atmega328
F_CPU = 8000000
PROGRAMMER = usbtiny

TARGET = us2066

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

CFLAGS = -std=c99 -Wall -O1 -mmcu=$(MCU) -DF_CPU=8000000

PGMFLAGS =

CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

all: $(TARGET).hex

src/%.o: src/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

$(TARGET).hex: $(TARGET)
	$(OBJCOPY) -O ihex $(TARGET) $(TARGET).hex

program: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -U flash:w:$(TARGET).hex

clean:
	rm -rf $(OBJ) $(TARGET) $(TARGET).hex
