#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdint.h>
#include<string.h>

#include<board.h>
#include"test_func.h"


void print_dev_res(board_device *dev);
void cleanBuff(char * buf,int len);


int main(void){

	int i;

	// #####################################
	// === initialization 
	// #####################################
	bcm2835_init();
	board_init();

	// #####################################
	// === setup the devices 
	// #####################################
	uint8_t id_list[2] = {0xc2,0x12};
	board_device device_lists[] = {
		{
			.id = 1,
			.name = "LED 01",
			.phy_pin_num = 1,
			.phy_pin_list = {RPI_BPLUS_GPIO_J8_13},
			.flag_ignore = 0,
			.test_func = test_func_led,
			.arg = NULL,
		},
		{
			.id = 2,
			.name = "LED 02",
			.phy_pin_num = 1,
			.phy_pin_list = {RPI_BPLUS_GPIO_J8_15},
			.flag_ignore = 0,
			.test_func = test_func_led,
			.arg = NULL,
		},
		{
			.id = 3,
			.name = "Button 01",
			.phy_pin_num = 1,
			.phy_pin_list = {RPI_BPLUS_GPIO_J8_18},
			.flag_ignore = 0,
			.test_func = test_func_button,
			.arg = NULL,
		},
		{
			.id = 4,
			.name = "Button 02",
			.phy_pin_num = 1,
			.phy_pin_list = {RPI_BPLUS_GPIO_J8_16},
			.flag_ignore = 0,
			.test_func = test_func_button,
			.arg = NULL,
		},
		{
			.id = 5,
			.name = "Buzzer",
			.phy_pin_num = 1,
			.phy_pin_list = {RPI_BPLUS_GPIO_J8_32},
			.flag_ignore = 0,
			.test_func = test_func_buzzer,
			.arg = NULL,
		},
		{
			.id = 6,
			.name = "Relay ",
			.phy_pin_num = 1,
			.phy_pin_list = {RPI_BPLUS_GPIO_J8_11},
			.flag_ignore = 0,
			.test_func = test_func_realy,
			.arg = NULL,
		},
		{
			.id = 7,
			.name = "IR",
			.phy_pin_num = 1,
			.phy_pin_list = {RPI_BPLUS_GPIO_J8_22},
			.flag_ignore = 0,
			.test_func = test_func_ir,
			.arg = NULL,
		},
		{
			.id = 8,
			.name = "SPI FLASH",
			.phy_pin_num = 0,
			.phy_pin_list = {0},
			.flag_ignore = 0,
			.test_func = test_func_spi_flash,
			.arg = (void *)id_list,
		},
		{
			.id = 9,
			.name = "I2C EEPROM",
			.phy_pin_num = 0,
			.phy_pin_list = {0},
			.flag_ignore = 0,
			.test_func = test_func_eeprom,
			.arg = NULL,
		},
		{
			.id = 10,
			.name = "16x2 LCD",
			.phy_pin_num = 7,
			.phy_pin_list = {
				RPI_BPLUS_GPIO_J8_35,
				RPI_BPLUS_GPIO_J8_33,
				RPI_BPLUS_GPIO_J8_31,
				RPI_BPLUS_GPIO_J8_29,
				RPI_BPLUS_GPIO_J8_38,
				RPI_BPLUS_GPIO_J8_40,
				RPI_BPLUS_GPIO_J8_37},
			.flag_ignore = 0,
			.test_func = test_func_lcd,
			.arg = NULL,
		},
		{
			.id = 11,
			.name = "UART and PL2303",
			.phy_pin_num = 0,
			.phy_pin_list = {0},
			.flag_ignore = 0,
			.test_func = test_func_uart,
			.arg = NULL,
		},
	};

	








	// #####################################
	// === test functions loop
	// #####################################
	int len = sizeof(device_lists) / sizeof(board_device);

	for(i=0 ;i<len;i++ ){
		printf("==================================\n");
		if(!device_lists[i].test_func(&device_lists[i]))
			fprintf(stderr,"Beep!!\n\a");
	}
	


	// #####################################
	// === show the results
	// #####################################
	int passed_num = 0;
	int ignored_num = 0;
	int fail_num = 0;
	int undefined_num = 0;
	printf("======================================================================\n");
	for(i=0 ;i<len ;i++ ){
		print_dev_res(&device_lists[i]);
	printf("======================================================================\n");
		if(device_lists[i].Status == PASS) passed_num ++;
		if(device_lists[i].Status == FAIL) fail_num ++;
		if(device_lists[i].Status == IGNORE) ignored_num ++;
		if(device_lists[i].Status == UNDEFINED) undefined_num++;
	}
	printf("-----------FAILED : %d \n", fail_num);
	printf("-------IGNORED : %d\n", ignored_num);
	printf("---UNDEFINED : %d\n", undefined_num);
	printf("PASSED    : %d\n", passed_num);

	return 0;
}



void print_dev_res(board_device *dev )
{
	char _buff[100];
	char _tmp[100];
	int i ;
	cleanBuff(_buff,100);

	// print id
	cleanBuff(_tmp,100);
	sprintf(_tmp, "| %3d|", dev->id );
	strcat(_buff,_tmp);
	
	// print device name
	cleanBuff(_tmp,100);
	sprintf(_tmp, " %20s|", dev->name);
	strcat(_buff,_tmp);
	
	// print pin list
	cleanBuff(_tmp,100);
	for(i=0 ; i < dev->phy_pin_num ;i++ ){
		char __tmp[10];
		cleanBuff(__tmp,10);
		sprintf( __tmp, " %d" , dev->phy_pin_list[i] );
		strcat(_tmp,__tmp);
	}
	char fff[100];
	sprintf(fff,"%20s",_tmp);
	strcat(_buff,fff);
	strcat(_buff,"|");
	
	// print pass or ignored
	cleanBuff(_tmp,100);
	if(dev->Status == PASS)
		sprintf(_tmp,"%-20s"," PASS ");
	else if (dev->Status == FAIL)
		sprintf(_tmp,"%-20s"," !! FAIL !!");
	else if(dev->Status == IGNORE)
		sprintf(_tmp,"%-20s"," -- IGNORE --");
	else if(dev->Status == UNDEFINED)
		sprintf(_tmp,"%-20s","?? UNDEFINED ??");
	strcat(_buff,_tmp);
	strcat(_buff,"|");

	printf("%s\n",_buff);
	
}


void cleanBuff(char * buf,int len)
{
	int i ;
	for(i = 0 ;i<len ;i++ ) buf[i] = 0;
}
