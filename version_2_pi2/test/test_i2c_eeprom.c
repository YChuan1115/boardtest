#include<stdio.h>
#include<stdlib.h>
#include"lib_i2c_eeprom.h"

int main(int argc,char *argv[]){
	bcm2835_init();
	bcm2835_i2c_begin();
	bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_150);

	printReason(i2c_eeprom_write_byte(1,2,0x5a));

	return 0;
}
