#include "piso.h"

void piso_setup(void){
	PISO_DDR |= (1 << SH_LD);
	LOAD;
}


uint8_t piso_read(void){
	uint8_t byte;	
	LATCH;
	byte = SPI_trade_byte(0x00);
	LOAD;
	return byte;
}