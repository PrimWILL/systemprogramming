CC = gcc
CFLAGS = -g -Wall
OBJS = adc.o arduino.o dust.o client.o lcd.o serial.o
TARGET = client

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)
	
clean:
	rm -f *.o
	rm -f $(TARGET)



adc.o: adc.h adc.c
arduino.o: arduino.h arduino.c
client.o: client.c adc.h arduino.h dust.h lcd.h serial.h
dust.o: dust.h serial.h dust.c
lcd.o: lcd.h lcd.c
serial.o: serial.h serial.c
