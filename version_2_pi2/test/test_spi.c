#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<bcm2835.h>

void spi_write_enable(void);
void spi_read_ID(uint8_t * rec_buff);
uint8_t spi_read_status_reg();
void spi_page_program(uint32_t addr ,uint8_t * buff, uint8_t len);
void spi_read_data(uint32_t addr, uint8_t * buff, uint8_t len);
uint8_t spi_read_manufactory_id();
uint8_t spi_read_device_id();

int main(int argc,char *argv[]){

	bcm2835_init();
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	uint8_t id[4];
	spi_read_ID(id);
	printf("%02x %02x %02x \n",id[1],id[2],id[3]);
	printf("%02x\n",spi_read_manufactory_id());
	printf("%02x\n",spi_read_device_id());


	return 0;
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

void spi_page_program(uint32_t addr ,uint8_t * buff, uint8_t len){
	uint8_t toBuff[len+4];
	int i ;
	toBuff[0] = 0x02;
	toBuff[1] = (addr & 0xff0000) >> 16;
	toBuff[2] = (addr & 0xff00) >> 8;
	toBuff[3] = (addr & 0xff) ;

	for(i = 0 ;i<len ;i++ ) toBuff[i+4] = buff[i];

	bcm2835_spi_writenb(toBuff,len+4);
}


void spi_read_data(uint32_t addr, uint8_t * buff, uint8_t len)
{
	uint8_t toBuff[len+4];
	int i ;
	toBuff[0] = 0x03;
	toBuff[1] = (addr & 0x00ff0000) >> 16;
	toBuff[2] = (addr & 0x0000ff00) >> 8;
	toBuff[3] = (addr & 0x000000ff) ;
	for(i = 0 ;i < len ;i++ ) toBuff[i+4] = 0;
	bcm2835_spi_transfern(toBuff,len+4);
	for(i = 0 ;i < len ;i++ ) buff[i] = toBuff[i+4];
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
	return cmd[5];
}




