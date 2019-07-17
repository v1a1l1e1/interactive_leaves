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
	print_string("\r\n\t\t\t ---> i2c_init()\r\n");		
	TWBR = TWBR_V; // 
	TWSR = (0<<TWPS0)|(0 << TWPS1); // prescaler = 1
	TWCR |= (1 << TWEN);
}

void i2c_start(void){
	print_string("\r\n\t\t\t ---> i2c_start()\r\n");
	TWCR = (1<<TWINT)|(1 << TWSTA)|(1 << TWEN);
	while ((TWCR & (1 << TWINT))==0) 
		;
	while ((TWSR & 0xF8)!=0x08)	// aspetto ack
		;
}

void i2c_stop(void){
	print_string("\r\n\t\t\t ---> i2c_stop()\r\n");
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	while(!(TWCR & (1<<TWSTO)))
		;
}

void i2c_address(uint8_t add_rw){
	print_string("\r\n\t\t\t ---> i2c_address()\r\n");
	SDA_W();
	TWDR = add_rw;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))) ; //tx completato
}

void i2c_tx_data(uint8_t data){
	print_string("\r\n\t\t\t ---> i2c_tx_data()\r\n");
	SDA_W();
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT))) ;
	print_string("\r\n\t\t\t ---> i2c_tx_data() OK\r\n");
}

uint8_t i2c_rx_data (uint8_t ack){
	print_string("\r\n\t\t\t ---> i2c_rx_data()\r\n");
	SDA_R();
	if (ack){
		print_string("\r\n\t\t\t\t ===> ack true\r\n");
		TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWEA));
	}else{
		print_string("\r\n\t\t\t\t ===> ack false\r\n"); 
		TWCR = (1 << TWINT) | (1 << TWEN);
	}
	while(! (TWCR & (1<<TWINT))) print_string("\r\n\t\t\t\t ===> aspetto TWINT\r\n");
	//while((TWSR & 0xF8) != 0x58) print_string("\r\n\t\t\t\t ===> aspetto ACK\r\n");
	print_string("\r\n\t\t\t ---> i2c_rx_data() OK\r\n");
	return (TWDR);
}