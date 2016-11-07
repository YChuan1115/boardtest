

#include<stdio.h>
#include<bcm2835.h>
#include<time.h>
//#include "lib_i2c_eeprom.h"
#include <lib_i2c_eeprom.h>

//////////////////////////////////
// Constant and Parameter Setting
//////////////////////////////////

#define SLAVE_ADDR 0x50





//////////////////////////////////
// function implement 
//////////////////////////////////

// INPUT : 
//     block : (0~7)   The memory block in the EEPROM
//     addr  : (0~255) Address of the memory in the EEPROM 
//     _byte : the data you want to write into EEPROM
//
// RETURN:
//     the situation of this operation 
//     use the function: printReason to print the result.
//
uint8_t i2c_eeprom_write_byte( uint8_t block, uint8_t addr,uint8_t _byte)
{
	uint8_t buf[2];
	uint8_t slave_addr = SLAVE_ADDR | (block & 0x07); 
	int retry_count = 100;
	uint8_t stop_flat = 0;
	uint8_t res;

	buf[0] = addr;
	buf[1] = _byte;
	bcm2835_i2c_setSlaveAddress(slave_addr);

	while(retry_count--){
		if(retry_count == 0) return res;
		res = bcm2835_i2c_write(buf, 2);
		switch (res){
			case BCM2835_I2C_REASON_OK:
				stop_flat = 1;
				break;
			case BCM2835_I2C_REASON_ERROR_NACK:
				break;
			case BCM2835_I2C_REASON_ERROR_CLKT:
				return res;
				break;
			case BCM2835_I2C_REASON_ERROR_DATA:
				return res;
				break;
		}
		if(stop_flat == 1) break;
		usleep(1000);
	}
	return res;
}

uint8_t i2c_eeprom_read_byte(uint8_t block, uint8_t addr,uint8_t * read_byte_buff)
{
	uint8_t res;
	uint8_t stop_flat = 0;
	int retry_count = 10;
	uint8_t slave_addr = SLAVE_ADDR | (block & 0x07);
	uint8_t buf[0];

	bcm2835_i2c_setSlaveAddress(slave_addr);

	while(retry_count--){
		if(retry_count == 0) return res;
		res = bcm2835_i2c_write(&addr, 1);
		switch (res){
			case BCM2835_I2C_REASON_OK:
				stop_flat = 1;
				break;
			case BCM2835_I2C_REASON_ERROR_NACK:
				break;
			case BCM2835_I2C_REASON_ERROR_CLKT:
				return res;
				break;
			case BCM2835_I2C_REASON_ERROR_DATA:
				return res;
				break;
		}
		if(stop_flat == 1) break;
	 	usleep(1000);
	}

	res = bcm2835_i2c_read(read_byte_buff,1); 
	return res;
}

void printReason(uint8_t res)
{
	switch(res){
		case BCM2835_I2C_REASON_OK:
			printf("i2c ok\n");
			break;
		case BCM2835_I2C_REASON_ERROR_NACK:
			printf("i2c error: No ACK!\n");
			break;
		case BCM2835_I2C_REASON_ERROR_CLKT:
			printf("i2c error: Received Clock Stretch Timeout!!\n");
			break;
		case BCM2835_I2C_REASON_ERROR_DATA:
			printf("i2c error: Not all data is sent / received.!!\n");
			break;
		default:
			printf("i2c What happen?\n");
			break;
	}
}

