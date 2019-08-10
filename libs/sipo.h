#include <avr/io.h>
#include "SPI.h"

#define SIPO_DDR DDRB
#define SIPO_SS PB1 
#define SIPO_SS_PORT PORTB
#define SIPO_PORT PORTB
#define SCLR  PB0
#define OE PB2

#define CLEAR SIPO_PORT &= (1 << SCLR)

void sipo_setup(void);
uint8_t sipo_write(uint8_t byte);
void sipo_clear(void);