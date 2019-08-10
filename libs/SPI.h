#include <avr/io.h>

#define MISO  PB4
#define MOSI  PB3
#define SCK   PB5

#define SPI_DDR  DDRB
#define SPI_PORT PORTB


#define SLAVE_SELECT(port,pin)   port &= ~(1 << pin)
#define SLAVE_DESELECT(port,pin) port |= (1 << pin)


void SPI_init(void);
uint8_t SPI_trade_byte(uint8_t byte);
