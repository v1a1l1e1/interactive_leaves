#include <avr/io.h>
#include "SPI.h"

#define PISO_DDR DDRB
#define PISO_PORT PORTB
#define SH_LD PB6
#define LOAD PISO_PORT &= ~(1 << SH_LD)
#define LATCH PISO_PORT |= (1 << SH_LD)

void piso_setup(void);
uint8_t piso_read(void);
