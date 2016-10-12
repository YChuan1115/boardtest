#ifndef __TEST_FUNC_H
#define __TEST_FUNC_H
#include<board.h>

// add your test function defination after ...
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


int test_func_led        (board_device * self);
int test_func_button     (board_device * self);
int test_func_buzzer     (board_device * self);
int test_func_realy      (board_device * self);
int test_func_ir         (board_device * self);
int test_func_eeprom     (board_device * self);
int test_func_spi_flash   (board_device * self);
int test_func_lcd        (board_device * self);
int test_func_uart       (board_device * self);



#endif 
