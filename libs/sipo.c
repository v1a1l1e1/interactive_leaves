#include "sipo.h"

void sipo_setup(void){
	SIPO_DDR |= (1 << SIPO_SS) | (1 << SCLR) | (1 << OE);
	SIPO_PORT &= ~(1 << OE);
	SLAVE_DESELECT(SIPO_SS_PORT, SIPO_SS);
}

uint8_t sipo_write(uint8_t byte){
	SLAVE_SELECT(SIPO_SS_PORT, SIPO_SS);
	uint8_t tmp = SPI_trade_byte(byte);
	SLAVE_DESELECT(SIPO_SS_PORT, SIPO_SS);
	return tmp;
}

void sipo_clear(void){
	SLAVE_SELECT(SIPO_SS_PORT, SIPO_SS);
	CLEAR;
	SLAVE_DESELECT(SIPO_SS_PORT, SIPO_SS);
}
