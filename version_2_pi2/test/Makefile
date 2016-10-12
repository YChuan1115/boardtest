CC=gcc
CFLAG=-I../include

all: test_ir test_i2c_eeprom test_spi

test_ir:
	$(CC) -c test_ir.c -o ir_test $(CFLAG)

test_i2c_eeprom:
	$(CC) test_i2c_eeprom.c -o eeprom_test $(CFLAG)

test_spi:
	$(CC) test_spi.c -o spi_flash_test $(CFLAG)
