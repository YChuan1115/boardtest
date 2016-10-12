#ifndef __LIB_UART_H
#define __LIB_UART_H
#include<sys/types.h>
#include<sys/stat.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<fcntl.h>
#include<termios.h>
#include<string.h>
#include<sys/select.h>

struct serial_port{
	char device[20];
	uint16_t baud_rate;
	int fd;
	struct termios old_os;
	struct termios new_os;
};


int open_serial(struct serial_port * ser_port );
int close_serial(struct serial_port * ser_port);
int open_serial_v2(struct serial_port * ser_port );

#endif
