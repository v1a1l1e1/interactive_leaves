#include "SPI.h"

void SPI_init(void){

	SPI_DDR |= (1 << MOSI);
	SPI_PORT |= (1 << MISO); //pullup
	SPI_DDR |= (1 << SCK);

	SPCR |= (1 << SPR1); // F_CPU/64
	SPCR |= (1 << MSTR); // ATMEGA master
	SPCR |= (1 << SPE);
}

uint8_t SPI_trade_byte(uint8_t byte){
	SPDR = byte;
	while( (SPSR & (1 << SPIF)) == 0) ;
	return SPDR;
}

