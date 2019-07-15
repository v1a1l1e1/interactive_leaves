#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h> // per i codici di errore
#include <avr/interrupt.h>

/* defs */

#define SDA_W() DDRC |=  (1 << PC4) // output
#define SDA_R() DDRC &= ~(1 << PC4) // input
#define TWBR_V 32

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_tx_data (uint8_t data);
uint8_t i2c_rx_data(uint8_t ack);
uint8_t i2c_get_data(void);
void i2c_wait(uint8_t timeout);