#ifndef BAUD                          
#define BAUD  9600                    
#endif
#define   USART_HAS_DATA   bit_is_set(UCSR0A, RXC0)
#define   USART_READY      bit_is_set(UCSR0A, UDRE0)
                 
void init_usart(void);
void transmit_byte(uint8_t data);
uint8_t receive_byte(void);

void print_string(const char string[]);
void read_string(char string[], uint8_t max_length);
void print_byte(uint8_t byte);
void print_word(uint16_t word);
void print_binary_byte(uint8_t byte);
char nibble2hex(uint8_t nibble);
void print_hex_byte(uint8_t byte);
uint8_t get_number(void);

