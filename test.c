#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "i2c.h"
#include "mpr121.h"

ISR(INT0_vect){
	cli();
	mpr121_nirq();
	sei();
}

void init_interrupt(void){
	EIMSK |= (1<<INT0);
	EICRA |= ((0 << ISC00) | (0 << ISC00));
//	sei();
}


int main(void){

	DDRB = 0x01;
	PORTB = 0x00;
	clock_prescale_set(clock_div_1);

	init_usart();
	i2c_init();
	init_interrupt();	
	flush();
	
	if (mpr121_init()){
//		print_string("\r\nMPR121: init() OK\r\n");
		while(1){
			_delay_ms(125);
			send_info(0);


			PORTB ^= (1 << PB0);
			_delay_ms(250);
		}
	}
	return 0;
}

