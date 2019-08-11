#include <avr/io.h>
#include <util/setbaud.h>
#include "usart.h"

/* inizializzazione della USART */
void init_usart(void){
#define USART_INIT
/* impostazione del valore di prescaler in base al baudrate. 12 bit */
	UBRR0H = UBRRH_VALUE; 						/*UBRRnH = 4bit*/
	UBRR0L = UBRRL_VALUE; 						/*UBRRnL = 8bit*/
#if USE_2X 										// bit da settare per doppia velocità
  UCSR0A |= (1 << U2X0);
#else 											// forzare il bit a 0 in modo da disabilitare esplicitamente la doppia velocità
  UCSR0A &= ~(1 << U2X0);
#endif
	UCSR0B = (1 << TXEN0) | (1 << RXEN0); 		// abilitazione a trasmissione e ricezione
	UCSR0C = (1 << UCSZ01)| (1 << UCSZ00);		// impostazione della dimensione del frame da ricevere/trasmettere. 
							// UCSZ02 = 0 UCSZ01 = 1 UCASZ00 = 1 => frame da 8 bit con 1 bit di stop
}


void transmit_byte(uint8_t data){
	loop_until_bit_is_set(UCSR0A, UDRE0);		//UDRE0 è a 1 quando c'è un bit pronto per la trasmissione
	UDR0 = data;								//UDR0 contiene i dati da inviare in output
}

void transmit_word(uint8_t l, uint8_t h){
	transmit_byte(l);
	transmit_byte(h);
}

uint8_t receive_byte(void){
	loop_until_bit_is_set(UCSR0A, RXC0);		//RXC0 è a 1 quando un frame è pronto per essere ricevuto
	return UDR0;					//UDR0 contiene i dati da leggere in input
}

void flush( void )
{
	unsigned char dummy;
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}

void print_string(const char string[]){			// stampa una stringa di caratteri sul monitor seriale, un byte alla volta
	uint8_t i=0;
	while(string[i]){
		transmit_byte(string[i]);
		i++;
	}
}
void read_string(char string[], uint8_t max_length){	
	char in_char;
	uint8_t i=0;
	while(i < (max_length-1)){
		in_char = receive_byte();
		transmit_byte(in_char);			// echo
		if (in_char == '\r')
			break;
		else{
			string[i] = in_char;
			i++;
		}
	}
	string[i] = 0;
}

void print_byte(uint8_t byte){
	transmit_byte('0' + (byte / 100));
	transmit_byte('0' + ((byte / 10) % 10));
	transmit_byte('0' + (byte % 10));	
}

void print_word(uint16_t word) {
	transmit_byte('0' + (word / 10000));                 
	transmit_byte('0' + ((word / 1000) % 10));               
	transmit_byte('0' + ((word / 100) % 10)); 
	transmit_byte('0' + ((word / 10) % 10));
	transmit_byte('0' + (word % 10));                            
}

void print_binary_byte(uint8_t byte){
	uint8_t bit;
	for(bit=7; bit < 255; bit++){
		if(bit_is_set(byte, bit))
			transmit_byte('1');
		else
			transmit_byte('0');
	}
}

char nibble2hex(uint8_t nibble){
	if(nibble<10)
		return ('0' + nibble);
	else
		return ('A' + nibble - 10);
}

void print_hex_byte(uint8_t hex_byte){
	uint8_t nibble;
	nibble = (hex_byte & 0b11110000) >> 4;
	transmit_byte(nibble2hex(nibble));
	nibble = hex_byte & 0b00001111;
	transmit_byte(nibble2hex(nibble));
}



uint8_t get_number(void){
	char h = '0';
	char d = '0';
	char u = '0';
	char c = '0';

	do{
		h = d;
		d = u;
		u = c;
		c = receive_byte();
		transmit_byte(c);
	} while (c != '\r');
	return (100 * (h-'0') + 10 * (d-'0') + u-'0');
}
