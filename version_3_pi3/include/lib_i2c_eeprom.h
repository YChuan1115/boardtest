/*
 * The Library of 16K I2C Serial EEPROM
 * Company : Microchip
 * Product : 24AA16/24LC16B
 * Max Frequency :400 KHz
 * */

#ifndef __LIB_I2C_EEPROM_H
#define __LIB_I2C_EEPROM_H

#include<stdint.h>
#include<bcm2835.h>
#include<time.h>

//////////////////////////////////
// Constant and Parameter Setting
//////////////////////////////////

#define SLAVE_ADDR  0x51/*0x50*/




//////////////////////////////////
// function declaration
//////////////////////////////////

uint8_t i2c_eeprom_write_byte( uint8_t block, uint8_t addr,uint8_t _byte);
uint8_t i2c_eeprom_read_byte(uint8_t block, uint8_t addr,uint8_t * read_byte_buff);
void printReason(uint8_t res);



#endif 
