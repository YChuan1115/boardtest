CC=gcc
EXE=board_test
CFLAG= -I./include -I./bcm2835_lib  -I../bcm2835_lib
SRC=main.c lib_i2c_eeprom.c  lib_lcd1602.c  lib_spi.c  lib_uart.c  
OBJ=lib_i2c_eeprom.o lib_lcd1602.o lib_spi.o lib_uart.o bcm2835.o board.o
VPATH=bcm2835_lib

all: srcc tf
	$(CC) -c main.c -o main.o $(CFLAG)
	$(CC) *.o -o $(EXE)
	rm *.o

srcc:
	cd ./src;make;
	cp ./src/*.o ./

tf:
	$(CC) -c test_func.c $(CFLAG)

clean:
	rm board_test;rm *.o;cd ./src ;make clean
