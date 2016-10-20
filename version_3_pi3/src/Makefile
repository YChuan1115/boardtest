CC=gcc
SRC=lib_i2c_eeprom.c lib_lcd1602.c lib_spi.c lib_uart.c bcm2835.c board.c mcp3008.c
CFLAG=-I./ -I../include 

all:
	$(CC) -c $(SRC) $(CFLAG)

clean:
	rm *.o
