#include"test_func.h"
#include<board.h>
#include<stdlib.h>
#include<stdio.h>
#include<lib_uart.h>
#include<lib_i2c_eeprom.h>
#include<lib_spi.h>
#include<lib_lcd1602.h>
#include<mcp3008.h>
#include<lis3dh.h>
#include<bcm2835.h>
char buff[30];
int rres;

// add your test function defination after ...
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~










// default test_func_*
// ~~~~~~~~~~~~~~~~~~~~~~
//

int test_func_led        (board_device * self)
{
char _buff[30];
        int i = 0;
        //bcm2835_gpio_write(PIN_LED_01,HIGH);
        // init the pin setup
        int led_pin = self->phy_pin_list[0];
        SET_OUTPUT(PIN_COM);
        bcm2835_gpio_write(PIN_COM,HIGH);
        SET_OUTPUT(led_pin);


        printf("LED %s Light?[Y/n]:", self->name );
        bcm2835_gpio_write(led_pin, HIGH);

        for(i = 0 ;i<30 ;i++ ) _buff[i] = '\0';
        fgets(_buff, 30, stdin);

        if((_buff[0] == '\n') || (_buff[0] == 'y') || (_buff[0] == 'Y'))
                self->Status = PASS;

        else if((_buff[0] == 'n') || (_buff[0] == 'N'))
                self->Status = FAIL;

        bcm2835_gpio_write(led_pin, LOW);

        return (self->Status == PASS)?(1):(0);
}

int test_func_button     (board_device * self)
{
	char _buff[30];
	int i = 0;

	int count = 0;

	// setup the pin out setup 
	int btn_pin = self->phy_pin_list[0];
	SET_INPUT(btn_pin);

	printf("\n=======================================\n");
	printf("Press %s  twice for pass this check!\n", self->name );
	printf("Or press any key of keyboard to failed this check\n");
	printf("=======================================\n");

	while(1){
		// check the keyboard button press 
		int res = nonblock_read_stdin(_buff, 30);
		if(res == -1){
			printf("ERROR : select\n");
			return -2;
		}else if (res > 0){
			self->Status = FAIL;
			break;
		}

		// check the button 1 press
		if(check_button(btn_pin) == BTN_UP_EDGE) count ++;

		// check the button 1 press twice !
		if(count == 2){
			printf("%s Succeed!!\n", self->name);
			self->Status = PASS;
			break;
		}
	}
	return (self->Status == PASS)?(1):(0);
}

int test_func_buzzer     (board_device * self)
{
	char _buff[30];
	int i ;

	char ch;
	int buzzer_pin = self->phy_pin_list[0];

	SET_OUTPUT(buzzer_pin);
	// buzzer off
	DIGIT_WRITE(buzzer_pin, HIGH);


	printf("Buzzer TEST: Do you hear the louder noize? [Y/n]:");

	i = 500;
	while(i--){
		DIGIT_WRITE(buzzer_pin, LOW);
		usleep(1000);
		DIGIT_WRITE(buzzer_pin, HIGH);
		usleep(1000);
	}

	//clean_buffer(); fgets(buff, BUFF_LEN, stdin);
	for(i = 0 ;i<30 ;i++ ) _buff[i] = '\0'; fgets(_buff, 30, stdin);

	if((_buff[0] == '\n') || (_buff[0] == 'y') || ( _buff[0] == 'Y'))
		self->Status = PASS;
	else if((_buff[0] == 'n') || (_buff[0] == 'N'))
		self->Status = FAIL;

	DIGIT_WRITE(buzzer_pin, HIGH);
	SET_INPUT(buzzer_pin);
	return (self->Status == PASS)?(1):(0);
}

int test_func_realy      (board_device * self)
{
	char _buff[30]; int i ;
	int pin_relay = self->phy_pin_list[0];

	printf("TEST: Relay %s on and off\n", self->name );
	printf("Do you hear the sound of switching relay? [Y/n]:\n");
	SET_OUTPUT(pin_relay);
	DIGIT_WRITE(pin_relay, LOW);

	while(1){
		// check the keyboard button press 
		int res = nonblock_read_stdin(_buff,30);
		if(res == -1){
			printf("ERROR : select\n");
			return -2;
		}else if (res > 0){
			if((_buff[0] == '\n') || (_buff[0] == 'y') || (_buff[0] == 'Y')){
				self->Status = PASS;
				break;
			}else{
				self->Status = FAIL;
				break;
			}
		}
		// END check the keyboard input

		// relay on 
		DIGIT_WRITE(pin_relay, HIGH);
		usleep(500000);
		// realy off
		DIGIT_WRITE(pin_relay, LOW);
		usleep(500000);
	}
	SET_INPUT(pin_relay);
	return (self->Status == PASS)?(1):(0);
}

int test_func_ir         (board_device * self)
{
	int break_flag = 0;
	char _buff[30];int i ;
	int pin_ir = self->phy_pin_list[0];
	SET_INPUT(pin_ir);
	printf("START to TEST IR module: %s.\n", self->name );
	printf("Please Send Some IR Signal to Pi board.\n");
	printf("\npress any key to skip.\n");

	while(1){
		// check keyboard input
		int res = nonblock_read_stdin(_buff,30);
		if(res == -1){
			fprintf(stderr,"ERROR: select fail\n");
			return -2;
		}else if (res > 0){
			self->Status = FAIL;
			break;
		}

		// check ir 
		if( check_ir_pin(pin_ir)  == BTN_DOWN_EDGE){
			struct timeval pre;
			struct timeval now;
			gettimeofday(&pre,NULL);
			while(1){
				if(check_ir_pin(pin_ir) == BTN_DOWN_EDGE){
					gettimeofday(&now,NULL);
					long delta_time = 1000000*( now.tv_sec - pre.tv_sec)+(now.tv_usec - pre.tv_usec);
					pre.tv_sec = now.tv_sec; pre.tv_usec = now.tv_usec;
					printf("dt:%6ld\n", delta_time);
				}else{
					gettimeofday(&now,NULL);
					long delta_time = 1000000*( now.tv_sec - pre.tv_sec)+(now.tv_usec - pre.tv_usec);
					if(delta_time > 300000){
						//////////////////////////////////////
						printf("IR: %s test ok !\n", self->name );
						self->Status = PASS;
						break_flag = 1;
						break;
					}
				}
			}
		}
		if(break_flag == 1)
			break;
	}

	return (self->Status == PASS)?(1):(0);
}

int test_func_eeprom     (board_device * self)
{
	printf("START To Check the EEPROM....\n");
	printf("use the default i2c port \n");
	bcm2835_i2c_begin();
        bcm2835_i2c_setClockDivider( BCM2835_I2C_CLOCK_DIVIDER_2500);
        int i = 0;
	for(i = 0 ;i<255 ;i++ ){
		if(i2c_eeprom_write_byte(0,i,i) != BCM2835_I2C_REASON_OK){
			self->Status = FAIL;
			return 0;
		}
	}
	for( i = 0 ;i<255 ;i++ ){
		uint8_t _byte;
		if(i2c_eeprom_read_byte(0,i,&_byte) != BCM2835_I2C_REASON_OK){
			self->Status = FAIL;
			printf("========================================\n");
			printf("Check EEPROM Read Data Func : FAILED !!!\n");
			printf("========================================\n");
			
			return 0;
		}
		if(i != _byte){
			self->Status = FAIL;
			printf("=======================================\n");
			printf("Check EEPROM Read BAD Data : FAILED !!!\n");
                        printf("%d,%d \n",i,_byte);
			printf("=======================================\n");
			return 0;
		}
	}
	self->Status = PASS;
	printf("Check EEPROM: OK !!!\n");
	return 1;
}

// the self->arg should be a uint_8 * arg 
// arg[0] = manufactory_id
// arg[1] = device_id
int test_func_spi_flash   (board_device * self)
{
	printf("START To Check the SPI %s Flash RAM....\n",self->name );
	spi_init();
	uint8_t * id_list = (uint8_t *) self->arg;
	uint8_t man_id = id_list[0];
	uint8_t dev_id = id_list[1];

	// the manufactory id should be 0xc2
	//if(0xc2 != spi_read_manufactory_id()){
	if(man_id != spi_read_manufactory_id()){
		printf("===========================================\n");
		printf("CHECK SPI Flash RAM %s Manufactory ID Failed!!\n", self->name );
		printf("===========================================\n");
		self->Status = FAIL;
		return 0;
	}
	usleep(100000);
	//if(0x12 != spi_read_device_id()){
	if(dev_id != spi_read_device_id()){
		printf("=========================================\n");
		printf("CHECK SPI Flash RAM %s Deivce ID Failed!!\n", self->name );
		printf("=========================================\n");
		self->Status = FAIL;
		return 0;
	}
	usleep(100000);
	printf("CHECK SPI Flash RAM %s PASSED!!!!!\n", self->name );
	self->Status = PASS;
	return 1;
}

int test_func_lcd        (board_device * self)
{
	char _buff[30]; int i;
	printf("PLEASE Mount the LCD Module onto Board.\n");
	printf("Are you Ready?[Y/n]\n");
	for(i=0 ;i<30;i++ ) _buff[i] = '\0'; fgets(_buff, 30, stdin);
	if((_buff[0] == '\n') || (_buff[0] == 'y') || (_buff[0] == 'Y')){
		printf("START to TEST LCD MODULE!\n");
		lcd_init();
		lcd_putStr(0,0,"-=ITTraining=-");
		lcd_putStr(1,0,"LCM-TEST OK!Orz.");

		char buf[15];
		int i; 
		lcd_pos(0,0);
		for(i= 0 ;i<14 ;i++ ) buf[i] = lcd_read_data();
		buf[14] = '\0';
		if( strcmp(buf,"-=ITTraining=-") == 0){
			printf("LCD Check PASSED!\n");
			self->Status = PASS;
			return 1;
		}else{
			printf("===========================\n");
			printf("LCD Check FAILED!\n");
			printf("===========================\n");
			self->Status = FAIL;
			return 0;
		}
	}else{
		printf("===========================\n");
		printf("LCD moduel TEST : FAILED!\n");
		printf("===========================\n");
		self->Status = FAIL;
		return 0;
	}
	return 0;
}

int test_func_uart       (board_device * self)
{
	char test_msg[] = "ITTraining. Board TEST";
	char mmsg[] = "ITTraining";
	char test_buf[30];
	int ii ;
	// check the /dev/ttyUSB exist ?
	printf("START to test PL2303...\n");
	printf("Please Connetc the USB port and the PL2303...\n");

	while(1){
		if(0 == access("/dev/ttyUSB0", F_OK)){
			printf("find the PL2303!\n");
			printf("start to test");
			break;
		}
		clean_buffer();
		if( 1 == nonblock_read_stdin(buff, BUFF_LEN) ){
			printf("===========================\n");
			printf("Skip this test!!\n");
			printf("PL2303 & UART TEST Failed.");
			printf("===========================\n");
			self->Status = FAIL;
			return ;
		}
		sleep(1);
		printf("Cannot to find the PL2303...\n");
		printf("Skip test by press [ENTER] key.\n");
	}

	// START to TEST the Connection between the PL2303 and ttyAMA0
	//
	struct serial_port usb_tty;
	struct serial_port ama_tty;
	strcpy(usb_tty.device,"/dev/ttyUSB0");
	strcpy(ama_tty.device,"/dev/ttyAMA0"/*/dev/serial0*/);
	if(-1 == open_serial_v2(&usb_tty)){
		printf("===========================\n");
		printf("PL2303 Open Faild!\n");
		printf("===========================\n");
		self->Status = FAIL;
		return 0;
	}
	printf("PL2303 Open Success!\n");

	if(-1 == open_serial_v2(&ama_tty)){
		printf("===========================\n");
		printf("ttyAMA0 Open Faild!\n");
		printf("===========================\n");
		self->Status = FAIL;
		close_serial(&usb_tty);
		return 0;
	}
	printf("ttyAMA0 Open Success!\n");

	nonblock_read_tty(&ama_tty,test_buf,30);
	nonblock_read_tty(&usb_tty,test_buf,30);
	for(ii = 0 ;ii<30 ;ii++ ) test_buf[ii] = '\0';

	// TEST ttyAMA0 Send Message to PL2303
	printf("ttyAMA0 send: %s\n",test_msg);
	write(ama_tty.fd, test_msg,strlen(test_msg) );
	usleep(500000);

	read(usb_tty.fd, test_buf,30);
	printf("PL2303 Received: %s\n", test_buf);
	//if((rres = strcmp(test_msg,test_buf)) < 0){
	if(strstr(test_buf,mmsg) == NULL){
		//printf("res of strcmp:%d\n", rres);
		printf("===========================\n");
		printf("ttyAMA0 Send message to PL2303 Failed!\n");
		printf("===========================\n");
		self->Status = FAIL;
		close_serial(&usb_tty);
		close_serial(&ama_tty);
		return 0;
	} 

	printf("========================================\n");
	printf("ttyAMA0 Send message to PL2303 Succeed!!\n");
	printf("========================================\n");

	nonblock_read_tty(&usb_tty,test_buf,30);
	nonblock_read_tty(&ama_tty,test_buf,30);
	for(ii = 0 ;ii<30 ;ii++ ) test_buf[ii] = '\0';

	// TEST ttyAMA0 Send Message to PL2303
	printf("PL2303 send: %s\n",test_msg);
	write(usb_tty.fd, test_msg,strlen(test_msg) );
	usleep(/*200000*/100000);
	read(ama_tty.fd, test_buf,30);
	printf("ttyAMA0 Received: %s\n", test_buf);
	//if((rres = strcmp(test_msg,test_buf)) < 0){
	if(strstr(test_buf,mmsg) == NULL){
		//printf("res of strcmp:%d\n", rres);
		printf("===========================\n");
		printf("PL2303 Send message to ttyAMA0 Failed!\n");
		printf("===========================\n");
		self->Status = FAIL;
		close_serial(&usb_tty);
		close_serial(&ama_tty);
		return 0;
	}
	printf("========================================\n");
	printf("PL2303 Send message to ttyAMA0 Succeed!!\n");
	printf("========================================\n");

	self->Status = PASS;

	close_serial(&usb_tty);
	close_serial(&ama_tty);
	return 1;
}
int test_func_mcp3008  (board_device * self)
{
	char _buff[30];
     bcm2835_spi_begin();
	// spi init 


	printf("\n=======================================\n");
	printf("The channel_0 of %s is given to High for pass this check!\n", self->name );
	printf("Or press any key of keyboard to failed this check\n");
	printf("=======================================\n");

	while(1){
		// check the keyboard button press 
		int res = nonblock_read_stdin(_buff, 30);
		if(res == -1){
			printf("ERROR : select\n");
			return -2;
		}else if (res > 0){
			self->Status = FAIL;
			break;
		}

		// check the button 1 press
		if(MCP3008_analog_read(0,0) == 1023) 
		{
			printf("The channel_0 of %s Succeed!!\n", self->name);
			self->Status = PASS;
			break;
		}
		else
		{
			printf("The channel_0 of MCP3008 is %d ,not 1024 \n", MCP3008_analog_read( 0,0));
			usleep(500000);
		}
		
	}
	return (self->Status == PASS)?(1):(0);
}
int test_func_DIP     (board_device * self)
{
	char _buff[30];

	// setup the pin out setup 
	int DIP_pin = self->phy_pin_list[0];
	SET_INPUT(DIP_pin);

	printf("\n=======================================\n");
	printf("Switch %s  for pass this check!\n", self->name );
	printf("Or press any key of keyboard to failed this check\n");
	printf("=======================================\n");
    int state=bcm2835_gpio_lev(DIP_pin);
	while(1){
		// check the keyboard button press 
		int res = nonblock_read_stdin(_buff, 30);
		if(res == -1){
			printf("ERROR : select\n");
			return -2;
		}else if (res > 0){
			self->Status = FAIL;
			break;
		}
		// check the switch chang
		if(bcm2835_gpio_lev(DIP_pin)==!state)
		{   
	        printf("%s Succeed!!\n", self->name);
			self->Status = PASS;
			break;
		}	
		
	}
	return (self->Status == PASS)?(1):(0);
	
	
}
int test_func_pwm        (board_device * self)
{
	char _buff[30];
	int i = 0;
	int channel ;
	if(self->phy_pin_list[0] ==18)
		channel=0;
	else
		channel=1;
	// init the pin setup
	int pwm_pin = self->phy_pin_list[0];
	bcm2835_gpio_fsel(pwm_pin,BCM2835_GPIO_FSEL_ALT5);
	bcm2835_pwm_set_clock(1920);   //set min frequence =19.2M/1920
    bcm2835_pwm_set_mode(channel,1,1,0);   //(channel,PWM0 MSEN enable , PWM0 enable , revpolar)
    bcm2835_pwm_set_range(channel,200);  //set period=20ms
    bcm2835_pwm_set_data(channel,5);

	printf(" %s transmit?[Y/n]:", self->name );

	for(i = 0 ;i<30 ;i++ ) _buff[i] = '\0';
	fgets(_buff, 30, stdin);

	if((_buff[0] == '\n') || (_buff[0] == 'y') || (_buff[0] == 'Y'))
		self->Status = PASS;

	else if((_buff[0] == 'n') || (_buff[0] == 'N'))
		self->Status = FAIL;
	 bcm2835_gpio_write(pwm_pin, BCM2835_GPIO_FSEL_ALT2);
     bcm2835_gpio_set_pud(pwm_pin,LOW);
	 
	
	return (self->Status == PASS)?(1):(0);
}
int test_func_lis3dh     (board_device * self)
{
	char _buff[30];
	 int8_t address=0;
     i2c_start();
	// lis3dh init 
    lis3dhData_t data;
	data.x=0;
	data.y=0;
	data.z=0;
    lis3dhInit();
	printf("\n=======================================\n");
	printf("Let %s one of 3 axis be negative \n", self->name );
	printf("Or press any key of keyboard to failed this check\n");
	printf("=======================================\n");
    if (i2c_read_data_block(LIS3DH_ADDRESS, LIS3DH_REGISTER_WHO_AM_I, &address, 1)!=BCM2835_I2C_REASON_OK)
	{printf("address fail\n");}
    else
	printf("address =%d\n",address);
	while(1){
		// check the keyboard button press 
		int res = nonblock_read_stdin(_buff, 30);
		
		if(res == -1){
			printf("ERROR : select\n");
			return -2;
		}else if (res > 0){
			self->Status = FAIL;
			break;
		}

		   if (lis3dhPoll(&data)!=BCM2835_I2C_REASON_OK)
		   {
			   printf("ERROR lis3dhPoll\n");
			}
		
            printf("x= %d  \n", data.x);
			printf("y= %d  \n", data.y);
			printf("z= %d  \n", data.z);
		if(data.x <0)
		{
			self->Status = PASS;
			break;
		}
		else if(data.y <0)
		{
			self->Status = PASS;
			break;
		}
		else if(data.z <0)
		{
			self->Status = PASS;
			break;
		}
			usleep(300000);
		
		
	}
	
	return (self->Status == PASS)?(1):(0);
}


