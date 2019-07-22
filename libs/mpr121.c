#include <util/delay.h>
#include "mpr121.h"
#include "usart.h"

uint8_t mpr121_init(void){
#ifndef I2C_INIT
	i2c_init();
#endif
	i2c_init();
	mpr121_write(SRST,0x63);
	uint8_t t= mpr121_read(CDT_GLOBAL);
	print_string("\r\nCDT = ");	
	print_byte(t);
	print_string("\r\n");
	mpr121_reset();
	return 1;
}

uint8_t mpr121_read(uint8_t add){
	uint8_t data;
	i2c_start();
	i2c_address(SLA_W);
	i2c_tx_data(add);
	i2c_restart();
	i2c_tx_data(SLA_R);
	data = i2c_rx_data(NACK);
	i2c_stop();
	return (data);
}

// per dati a 16 bit su due registri da 1 byte
uint16_t mpr121_read2(uint8_t addl, uint8_t addh){
	uint16_t data;
	i2c_start();
	i2c_tx_data(SLA_W);
	i2c_tx_data(addl);
	i2c_restart();
	i2c_tx_data(SLA_R);
	data = i2c_rx_data(ACK);
	i2c_tx_data(SLA_W);
	i2c_tx_data(addh);
	data |= ( (i2c_rx_data(NACK) & 0x1F) << 8);
	i2c_stop();
	return (data);
}

void mpr121_write(uint8_t add, uint8_t data){
	i2c_start();
	i2c_address(SLA_W);
	i2c_tx_data(add);
	i2c_tx_data(data);
	i2c_stop();
}
/*
uint16_t  mpr121_touched(void) {
  return ( mpr121_read2(TOUCH_STATUS0, TOUCH_STATUS1) & 0x0FFF);
}

uint16_t mpr121_touch(void){
	return (mpr121_read2(TOUCH_STATUS0, TOUCH_STATUS1));
}
*/
uint16_t mpr121_read_status(void){
	uint16_t s = (uint16_t)mpr121_read(TOUCH_STATUS1);
	s = (s << 8);
	s |= mpr121_read(TOUCH_STATUS0);
	return s;
}

void format_status(uint16_t s){
	//print_byte('P');
	s = s & 0x0FFF;
	print_word(s);
	/*uint8_t i;
	for (i=0; i < 12; i++){
		if ( (s & (1<<(11-i))) )
			print_string("1");
		else
			print_string("0");
	}*/
	//print_string("\r\n");
}

void mpr121_nirq(void){
	//format_status(mpr121_read_status());
	transmit_byte(mpr121_read(TOUCH_STATUS0));
	transmit_byte(mpr121_read(TOUCH_STATUS1)<<4);
}

void mpr121_reset(void){
	mpr121_write(SRST, 0x63);
	_delay_us(500);
	
	mpr121_stopMode();
	
    /* Dati filtrati > baseline */
	mpr121_write(MHDR, 0x01); 	// maximum half-delta (0 - 63) 
	mpr121_write(NHDR, 0x08);	// noise half-delta (0 - 63)
    mpr121_write(NCLR, 0x01);	// noise count limit
  	mpr121_write(FDLR, 0x00);	// filter delay count

	/* Dati filtrati < baseline */
	mpr121_write(MHDF, 0x01); // (0 - 63)
    mpr121_write(NHDF, 0x01); // (0 - 63)
    mpr121_write(NCLF, 0x02); //
	mpr121_write(FDLF, 0x01); //
  
	/* Condizione di tocco */
	mpr121_write(NHDT, 0x00); // (0 - 63)
	mpr121_write(NCLT, 0x00); //
	mpr121_write(FDLT, 0x00); //
  
	mpr121_write(DEBOUNCE_T_R, 0x11); 	// (EFI x SFI x 2 (ms) delay)
	mpr121_write(A_CONFIG0, 0x9F);  	// FFI (must match FFI in 0x5C) | RETRY enabled (2 times) | BVA (must match CL in 0x5E) | ARE enabled| ACE enabled
	mpr121_write(CDC_GLOBAL, 0x90); 	// FFI must match FFI in 0x7B
  
	mpr121_write(CDT_GLOBAL, 0x20); 	// 0.5uS encoding, 1ms period
  
	mpr121_write(USL, 156);
	mpr121_write(TL, 140); 				// ~562 ADC
	mpr121_write(LSL, 101);
  
	// impostazione preliminare delle soglie
	for (uint8_t i=0; i<12; i++) {
		mpr121_write(E0TTH + 2*i, 150);
		mpr121_write(E0RTH + 2*i, 50);
  	}
	_delay_us(500);
	mpr121_runMode();	
}

// baseline tracking attiva (10 bit) + 0 elettrodi attivi
void mpr121_stopMode( void ){
	mpr121_write(ECR, 0xC0); 	
}

// baseline tracking attiva (10 bit) + 12 elettrodi attivi
void mpr121_runMode( void ){
	mpr121_write(ECR, 0xCF); 
}


void mpr121_set_FFI(uint8_t ffi_){
	uint8_t ffi = (ffi_ & 0x03);
	uint8_t cdc = (mpr121_read( CDC_GLOBAL) & 0x3f);
	
	mpr121_stopMode();
	mpr121_write(CDC_GLOBAL, (ffi | cdc) );
	mpr121_runMode();
}

void mpr121_set_ESI(uint8_t esi_){
	uint8_t esi = esi_ & 0x07;
	uint8_t cdt_sfi = (mpr121_read( CDT_GLOBAL ) & 0xF8);
	
	mpr121_stopMode();
	mpr121_write(CDT_GLOBAL, (cdt_sfi | esi) );
	mpr121_runMode();
}

void mpr121_set_SFI(uint8_t sfi_){
	uint8_t sfi = ((sfi_ & 0x03) << 3);
	uint8_t cdt_esi = (mpr121_read( CDT_GLOBAL ) & 0xE7);
	
	mpr121_stopMode();
	mpr121_write(CDT_GLOBAL, (cdt_esi | sfi) );
	mpr121_runMode();
}

void mpr121_set_CDC(uint8_t cdc_){
	uint8_t cdc = cdc_ & 0x3F;
	uint8_t ffi = (mpr121_read( CDC_GLOBAL ) & 0xC0);
	
	mpr121_stopMode();
	mpr121_write(CDC_GLOBAL, (ffi | cdc) );
	mpr121_runMode();
}

void mpr121_set_CDT(uint8_t cdt_){
	uint8_t cdt = ((cdt_ & 0x07) << 5);
	uint8_t sfi_esi = (mpr121_read( CDT_GLOBAL) & 0x1F);
	
	mpr121_stopMode();
	mpr121_write(CDT_GLOBAL, sfi_esi | cdt );
	mpr121_runMode();
}

void mpr121_set_debounces(uint8_t t, uint8_t r)
{
	uint8_t touch = t & 0x07;
	uint8_t release = r & 0x07;
	
	mpr121_stopMode();
	mpr121_write( DEBOUNCE_T_R, ((release << 4) | touch) );
	mpr121_runMode();
	
} 

void mpr121_rising_condition(uint8_t mhd_, uint8_t nhd_, uint8_t ncl, uint8_t fdl)
{
	uint8_t mhd = mhd_ & 0x3F;
	uint8_t nhd = nhd_ & 0x3F;
	
	mpr121_stopMode();
  	mpr121_write(MHDR, mhd); 	// maximum half-delta (0 - 63) 
  	mpr121_write(NHDR, nhd);	// noise half-delta (0 - 63)
  	mpr121_write(NCLR, ncl);	// noise count limit
  	mpr121_write(FDLR, fdl);	// filter delay count
	mpr121_runMode();	
}

void mpr121_falling_condition(uint8_t mhd_, uint8_t nhd_, uint8_t ncl, uint8_t fdl)
{
	uint8_t mhd = mhd_ & 0x3F;
	uint8_t nhd = nhd_ & 0x3F;
	
	mpr121_stopMode();
  	mpr121_write(MHDF, mhd); 	// maximum half-delta (0 - 63) 
  	mpr121_write(NHDF, nhd);	// noise half-delta (0 - 63)
  	mpr121_write(NCLF, ncl);	// noise count limit
  	mpr121_write(FDLF, fdl);	// filter delay count
	mpr121_runMode();	
}

void mpr121_touched_condition(uint8_t nhd_, uint8_t ncl, uint8_t fdl)
{
	uint8_t nhd = nhd_ & 0x3F;
	
	mpr121_stopMode();
  	mpr121_write(NHDT, nhd);	// noise half-delta (0 - 63)
  	mpr121_write(NCLT, ncl);	// noise count limit
  	mpr121_write(FDLT, fdl);	// filter delay count
	mpr121_runMode();	
}

void mpr121_set_MHD(uint8_t rising, uint8_t falling) 
{
	mpr121_stopMode();
  	mpr121_write(MHDR, rising); 
  	mpr121_write(MHDF, falling);
	mpr121_runMode();
}

void mpr121_set_NHD(uint8_t rising, uint8_t falling, uint8_t touched) 
{
	mpr121_stopMode();
  	mpr121_write(NHDR, rising); 
  	mpr121_write(NHDF, falling); 
  	mpr121_write(NHDT, touched); 
	mpr121_runMode();
}

void mpr121_set_NCL(uint8_t rising, uint8_t falling, uint8_t touched) 
{
	mpr121_stopMode();
  	mpr121_write(NCLR, rising); 
  	mpr121_write(NCLF, falling); 
  	mpr121_write(NCLT, touched); 
	mpr121_runMode();
}

void mpr121_set_FDL(uint8_t rising, uint8_t falling, uint8_t touched) 
{
	mpr121_stopMode();
  	mpr121_write(FDLR, rising); 
  	mpr121_write(FDLF, falling); 
  	mpr121_write(FDLT, touched); 
	mpr121_runMode();
}

void mpr121_set_thresholds(uint8_t touch, uint8_t release) {
  mpr121_stopMode();
  for (uint8_t i=0; i<12; i++) {
    mpr121_write(E0TTH + 2*i, touch);
    mpr121_write(E0RTH + 2*i, release);
  }
  mpr121_runMode();  
}

void mpr121_set_threshold(uint8_t ele, uint8_t touch, uint8_t release) {
  	mpr121_stopMode();
  	mpr121_write(ele, touch);
  	mpr121_write(ele, release);
	mpr121_runMode();  
}

uint16_t  mpr121_filteredData(uint8_t ele) {
  if (ele > 12) return 0;
  return (mpr121_read2(E0_DATA_LB + ele*2, E0_DATA_HB + ele*2));
}

uint16_t  mpr121_baselineData(uint8_t ele) {
  if (ele > 12) return 0;
  return (mpr121_read(E0BV + ele) << 2);
}

