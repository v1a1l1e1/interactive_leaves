#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>

/* defs */

#define SDA_W() DDRC |=  (1 << PC4) // output
#define SDA_R() DDRC &= ~(1 << PC4) // input
#define TWBR_V 32

/* dichiarazioni */

void i2c_init(void);
void i2c_start(void);
void i2c_stop(uint8_t addr);
uint8_t i2c_tx_data (uint8_t data);
uint8_t i2c_rx_data(uint8_t ack);
uint8_t i2c_get_data(void);
uint8_t i2c_get_status(void);

/*definizioni */

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
	if (ack){
		TWCR = (TWCR & 0x0f) | (1 << TWINT) | (1 << TWEN) | (1 << TWEA));
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

uint8_t i2c_get_status(void){
	return TW_STATUS;
}
