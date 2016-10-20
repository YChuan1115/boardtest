#include<board.h>
#include <lib_uart.h>

char buff[BUFF_LEN];
int len = BUFF_LEN;
uint8_t pass_sum = 0;
uint8_t unpass_sum = 0;

int board_init()
{
	// setup the leds 
	SET_OUTPUT(PIN_LED_01);
	SET_OUTPUT(PIN_LED_02);

	bcm2835_gpio_write(PIN_LED_01,LOW);
	bcm2835_gpio_write(PIN_LED_02,LOW);
    
	// setup the buttons 
	SET_INPUT(PIN_BTN_01);
	SET_INPUT(PIN_BTN_02);
	SET_PULLUP(PIN_BTN_01);
	SET_PULLUP(PIN_BTN_02);
    
}


void test_led(checkList * chk_list){

	// test led 01
	bcm2835_gpio_write(PIN_LED_01,HIGH);
	printf("LED 01 Light?[Y/n]:");
	clean_buffer(); fgets(buff, BUFF_LEN, stdin);

	if((buff[0] == '\n') || (buff[0] == 'y') || (buff[0] == 'Y')){
		printf("LED 01 : PASS !\n");
		chk_list->led_1 = 1;
		pass_sum += 1;
	}
	else if((buff[0] == 'n') || (buff[0] == 'N')){
		printf("=======================\nLED 01 : FAILED!\n");
		printf("===========================\n");
		chk_list->led_1 = -1;
		unpass_sum += 1;
	}

	bcm2835_gpio_write(PIN_LED_01,LOW);



	// test led 02
	bcm2835_gpio_write(PIN_LED_02,HIGH);
	printf("LED 02 Light?[Y/n]:");
	clean_buffer(); fgets(buff, BUFF_LEN, stdin);

	if((buff[0] == '\n') || (buff[0] == 'y') || (buff[0] == 'Y')){
		printf("LED 02 : PASS !\n");
		chk_list->led_2 = 1;
		pass_sum += 1;

	}
	else if((buff[0] == 'n') || (buff[0] == 'N')){
		printf("=======================\nLED 02 : FAILED!\n");
		printf("===========================\n");
		chk_list->led_2 = -1;
		unpass_sum += 1;
	}

	bcm2835_gpio_write(PIN_LED_02,LOW);
}

void test_button_1(checkList * chk_list)
{
	int keyboard,res;
	int count = 0;
	struct timeval timeout;
	fd_set read_fd;

	printf("\n=======================================\n");
	printf("Press Button 1 twice for pass this check!\n");
	printf("Or press any key of keyboard to failed this check\n");
	printf("=======================================\n");
	keyboard = open("/dev/tty", O_RDONLY | O_NONBLOCK);
	while(1){
		// check the keyboard button press 
		timeout.tv_sec = 0; timeout.tv_usec = 30000;
		FD_ZERO(&read_fd);  FD_SET(keyboard, &read_fd);
		res = select(keyboard + 1, &read_fd, NULL, NULL, &timeout );
		if(res == -1){
			printf("ERROR : select\n");
			close(keyboard);
			exit(1);
		}else if (res > 0){
			if(FD_ISSET(keyboard, &read_fd)){
				clean_buffer(); fgets(buff, BUFF_LEN, stdin);
				printf("=========================\n");
				printf("BUTTON 1 TEST: FAILED!\n");
				printf("===========================\n");
				chk_list->button_1 = -1;
				unpass_sum += 1;
				break;
			}
		}

		// check the button 1 press
		if(check_button_1() == BTN_UP_EDGE) count ++;

		// check the button 1 press twice !
		if(count == 2){
			printf("BUTTON 1 TEST: PASSED!\n");
			chk_list->button_1 = 1;
			pass_sum += 1;
			break;
		}
	}
	close(keyboard);
}

void test_button_2(checkList * chk_list)
{
	int keyboard,res;
	int count = 0;
	struct timeval timeout;
	fd_set read_fd;

	printf("\n=======================================\n");
	printf("Press Button 2 twice for pass this check!\n");
	printf("Or press any key of keyboard to failed this check\n");
	printf("=======================================\n");
	keyboard = open("/dev/tty", O_RDONLY | O_NONBLOCK);
	while(1){
		// check the keyboard button press 
		timeout.tv_sec = 0; timeout.tv_usec = 30000;
		FD_ZERO(&read_fd);  FD_SET(keyboard, &read_fd);
		res = select(keyboard + 1, &read_fd, NULL, NULL, &timeout );
		if(res == -1){
			printf("ERROR : select\n");
			close(keyboard);
			exit(1);
		}else if (res > 0){
			if(FD_ISSET(keyboard, &read_fd)){
				clean_buffer(); fgets(buff, BUFF_LEN, stdin);
				printf("=========================\n");
				printf("BUTTON 2 TEST: FAILED!\n");
				printf("===========================\n");
				chk_list->button_2 = -1;
				unpass_sum += 1;
				break;
			}
		}

		// check the button 2 press
		if(check_button_2() == BTN_UP_EDGE) count ++;

		// check the button 2 press twice !
		if(count == 2){
			printf("BUTTON 2 TEST: PASSED!\n");
			chk_list->button_2 = 1;
			pass_sum += 1;
			break;
		}
	}
	close(keyboard);
}

int check_button_1(){
	static pre_status = 1;
	int val = DIGIT_READ(PIN_BTN_01);
	if(val == pre_status){
		if(val == 1) return BTN_RELEASE;
		if(val == 0) return BTN_PRESSED;
	}
	else {
		if(val == 1) {
			pre_status = val;
			return BTN_UP_EDGE;
		}
		if(val == 0){
			pre_status = val;
			return BTN_DOWN_EDGE;
		}
	}
}

int check_button_2(){
	static pre_status = 1;
	int val = DIGIT_READ(PIN_BTN_02);
	if(val == pre_status){
		if(val == 1) return BTN_RELEASE;
		if(val == 0) return BTN_PRESSED;
	}
	else {
		if(val == 1) {
			pre_status = val;
			return BTN_UP_EDGE;
		}
		if(val == 0){
			pre_status = val;
			return BTN_DOWN_EDGE;
		}
	}
}


void show_check_list(checkList * list)
{
	printf("========================================\n");
	printf("               check List               \n");
	printf("========================================\n");
	printf("LED D2     | pin 13  | "); printCheckStatus(list->led_1); printf("\n");
	printf("----------------------------------------\n");
	printf("LED D3     | pin 15  | "); printCheckStatus(list->led_2); printf("\n");
	printf("----------------------------------------\n");
	printf("Button SW1 | pin 18  | "); printCheckStatus(list->button_1); printf("\n");
	printf("----------------------------------------\n");
	printf("Button SW2 | pin 16  | "); printCheckStatus(list->button_2); printf("\n");
	printf("----------------------------------------\n");
	printf("Buzzer     | pin 32  | "); printCheckStatus(list->buzzer); printf("\n");
	printf("----------------------------------------\n");
	printf("Relay      | pin 11  | "); printCheckStatus(list->relay); printf("\n");
	printf("----------------------------------------\n");
	printf("PL2303     |         | "); printCheckStatus(list->uart); printf("\n");
	printf("----------------------------------------\n");
	printf("IR Receiver| pin 22  | "); printCheckStatus(list->ir); printf("\n");
	printf("----------------------------------------\n");
	printf("SPI FLASH  |         | "); printCheckStatus(list->spi); printf("\n");
	printf("----------------------------------------\n");
	printf("I2C EEPROM |         | "); printCheckStatus(list->eeprom); printf("\n");
	printf("----------------------------------------\n");
	printf("LCD Module |         | "); printCheckStatus(list->lcd); printf("\n");
	printf("----------------------------------------\n");
}

void printCheckStatus(int i)
{
	switch(i){
		case -1:
			printf(" NOT PASSED! ");
			break;
		case 0 :
			printf(" NOT TEST Yet? ");
			break;
		case 1 :
			printf(" PASSED It's OK! ");
			break;
	}
}

void buzzer_on()
{
	usleep(10);
	SET_OUTPUT(PIN_BUZZER);
	usleep(10);
	bcm2835_gpio_write(PIN_BUZZER,HIGH);
}

void buzzer_off()
{
	SET_INPUT(PIN_BUZZER);
}

void test_buzzer(checkList * chk_list)
{
	char ch;
	printf("Buzzer TEST: Do you hear the louder noize? [Y/n]:");

	buzzer_on(); usleep(1000);
	buzzer_off(); usleep(1000);
	buzzer_on();

	clean_buffer(); fgets(buff, BUFF_LEN, stdin);

	if((buff[0] == '\n') || (buff[0] == 'y') || (buff[0] == 'Y')){
		printf("BUZZER : PASS !\n");
		chk_list->buzzer= 1;
		pass_sum += 1;
	}
	else if((buff[0] == 'n') || (buff[0] == 'N')){
		printf("=======================\nBUZZER : FAILED!\n");
		printf("===========================\n");
		chk_list->buzzer= -1;
		unpass_sum += 1;
	}

	buzzer_off();
}

void test_relay(checkList * chk_list)
{
	struct timeval timeout;
	fd_set read_fd;
	int keyboard,res;
	keyboard = open("/dev/tty", O_RDONLY | O_NONBLOCK);

	printf("TEST: Relay on and off\n");
	printf("Do you hear the sound of switching relay? [Y/n]:\n");
	SET_OUTPUT(PIN_RELAY);
	bcm2835_gpio_write(PIN_RELAY, LOW);

	while(1){
		// check the keyboard button press 
		timeout.tv_sec = 0; timeout.tv_usec = 30000;
		FD_ZERO(&read_fd);  
		FD_SET(keyboard, &read_fd);
		res = select(keyboard + 1, &read_fd, NULL, NULL, &timeout );
		if(res == -1){
			printf("ERROR : select\n");
			close(keyboard);
			exit(1);
		}else if (res > 0){
			if(FD_ISSET(keyboard, &read_fd)){
				clean_buffer(); fgets(buff, BUFF_LEN, stdin);
				if((buff[0] == '\n') || (buff[0] == 'y') || (buff[0] == 'Y')){
					printf("Relay TEST: OK!\n");
					chk_list->relay = 1;
					pass_sum += 1;
				}else{
					printf("===========================\n");
					printf("Relay TEST: FAILED!\n");
					printf("===========================\n");
					chk_list->relay = -1;
					unpass_sum += 1;
				}
				SET_INPUT(PIN_RELAY);
				close(keyboard);
				break;
			}
		}
		// END check the keyboard input

		// relay on 
		bcm2835_gpio_write(PIN_RELAY, HIGH);
		usleep(500000);
		// realy off
		bcm2835_gpio_write(PIN_RELAY, LOW);
		usleep(500000);
	}
}

void test_ir(checkList * chk_list)
{
	struct timeval timeout;
	fd_set read_fd;
	int keyboard,res;
	int break_flag = 0;
	keyboard = open("/dev/tty", O_RDONLY | O_NONBLOCK);

	SET_INPUT(PIN_IR_RX);
	printf("START to TEST IR module.\n");
	printf("Please Send Some IR Signal to Pi board.\n");
	printf("\npress any key to skip.\n");
	while(1){
		if( check_ir() == BTN_DOWN_EDGE ){
			struct timeval pre;
			struct timeval now;
			gettimeofday(&pre,NULL);
			while(1){
				if(check_ir() == BTN_DOWN_EDGE){
					gettimeofday(&now,NULL);
					long delta_time = 1000000*( now.tv_sec - pre.tv_sec)+(now.tv_usec - pre.tv_usec);
					pre.tv_sec = now.tv_sec; pre.tv_usec = now.tv_usec;
					printf("dt:%6ld\n", delta_time);
				}else{
					gettimeofday(&now,NULL);
					long delta_time = 1000000*( now.tv_sec - pre.tv_sec)+(now.tv_usec - pre.tv_usec);
					if(delta_time > 300000){
						//////////////////////////////////////
						printf("IR test ok !\n");
						chk_list->ir = 1;
						pass_sum += 1;
						break_flag = 1;
						break;
					}
				}
			}
		}
		if(break_flag == 1) {
			break;
		}

		// check the keyboard input
		timeout.tv_sec = 0; timeout.tv_usec = 30000;
		FD_ZERO(&read_fd);  FD_SET(keyboard, &read_fd);
		res = select(keyboard + 1, &read_fd, NULL, NULL, &timeout );
		if(res == -1){
			printf("ERROR : select\n");
			exit(1);
		}else if (res > 0){
			if(FD_ISSET(keyboard, &read_fd)){
				clean_buffer(); fgets(buff, BUFF_LEN, stdin);
				printf("===========================\n");
				printf("IR Module test Failed:!\n");
				printf("===========================\n");
				chk_list->ir = -1;
				unpass_sum += 1;
				break;
			}
		}
	}
	close(keyboard);
}

void test_eeprom(checkList * chk_list){
	printf("START To Check the EEPROM....");
	int i = 0;
	for(i = 0 ;i<255 ;i++ ){
		if(i2c_eeprom_write_byte(0,i,i) != BCM2835_I2C_REASON_OK){
			chk_list->eeprom = -1;
			unpass_sum += 1;
			printf("===========================\n");
			printf("Check EEPROM : FAILED !!!!!\n");
			printf("===========================\n");
			return ;
		}
	}
	for( i = 0 ;i<255 ;i++ ){
		uint8_t _byte;
		if(i2c_eeprom_read_byte(0,i,&_byte) != BCM2835_I2C_REASON_OK){
			chk_list->eeprom = -1;
			unpass_sum += 1;
			printf("===========================\n");
			printf("Check EEPROM : FAILED !!!\n");
			printf("===========================\n");
			return ;
		}
		if(i != _byte){
			chk_list->eeprom = -1;
			unpass_sum += 1;
			printf("===========================\n");
			printf("Check EEPROM : FAILED !!!\n");
			printf("===========================\n");
			return ;
		}
	}
	chk_list->eeprom = 1;
	pass_sum += 1;
	printf("Check EEPROM: OK !!!\n");
}

void test_spi(checkList * chk_list)
{
	printf("START To Check the SPI Flash RAM....\n");
	spi_init();
	if(0xc2 != spi_read_manufactory_id()){
		printf("===========================\n");
		printf("CHECK SPI Flash RAM Failed!!!!\n");
		printf("===========================\n");
		chk_list->spi = -1;
		unpass_sum += 1;
		return ;
	}
	usleep(100000);
	if(0x12 != spi_read_device_id()){
		printf("===========================\n");
		printf("CHECK SPI Flash RAM Failed!!!!\n");
		printf("===========================\n");
		chk_list->spi = -1;
		unpass_sum += 1;
		return ;
	}
	usleep(100000);
	printf("CHECK SPI Flash RAM PASSED!!!!!\n");
	chk_list->spi = 1;
	pass_sum += 1;
}

void test_lcd(checkList * chk_list)
{
	printf("PLEASE Mount the LCD Module onto Board.\n");
	printf("Are you Ready?[Y/n]\n");
	clean_buffer(); fgets(buff, BUFF_LEN, stdin);
	if((buff[0] == '\n') || (buff[0] == 'y') || (buff[0] == 'Y')){
		printf("START to TEST LCD MODULE!\n");
		lcd_init();
		lcd_putStr(0,0,"-=ITTraining=-");
		lcd_putStr(1,0,"LCM-TEST OK!Orz.");
		printf("Does the LCD Work Correctly?[Y/n]\n");
		clean_buffer(); fgets(buff, BUFF_LEN, stdin);
		if((buff[0] == '\n') || (buff[0] == 'y') || (buff[0] == 'Y')){
			printf("LCD moude TEST : PASSED!!\n");
			chk_list->lcd = 1;
			pass_sum += 1;
			return ;
		}
		else{
			chk_list->lcd = -1;
			unpass_sum += 1;
			printf("===========================\n");
			printf("LCD moduel TEST : FAILED!\n");
			printf("===========================\n");
			return ;
		}
	}else{
		chk_list->lcd = -1;
		unpass_sum += 1;
		printf("===========================\n");
		printf("LCD moduel TEST : FAILED!\n");
		printf("===========================\n");
		return ;
	}

}

void test_lcd_auto(checkList * chk_list)
{
	printf("PLEASE Mount the LCD Module onto Board.\n");
	printf("Are you Ready?[Y/n]\n");
	clean_buffer(); fgets(buff, BUFF_LEN, stdin);
	if((buff[0] == '\n') || (buff[0] == 'y') || (buff[0] == 'Y')){
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
			chk_list->lcd = 1;
			pass_sum += 1;
			return ;
		}else{
			printf("===========================\n");
			printf("LCD Check NOT FAILED!\n");
			printf("===========================\n");
			chk_list->lcd = -1;
			unpass_sum += 1;
			return ;
		}
	}else{
		chk_list->lcd = -1;
		unpass_sum += 1;
		printf("===========================\n");
		printf("LCD moduel TEST : FAILED!\n");
		printf("===========================\n");
		return ;
	}
}

void test_uart_auto(checkList * chk_list)
{
	char test_msg[] = "ITTraining. Board TEST";
	char test_buf[30];
	int ii ;
	// check the /dev/ttyUSB exist ?
	printf("START to test PL2303...\n");
	printf("Please Connetc the USB port and the PL2303...\n");
	printf("Skip test by press [ENTER] key.\n");
	while(1){
		sleep(1);
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
			chk_list->uart = -1;
			unpass_sum += 1;
			return ;
		}
	}

	// START to TEST the Connection between the PL2303 and ttyAMA0
	//
	struct serial_port usb_tty;
	struct serial_port ama_tty;
	strcpy(usb_tty.device,"/dev/ttyUSB0");
	strcpy(ama_tty.device,"/dev/ttyAMA0");
	if(-1 == open_serial(&usb_tty)){
		printf("===========================\n");
		printf("PL2303 Open Faild!\n");
		printf("===========================\n");
		chk_list->uart = -1;
		unpass_sum += 1;
		return ;
	}
	printf("PL2303 Open Success!\n");

	if(-1 == open_serial(&ama_tty)){
		printf("===========================\n");
		printf("ttyAMA0 Open Faild!\n");
		printf("===========================\n");
		chk_list->uart = -1;
		unpass_sum += 1;
		close_serial(&usb_tty);
		return ;
	}
	printf("ttyAMA0 Open Success!\n");

	nonblock_read_tty(&ama_tty,test_buf,30);
	for(ii = 0 ;ii<30 ;ii++ ) test_buf[ii] = '\0';

	// TEST ttyAMA0 Send Message to PL2303
	printf("PL2303 send: %s\n",test_msg);
	write(usb_tty.fd, test_msg,strlen(test_msg) );
	usleep(500000);
	read(ama_tty.fd, test_buf,30);
	printf("ttyAMA0 Received: %s\n", test_buf);
	if(0 != strcmp(test_msg,test_buf)){
		printf("===========================\n");
		printf("PL2303 Send message to ttyAMA0 Failed!\n");
		printf("===========================\n");
		chk_list->uart = -1;
		unpass_sum += 1;
		close_serial(&usb_tty);
		close_serial(&ama_tty);
		return ;
	}
	printf("PL2303 Send message to ttyAMA0 Succeed!!\n");

	nonblock_read_tty(&usb_tty,test_buf,30);
	for(ii = 0 ;ii<30 ;ii++ ) test_buf[ii] = '\0';

	// TEST ttyAMA0 Send Message to PL2303
	printf("ttyAMA0 send: %s\n",test_msg);
	write(ama_tty.fd, test_msg,strlen(test_msg) );
	usleep(500000);
	read(usb_tty.fd, test_buf,30);
	printf("PL2303 Received: %s\n", test_buf);
	if(0 != strcmp(test_msg,test_buf)){
		printf("===========================\n");
		printf("ttyAMA0 Send message to PL2303 Failed!\n");
		printf("===========================\n");
		chk_list->uart = -1;
		unpass_sum += 1;
		close_serial(&usb_tty);
		close_serial(&ama_tty);
		return ;
	}

	printf("ttyAMA0 Send message to PL2303 Succeed!!\n");
	chk_list->uart = 1;
	pass_sum += 1;

	close_serial(&usb_tty);
	close_serial(&ama_tty);
}

void clean_buffer(){
	len = BUFF_LEN;
	while(len--)buff[len]='\0';
	len = 0;
}

int check_ir(){
	static pre_status = 1;
	SET_INPUT(PIN_IR_RX);
	int val = DIGIT_READ(PIN_IR_RX);
	if(val == pre_status){
		if(val == 1) return BTN_RELEASE;
		if(val == 0) return BTN_PRESSED;
	}
	else {
		if(val == 1) {
			pre_status = val;
			return BTN_UP_EDGE;
		}
		if(val == 0){
			pre_status = val;
			return BTN_DOWN_EDGE;
		}
	}
}

int check_button(int pin)
{
	static pre_status = 1;
	int val = DIGIT_READ(pin);
	if(val == pre_status){
		if(val == 1) return BTN_RELEASE;
		if(val == 0) return BTN_PRESSED;
	}
	else {
		if(val == 1) {
			pre_status = val;
			return BTN_UP_EDGE;
		}
		if(val == 0){
			pre_status = val;
			return BTN_DOWN_EDGE;
		}
	}
}

int check_ir_pin(int ir_pin){
	static pre_status = 1;
	SET_INPUT(ir_pin );
	int val = DIGIT_READ(ir_pin );
	if(val == pre_status){
		if(val == 1) return BTN_RELEASE;
		if(val == 0) return BTN_PRESSED;
	}
	else {
		if(val == 1) {
			pre_status = val;
			return BTN_UP_EDGE;
		}
		if(val == 0){
			pre_status = val;
			return BTN_DOWN_EDGE;
		}
	}
}
