CC=avr-gcc
#MCU=atmega328p
MCU=atmega2560
PORT=/dev/ttyACM0
F_CPU=16000000UL
CFLAGS= -g -Os -Wall
TARGET=robot
CFLAGS += -DF_CPU=$(F_CPU)
bin: main.o pwm.o util.o adc.o usart.o
#	$(CC) $(CFLAGS) -mmcu=$(MCU) main.o usart.o interrupt.o timer.o -o $(TARGET)
	$(CC) $(CFLAGS) -mmcu=$(MCU) main.o pwm.o util.o adc.o usart.o -o $(TARGET)

hex :
	avr-objcopy -O ihex -R .eeprom $(TARGET) $(TARGET).hex 

flash:
	avrdude -v -v -v -c wiring -p atmega2560 -P $(PORT) -b115200 -U flash:w:$(TARGET).hex
	
main.o:main.c
	$(CC) $(CFLAGS) -mmcu=$(MCU) -c main.c

pwm.o: src/pwm.c include/pwm.h
	$(CC) $(CFLGS) -mmcu=$(MCU) -c src/pwm.c

adc.o: src/adc.c include/adc.h
	$(CC) $(CFLGS) -mmcu=$(MCU) -c src/adc.c

util.o: src/util.c include/util.h
	$(CC) $(CFLGS) -mmcu=$(MCU) -c src/util.c

usart.o: src/usart.c include/usart.h
	$(CC) $(CFLGS) -mmcu=$(MCU) -c src/usart.c

clean:
	rm -f *.o $(TARGET)*






