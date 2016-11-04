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


//#include "lib_uart.h"
#include <lib_uart.h>


// input device 
// ex: "/dev/ttyAMA0" or "/dev/ttyUSB0"
int open_serial(struct serial_port * ser_port )
{
	//////////////////////////
	// open the device 
	//////////////////////////
	ser_port->fd = open(ser_port->device , O_RDWR | O_NOCTTY | O_NDELAY );
	if(ser_port->fd < 0){
		fprintf(stderr, "ERROR: can't open %s !!!\n",ser_port->device );
		return -1;
	}
	printf("open device : %s success !\n", ser_port->device );
	// --------------------------- //


	//////////////////////////
	// backup the termios 
	//////////////////////////
	tcgetattr(ser_port->fd, &ser_port->old_os);
	// --------------------------- //


	//////////////////////////
	// set up the baud rate of the new termios 
	//////////////////////////
	tcgetattr(ser_port->fd, &ser_port->new_os);
	cfsetispeed(&ser_port->new_os,B9600/*B115200*/); // input speed 115200
	cfsetospeed(&ser_port->new_os,B9600/*B115200*/); // output speed 115200
	// --------------------------- //

	//////////////////////////
	// set up the local transfer mode flag
	//////////////////////////
	ser_port->new_os.c_lflag &=~( ECHO | ECHOE | ISIG);
	// --------------------------- //

	//////////////////////////
	// set up the control trasnfer mode flag
	// 8N1 : 8 bits a word, no parity bit, 1 stop bit
	//////////////////////////
	ser_port->new_os.c_cflag &=~( PARENB );
	ser_port->new_os.c_cflag &=~( CSTOPB );
	ser_port->new_os.c_cflag &=~( CSIZE  );
	ser_port->new_os.c_cflag |= ( CS8    );
	// turn off the Hardware flow control
	ser_port->new_os.c_cflag &=~( CRTSCTS );
	// --------------------------- //


	//////////////////////////
	// setup the output transfer mode flag
	//////////////////////////
	ser_port->new_os.c_oflag &=~( OPOST );

	//////////////////////////
	// setup the input transfer mode flag
	//////////////////////////
	ser_port->new_os.c_iflag = 0;
	//ser_port->new_os.c_cc[VTIME] = 4;
	//ser_port->new_os.c_cc[VMIN] = 5;
	ser_port->new_os.c_cc[VTIME] = 0;
	ser_port->new_os.c_cc[VMIN] = 1;

	//////////////////////////
	// START NEW TERMIOS
	//////////////////////////
	if(tcsetattr(ser_port->fd, TCSAFLUSH, &ser_port->new_os) < 0){
		fprintf(stderr,"ERROR");
		return -1;
	}
	return 0;
}

int close_serial(struct serial_port * ser_port)
{
	// RECOVER 
	int res;
	res = tcsetattr( ser_port->fd, TCSAFLUSH, &ser_port->old_os);
	close(ser_port->fd);
	if(res == 0 ){
		fprintf(stderr,"close: %s\n", ser_port->device);
	}else{
		fprintf(stderr,"error close: %s!!!!\n", ser_port->device);
	}
	return res;
}

int nonblock_read_tty(struct serial_port * ser_port, char * buff, uint8_t len)
{
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 30000;
	fd_set read_fd;

	FD_ZERO(&read_fd);
	fprintf(stderr,"start to nonblock read\n");
	FD_SET(ser_port->fd ,&read_fd);
	int res = select(ser_port->fd+1, &read_fd, NULL, NULL, &timeout);
	if(res == -1){
		fprintf(stderr,"ERROR: select function\n");
		return res;
	}else if(FD_ISSET(ser_port->fd, &read_fd)){
		read(ser_port->fd, buff, len);
		return 1;
	}
	return 0;
}

int nonblock_read_stdin(char * buff, uint8_t len)
{
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 30000;
	fd_set read_fd;

	int keyboard = open("/dev/tty", O_RDONLY | O_NONBLOCK);
	int i ;

	FD_ZERO(&read_fd);
	//fprintf(stderr,"start to nonblock read\n");
	FD_SET(keyboard ,&read_fd);
	int res = select(keyboard + 1, &read_fd, NULL, NULL, &timeout);
	if(res == -1){
		fprintf(stderr,"ERROR: select function\n");
		close(keyboard);
		return res;
	}else if(FD_ISSET(keyboard, &read_fd)){
		for(i=0 ;i<len ;i++ ) buff[i] = '\0';
		read(keyboard, buff, len);
		close(keyboard);
		return 1;
	}
	close(keyboard);
	return 0;
}


int open_serial_v2(struct serial_port * ser_port )
{
	//////////////////////////
	// open the device 
	//////////////////////////
	ser_port->fd = open(ser_port->device , O_RDWR | O_NOCTTY | O_NDELAY );
	if(ser_port->fd < 0){
		fprintf(stderr, "ERROR: can't open %s !!!\n",ser_port->device );
		return -1;
	}
	printf("open device : %s success !\n", ser_port->device );
	// --------------------------- //


	//////////////////////////
	// backup the termios 
	//////////////////////////
	tcgetattr(ser_port->fd, &ser_port->old_os);
	// --------------------------- //


	//////////////////////////
	// set up the baud rate of the new termios 
	//////////////////////////
	tcgetattr(ser_port->fd, &ser_port->new_os);
	cfsetispeed(&ser_port->new_os,B115200); // input speed 115200
	cfsetospeed(&ser_port->new_os,B115200); // output speed 115200
	// --------------------------- //

	//////////////////////////
	// set up the local transfer mode flag
	//////////////////////////
	ser_port->new_os.c_lflag &=~( ECHO | ECHOE | ISIG);
	// --------------------------- //

	//////////////////////////
	// set up the control trasnfer mode flag
	// 8N1 : 8 bits a word, no parity bit, 1 stop bit
	//////////////////////////
	ser_port->new_os.c_cflag &=~( PARENB );
	ser_port->new_os.c_cflag &=~( CSTOPB );
	ser_port->new_os.c_cflag &=~( CSIZE  );
	ser_port->new_os.c_cflag |= ( CS8    );
	// turn off the Hardware flow control
	ser_port->new_os.c_cflag &=~( CRTSCTS );
	// --------------------------- //


	//////////////////////////
	// setup the output transfer mode flag
	//////////////////////////
	ser_port->new_os.c_oflag &=~( OPOST );

	//////////////////////////
	// setup the input transfer mode flag
	//////////////////////////
	ser_port->new_os.c_iflag = 0;
	//ser_port->new_os.c_cc[VTIME] = 4;
	//ser_port->new_os.c_cc[VMIN] = 5;
	ser_port->new_os.c_cc[VTIME] = 0;
	ser_port->new_os.c_cc[VMIN] = 1;

	ser_port->new_os.c_iflag = 0;
	ser_port->new_os.c_oflag = 0;
	ser_port->new_os.c_cflag = 0x18B2;
	ser_port->new_os.c_lflag = 0;

	//////////////////////////
	// START NEW TERMIOS
	//////////////////////////
	if(tcsetattr(ser_port->fd, TCSAFLUSH, &ser_port->new_os) < 0){
		fprintf(stderr,"ERROR");
		return -1;
	}
	return 0;
}
