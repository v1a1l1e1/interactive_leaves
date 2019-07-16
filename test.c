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
cli();
print_string("T --> ");
print_word(stato);
print_string("\r\n");
PORTB |= (1 << PB0);
_delay_ms(500);
PORTB &= ~(1 << PB0);
sei();
}

void init_interrupt(void){
	EIMSK |= (1<<INT0);
	EICRA &= ~((1 << ISC00) | (1 << ISC00));
	sei();
}

int main(void){
	DDRB = 0xff;
	uint16_t prev = 0;
	clock_prescale_set(clock_div_1);
	init_usart();
	print_string("\r\n --- MPR121 test --\r\n");
	i2c_init();
	if(! mpr121_init())
		print_string("\r\n -- mpr121 non rilevato\r\n");
	else{
		mpr121_reset();
		init_interrupt();

		while(1){
			stato = 0x0000;
			if (stato != prev){
				prev = stato;
				PORTB |= (1 << PB4);
				_delay_ms(1);
			} else {
				PORTB &= ~(1<<PB4);
				_delay_ms(1);
			}
		}
	}
	return 0;
}

