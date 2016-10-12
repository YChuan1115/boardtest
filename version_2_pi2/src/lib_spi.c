
#include <stdio.h>
#include <bcm2835.h>
//#include "lib_spi.h"
#include <lib_spi.h>


void spi_init()
{
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
}

uint8_t spi_read_manufactory_id()
{
	uint8_t cmd[6];

	cmd[0] = 0x90;
	cmd[1] = 0x90;
	cmd[2] = 0x90;
	cmd[3] = 0x00;
	cmd[4] = 0x00;
	cmd[5] = 0x00;
	bcm2835_spi_transfern(cmd,6);
	printf("spi read : %02x %02x", cmd[4], cmd[5]);
	return cmd[4];
}

uint8_t spi_read_device_id()
{
	uint8_t cmd[6];

	cmd[0] = 0x90;
	cmd[1] = 0x90;
	cmd[2] = 0x90;
	cmd[3] = 0x00;
	cmd[4] = 0x00;
	cmd[5] = 0x00;
	bcm2835_spi_transfern(cmd,6);
	printf("spi read : %02x %02x", cmd[4], cmd[5]);
	return cmd[5];
}

void spi_write_enable(void)
{
	bcm2835_spi_transfer(0x06);
}

void spi_write_disable()
{
	bcm2835_spi_transfer(0x04);
}

void spi_read_ID(uint8_t * rec_buff)
{
	uint8_t to_buff[4];
	to_buff[0] = 0x9f;
	to_buff[1] = 0x00;
	to_buff[2] = 0x00;
	to_buff[3] = 0x00;
	bcm2835_spi_transfernb(to_buff,rec_buff,4);
}

uint8_t spi_read_status_reg()
{
	uint8_t toBuff[2] = {0};
	uint8_t fromBuff[2] = {0};
	toBuff[0] = 0x05;
	bcm2835_spi_transfernb(toBuff, fromBuff, 2);
	return fromBuff[1];
}

