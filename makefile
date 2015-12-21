# Makefile for compiling sublibinal_avr

#Variables
LCC=avr-ar
LCFLAGS=rcs
CC= avr-gcc
CFLAGS= -mmcu=atmega328p -Wall -c -Iinclude/ -std=c99 
SRC_DIR=src/
OUT_DIR=build/

all: sublibinal_avr

sublibinal_avr : Timer.o ADC.o Packetizer.o Queue.o System.o UART.o ExtInterrupts.o PWM.o SPI.o 
	$(LCC) $(LCFLAGS) $(OUT_DIR)libSUBLIBinal_avr.a $(OUT_DIR)Timer.o $(OUT_DIR)ADC.o $(OUT_DIR)Packetizer.o $(OUT_DIR)Queue.o $(OUT_DIR)System.o $(OUT_DIR)UART.o $(OUT_DIR)ExtInterrupts.o $(OUT_DIR)PWM.o $(OUT_DIR)SPI.o 

Timer.o : $(SRC_DIR)Timer.c
	$(CC) $(CFLAGS) $(SRC_DIR)Timer.c -o $(OUT_DIR)Timer.o

Queue.o : $(SRC_DIR)Queue.c
	$(CC) $(CFLAGS) $(SRC_DIR)Queue.c -o $(OUT_DIR)Queue.o

ADC.o : $(SRC_DIR)ADC.c
	$(CC) $(CFLAGS) $(SRC_DIR)ADC.c -o $(OUT_DIR)ADC.o

Packetizer.o : $(SRC_DIR)Packetizer.c
	$(CC) $(CFLAGS) $(SRC_DIR)Packetizer.c -o $(OUT_DIR)Packetizer.o

System.o : $(SRC_DIR)System.c
	$(CC) $(CFLAGS) $(SRC_DIR)System.c -o $(OUT_DIR)System.o

UART.o : $(SRC_DIR)UART.c
	$(CC) $(CFLAGS) $(SRC_DIR)UART.c -o $(OUT_DIR)UART.o

ExtInterrupts.o : $(SRC_DIR)ExtInterrupts.c
	$(CC) $(CFLAGS) $(SRC_DIR)ExtInterrupts.c -o $(OUT_DIR)ExtInterrupts.o

PWM.o : $(SRC_DIR)PWM.c
	$(CC) $(CFLAGS) $(SRC_DIR)PWM.c -o $(OUT_DIR)PWM.o

SPI.o : $(SRC_DIR)SPI.c
	$(CC) $(CFLAGS) $(SRC_DIR)SPI.c -o $(OUT_DIR)SPI.o

$(shell mkdir -p $(OUT_DIR))


