CC=avr-gcc
#MCU=atmega328p
MCU=atmega2560
F_CPU=16000000UL
CFLAGS= -g -Os -Wall
TARGET=robot
CFLAGS += -DF_CPU=$(F_CPU)
bin: main.o
#	$(CC) $(CFLAGS) -mmcu=$(MCU) main.o usart.o interrupt.o timer.o -o $(TARGET)
	$(CC) $(CFLAGS) -mmcu=$(MCU) main.o -o $(TARGET)

hex :
	avr-objcopy -O ihex -R .eeprom $(TARGET) $(TARGET).hex 

flash:
	avrdude -v -v -v -c arduino -p m328p -P /dev/ttyUSB0 -b57600 -U flash:w:$(TARGET).hex
	
main.o:main.c
	$(CC) $(CFLAGS) -mmcu=$(MCU) -c main.c

#radio.o: src/radio.c include/radio.h
#	$(CC) $(CFLGS) -mmcu=$(MCU) -c src/radio.c

clean:
	rm -f *.o $(TARGET)*






