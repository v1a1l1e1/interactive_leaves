#include "i2c.h"

void i2c_init(void){
	/*
	  ------------------------------------------
	  | freq = F_CPU / (16 + 2*TWBR*prescaler) |
	  ------------------------------------------
		F_CPU = 8MHz
		TWBR = 32
		freq = 8MHz/(16+2*32) ==> ~100KHz
	*/		
	TWBR = TWBR_V; // 
	TWSR &= (~(1<<TWPS0) | ~(1 << TWPS1)); // prescaler = 1
	TWCR |= (1 << TWEN);
	#define I2C_INIT
}

void i2c_start(void){
	SDA_W();
	TWCR = (1<<TWINT) | (1 << TWEN) | (1 << TWSTA);
	while ((TWCR & (1 << TWINT)) == 0) 
		;
}

void i2c_stop(void){
	SDA_W();
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void i2c_tx_data(uint8_t data){
	SDA_W();
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0) 
			;
}

uint8_t i2c_rx_data (uint8_t ack){
	SDA_R();
	if (ack){
		TWCR = (TWCR & 0x0f) | (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
		while ((TWCR & (1 << TWINT)) == 0) 
			;
	}
	else {
		TWCR = (TWCR & 0x0f) | (1 << TWINT) | (1 << TWEN);
		while ((TWCR & (1 << TWINT)) == 0) 
			;
	}
	return (i2c_get_data());
}

uint8_t i2c_get_data(void){
	return TWDR;
}

void i2c_wait(uint8_t timeout){
	uint8_t i=0;
	while ( ( (TWCR & (1 << TWINT)) == 0) && (i < timeout))
		++i;
}