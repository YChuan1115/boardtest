#ifndef __LIB_SPI_H
#define __LIB_SPI_H

void spi_init();
void spi_write_enable(void);
void spi_read_ID(uint8_t * rec_buff);
uint8_t spi_read_status_reg();
uint8_t spi_read_manufactory_id();
uint8_t spi_read_device_id();




#endif 
