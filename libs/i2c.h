#include <avr/io.h>
#include <util/twi.h> // per i codici di errore

/* defs */
#define SDA_R() DDRC = (1 << PC4)
#define SDA_W() DDRC = (0 << PC4)
#define TWBR_V 32

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_address(uint8_t add_rw);
void i2c_tx_data (uint8_t data);
uint8_t i2c_rx_data(uint8_t ack);

