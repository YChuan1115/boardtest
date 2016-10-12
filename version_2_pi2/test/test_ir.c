#include<stdio.h>
#include<stdlib.h>
#include"board.h"

int pre_stat = 1;

int main(int argc,char *argv[]){
	bcm2835_init();
	SET_INPUT(PIN_IR_REC);
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
					if(delta_time > 1000000)
						break;
				}
			}
		}
	}
	return 0;
}




