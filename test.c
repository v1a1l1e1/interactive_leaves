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
	
	uint8_t data0, data1;
	while(1){
		i2c_start();
		i2c_tx_data(SLA_W);
		//controllo ack
		i2c_tx_data(TOUCH_STATUS0);
		//controllo ack
		i2c_start();
		i2c_tx_data(SLA_R);
		//controllo ack
		data0 = i2c_rx_data(1);
		i2c_stop();
		print_byte(data0);
		print_byte(data1);
		_delay_ms(500);
	}
	return 0;
}

