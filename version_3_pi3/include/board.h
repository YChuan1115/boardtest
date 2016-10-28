#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/select.h>
#include<sys/time.h>
#include<fcntl.h>
#include<bcm2835.h>

#ifndef __BOARD_H
#define __BOARD_H

#ifdef ver2
#define PIN_LED_01    RPI_BPLUS_GPIO_J8_13
#define PIN_LED_02    RPI_BPLUS_GPIO_J8_15

#define PIN_BTN_01    RPI_BPLUS_GPIO_J8_18
#define PIN_BTN_02    RPI_BPLUS_GPIO_J8_16
#define PIN_BUZZER    RPI_BPLUS_GPIO_J8_32
#define PIN_RELAY     RPI_BPLUS_GPIO_J8_11
#define PIN_IR_REC    RPI_BPLUS_GPIO_J8_22
#else
#define PIN_LED_01    5
#define PIN_LED_02    6
#define PIN_LED_03    13
#define PIN_LED_04    26
#define PIN_LED_05    12
#define PIN_BTN_01    24
#define PIN_BTN_02    23
#define PIN_BUZZER    16
#define PIN_RELAY     27
#define PIN_DIP1      20
#define PIN_DIP2      21
#define PIN_IR_TX     25
#define PIN_IR_RX     17
#define PIN_COM       22
#define PIN_PWM0      18
#define PIN_PWM1      19	
#endif

#define SET_OUTPUT(x) bcm2835_gpio_fsel( x , BCM2835_GPIO_FSEL_OUTP)
#define SET_INPUT(x)  bcm2835_gpio_fsel( x , BCM2835_GPIO_FSEL_INPT)
#define SET_PULLUP(x) bcm2835_gpio_set_pud( x ,BCM2835_GPIO_PUD_UP)
#define SET_PULLDOWN(x) bcm2835_gpio_set_pud( x ,BCM2835_GPIO_PUD_DOWN)
#define SET_PULLOFF(x) bcm2835_gpio_set_pud( x ,BCM2835_GPIO_PUD_OFF)
#define DIGIT_READ(x) bcm2835_gpio_lev(x)
#define DIGIT_WRITE(x,y) bcm2835_gpio_write(x,y);

#define BTN_RELEASE   1
#define BTN_DOWN_EDGE 2
#define BTN_PRESSED   3
#define BTN_UP_EDGE   4
#define BUFF_LEN 20

typedef struct {
	int led_1;
	int led_2;
	int button_1;
	int button_2;
	int buzzer;
	int relay;
	int ir;
	int eeprom;
	int spi;
	int lcd;
	int uart;
}checkList;

enum status {PASS=101,FAIL,IGNORE,UNDEFINED};

typedef struct _board_device{
	struct _board_device * self;
	int    id;
	char   name[32];
	int    phy_pin_num;
	int    phy_pin_list[26];
	int    flag_ignore;
	int  (*test_func)(struct _board_device * self);
	void  *arg;
	enum status Status;
	/*. add here...*/
} board_device;

int board_init();





/// old version test function

void test_led(         checkList * chk_list);
void test_button_1(    checkList * chk_list);
void test_button_2(    checkList * chk_list);
void test_buzzer(      checkList * chk_list);
void test_relay(       checkList * chk_list);
void test_ir(          checkList * chk_list);
void test_eeprom(      checkList * chk_list);
void test_spi(         checkList * chk_list);
void test_lcd(         checkList * chk_list);
void test_lcd_auto(    checkList * chk_list);
void test_uart_auto(   checkList * chk_list);
void show_check_list(  checkList * list);

void printCheckStatus(int i);
int check_button_1();
int check_button_2();
void clean_buffer();
void buzzer_on();
void buzzer_off();
int check_button(int pin);
int check_ir_pin(int ir_pin);

#endif 
