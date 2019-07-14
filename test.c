#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <util/twi.h>
#include "usart.h"
#include "i2c.h"
#include "mpr121.h"

#define MPR121_ADD 0x5A
#define SLA_W MPR121_ADD + 0x00
#define SLA_R MPR121_ADD + 0x01

int main(void){

	clock_rescale_set(clock_div_1);
	init_usart();
	print_String("\r\n --- MPR121 test --\r\n");
	i2c_init();
	mpr121_init();

	while(1){
		...
	}
	return 0;
}

