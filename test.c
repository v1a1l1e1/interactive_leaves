#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "i2c.h"
#include "mpr121.h"

volatile uint16_t stato;

ISR(INT0_vect){
stato = mpr121_nirq();
send_byte((uint8_t)stato & 0x00ff);
send_byte((uint8_t)stato & 0xff00);
PORTB |= (1 << PB0);
_delay_ms(500);
PORTB &= ~(1 << PBO);
}

void init_interrupt(void){
	EIMSK |= (1<<INT0);
	EICRA &= ~((1 << ISC00) | (1 << ISC00))
}

int main(void){
	uint16_t prev = 0;
	clock_rescale_set(clock_div_1);
	init_usart();
	print_String("\r\n --- MPR121 test --\r\n");
	i2c_init();
	if(! mpr121_init())
		print_String("\r\n -- mpr121 non rilevato\r\n");
	else{
		mpr121_reset();
		init_interrupt();

	while(1){
		stato = 0x0000;
		sei();
		if (stato != prev){
			prev = stato;
			PORTB |= (1 << PB4);
			_delay_ms(1);
			cli();
		} else {
			PORTB &= ~(1<<PB4);
			_delay_ms(1);
		}
	}
	return 0;
}

