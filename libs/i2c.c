#include <util/delay.h>
#include "i2c.h"
#include "usart.h"

/*
  ------------------------------------------
  | freq = F_CPU / (16 + 2*TWBR*prescaler) |
  ------------------------------------------
	F_CPU = 8MHz
	TWBR = 32
	freq = 8MHz/(16+2*32) ==> ~100KHz
*/

void i2c_init(void){
#define I2C_INIT
#ifndef USART_INIT
	init_usart();
#endif		
	TWBR = TWBR_V; // 
	TWSR = (0<<TWPS0)|(0 << TWPS1); // prescaler = 1
	TWCR |= (1 << TWEN);
}

void i2c_start(void){
	_delay_us(1);	
	TWCR = (1<<TWINT)|(1 << TWSTA)|(1 << TWEN);
	while ((TWCR & (1 << TWINT))==0) 
		;
	while ((TWSR & 0xF8)!=0x08)	// START CONDITION trasmessa
		;
}

void i2c_restart(void){
	TWCR = (1<<TWINT)|(1 << TWSTA)|(1 << TWEN);
	while ((TWCR & (1 << TWINT))==0) 
		;
	while ((TWSR & 0xF8)!=0x10)	// START CONDITION trasmessa
		;
}

void i2c_stop(void){
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	while(!(TWCR & (1<<TWSTO)))
		;
}

void i2c_address(uint8_t add_rw){
	SDA_W();
	TWDR = add_rw;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))) ; //tx completato
	if((TWSR & 0xF8) == 0x18) ;//print_string("\r\n\t\t\t ---> SLA+W (t) e ACK (r)\r\n"); 
	if((TWSR & 0xF8) == 0x40) ;//print_string("\r\n\t\t\t ---> SLA+R (t) e ACK (r)\r\n"); 
}

void i2c_tx_data(uint8_t data){
	SDA_W();
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT))) ;
	if ((TWSR & 0xF8) == 0x28) ;//print_string("\r\n\t\t\t ---> data (t) e ACK (r)\r\n"); 
}

uint8_t i2c_rx_data (uint8_t ack){
	SDA_R();
	if (ack){
		TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWEA));
	}else{ 
		TWCR = (1 << TWINT) | (1 << TWEN);
	}
	while(! (TWCR & (1<<TWINT))) ;// print_string("\r\n\t\t\t\t ===> aspetto TWINT\r\n");
	if ((TWSR & 0xF8) == 0x50) ;//print_string("\r\n\t\t\t ---> data (r) e ACK (t)\r\n");
	if ((TWSR & 0xF8) == 0x58) ;//print_string("\r\n\t\t\t ---> data (r) e NACK (t)\r\n"); 
	return (TWDR);
}