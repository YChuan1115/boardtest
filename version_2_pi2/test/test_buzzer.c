#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"board.h"





int main(int argc, char *argv[]){

	//
	// initialization 
	//
	char ch;

	bcm2835_init();
	board_init();

	SET_OUTPUT(PIN_BUZZER);

	printf("h : output high \n");
	printf("l : output low \n");
	printf("u : input pull up\n");
	printf("d : input pull down\n");
	printf("o : input off \n");
	printf("z : as input off\n");
	while(1){
		ch = getchar();
		if(ch == 'h'){
			SET_OUTPUT(PIN_BUZZER);
			bcm2835_gpio_write(PIN_BUZZER, HIGH);
		}
		if(ch == 'l'){
			SET_OUTPUT(PIN_BUZZER);
			bcm2835_gpio_write(PIN_BUZZER, LOW);
		}
		if(ch == 'u'){
			SET_INPUT(PIN_BUZZER);
			SET_PULLUP(PIN_BUZZER);
		}
		if(ch == 'd'){
			SET_INPUT(PIN_BUZZER);
			SET_PULLDOWN(PIN_BUZZER);
		}
		if(ch == 'o'){
			SET_INPUT(PIN_BUZZER);
			SET_PULLOFF(PIN_BUZZER);
		}
		if(ch == 'z'){
			break;
		}
		
	}


	return 0;
}
