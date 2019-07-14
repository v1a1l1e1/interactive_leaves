#include "mpr121.h"

uint8_t mpr121_init(void){
#ifndef I2C_INIT
	i2c_init();
	#define I2C_INIT
#endif
	mpr121_write(SRST,0x63);
	i2c_wait(TIMEOUT);
	uint8_t c = mpr121_read(CDT_GLOBAL); 
	if (c != 0x24) return 0;
	mpr121_reset();
	return 1;
}

uint8_t mpr121_read(uint8_t add){
	i2c_start();
	i2c_tx_data(SLA_W);
	i2c_tx_data(add);
	i2c_start();
	i2c_tx_data(SLA_R);
	i2c_rx_data(1);
	data = i2c_get_data();
	i2c_wait(TIMEOUT);
	i2c_nack();
	i2c_stop();
	return (data);
}

uint16_t mpr121_read2(uint8_t add){
	i2c_start();
	i2c_tx_data(SLA_W);
	i2c_tx_data(add);
	i2c_start();
	i2c_tx_data(SLA_R);
	i2c_rx_data(1);
	data = i2c_get_data();
	i2c_wait(TIMEOUT);
	i2c_rx_data(0);
	data |= ((i2c_get_data() & 0x1F) << 8);
	i2c_nack();
	i2c_stop();
	return (data);
}

void mpr121_write(uint8_t add, uint8_t data){
	i2c_start();
	i2c_tx(SLA_W);
	i2c_tx(add);
	i2c_tx(data);
	i2c_stop();
}

uint16_t mpr121_touch(void){
	uint16_t status;
	status = mpr121_read2(TOUCH_STATUS0);
	return status;
}

uint16_t mpr121_nirq(void){
	return mpr121_touch();
}

void mpr121_reset(void){
	mpr121_write(SRST, 0x63);
	_delay_ms(1);
	
	stopMode();
	
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
	mpr121_write(A_CONFIG0, 0x9F);  // FFI (must match FFI in 0x5C) | RETRY enabled (2 times) | BVA (must match CL in 0x5E) | ARE enabled| ACE enabled
	mpr121_write(CDC_GLOBAL, 0x90); 		// FFI must match FFI in 0x7B
  
	mpr121_write(CDT_GLOBAL, 0x20); 		// 0.5uS encoding, 1ms period
  
	mpr121_write(USL, 156);
	mpr121_write(TL, 140); // should correspond to ~562 ADC
	mpr121_write(LSL, 101);
  
	// preliminary setting for thresholds
	for (uint8_t i=0; i<12; i++) {
		mpr121_write(E0TTH + 2*i, 150);
		mpr121_write(E0RTH + 2*i, 50);
  	}
	_delay_ms(5);
	runMode();	
}

void mpr121_stopMode( void ){
	mpr121_write(ECR, 0xC0); // baseline tracking attiva (10 bit) + 0 elettrodi attivi
}

void mpr121_runMode( void ){
	mpr121_write(ECR, 0xCF); // baseline tracking attiva (10 bit) + 12 elettrodi attivi
}


void mpr121_set_FFI(uint8_t ffi){
	uint8_t _ffi = (ffi & 0x03);
	uint8_t cdc = mpr121_read( CDC_GLOBAL );
	cdc = cdc & 0x3F;
	
	stopMode();
	mpr121_write(CDC_GLOBAL, _ffi | cdc );
	runMode();
}

void mpr121_set_ESI(uint8_t esi){
	uint8_t _esi = esi & 0x07;
	_esi = _esi << 0;
	uint8_t cdt_sfi = mpr121_read( CDT_GLOBAL );
	cdt_sfi = cdt_sfi & 0xF8;
	
	stopMode();
	mpr121_write(CDT_GLOBAL, cdt_sfi | _esi );
	runMode();
}

void mpr121_set_SFI(uint8_t sfi){
	uint8_t _sfi = sfi & 0x03;
	_sfi = _sfi << 3;
	uint8_t cdt_esi = mpr121_read( CDT_GLOBAL );
	cdt_esi = cdt_esi & 0xE7;
	
	stopMode();
	mpr121_write(CDT_GLOBAL, cdt_esi | _sfi );
	runMode();
}

void mpr121_set_CDC(uint8_t cdc){
	uint8_t _cdc = cdc & 0x3F;
	_cdc = _cdc << 0;
	uint8_t ffi = mpr121_read( CDC_GLOBAL );
	ffi = ffi & 0xC0;
	
	stopMode();
	mpr121_write(CDC_GLOBAL, ffi | _cdc );
	runMode();
}

void mpr121_set_CDT(uint8_t cdt){
	uint8_t _cdt = cdt & 0x07;
	_cdt = _cdt << 5;
	uint8_t sfi_esi = mpr121_read( CDT_GLOBAL );
	sfi_esi = sfi_esi & 0x1F;
	
	stopMode();
	mpr121_write(CDT_GLOBAL, sfi_esi | _cdt );
	runMode();
}

/* SET DEBOUNCE
* Set how many additional readings must be 
* perceived before detecting a touch or release
*/
void mpr121_set_debounces(uint8_t touch, uint8_t release)
{
	uint8_t _touch = touch & 0x07;
	_touch = _touch << 0;
	
	uint8_t _release = release & 0x07;
	_release = _release << 4;
	
	stopMode();
	mpr121_write( DEBOUNCE_T_R, (_release | _touch) );
	runMode();
	
} 

/* Rising scenario 
* when filtered data greater than baseline values 
*/
void mpr121_rising_condition(uint8_t mhd, uint8_t nhd, uint8_t ncl, uint8_t fdl)
{
	uint8_t _mhd = mhd & 0x3F;
	uint8_t _nhd = nhd & 0x3F;
	
	stopMode();
  	mpr121_write(MHDR, _mhd); 	// maximum half-delta (0 - 63) 
  	mpr121_write(NHDR, _nhd);	// noise half-delta (0 - 63)
  	mpr121_write(NCLR, ncl);	// noise count limit
  	mpr121_write(FDLR, fdl);	// filter delay count
	runMode();	
}

/* Falling scenario 
* when filtered data are lower than baseline values 
*/
void mpr121_falling_condition(uint8_t mhd, uint8_t nhd, uint8_t ncl, uint8_t fdl)
{
	uint8_t _mhd = mhd & 0x3F;
	uint8_t _nhd = nhd & 0x3F;
	
	stopMode();
  	mpr121_write(MHDF, _mhd); 	// maximum half-delta (0 - 63) 
  	mpr121_write(NHDF, _nhd);	// noise half-delta (0 - 63)
  	mpr121_write(NCLF, ncl);	// noise count limit
  	mpr121_write(FDLF, fdl);	// filter delay count
	runMode();	
}

/* Touched scenario 
* when electrodes are in touch status 
*/
void mpr121_set_touched(uint8_t nhd, uint8_t ncl, uint8_t fdl)
{
	uint8_t _nhd = nhd & 0x3F;
	
	stopMode();
  	mpr121_write(NHDT, _nhd);	// noise half-delta (0 - 63)
  	mpr121_write(NCLT, ncl);	// noise count limit
  	mpr121_write(FDLT, fdl);	// filter delay count
	runMode();	
}

void mpr121_set_MHD(uint8_t rising, uint8_t falling) 
{
	stopMode();
  	mpr121_write(MHDR, rising); 
  	mpr121_write(MHDF, falling);
	runMode();
}

void mpr121_set_NHD(uint8_t rising, uint8_t falling, uint8_t touched) 
{
	stopMode();
  	mpr121_write(NHDR, rising); 
  	mpr121_write(NHDF, falling); 
  	mpr121_write(NHDT, touched); 
	runMode();
}

void mpr121_set_NCL(uint8_t rising, uint8_t falling, uint8_t touched) 
{
	stopMode();
  	mpr121_write(NCLR, rising); 
  	mpr121_write(NCLF, falling); 
  	mpr121_write(NCLT, touched); 
	runMode();
}

void mpr121_set_FDL(uint8_t rising, uint8_t falling, uint8_t touched) 
{
	stopMode();
  	mpr121_write(FDLR, rising); 
  	mpr121_write(FDLF, falling); 
  	mpr121_write(FDLT, touched); 
	runMode();
}

void mpr121_set_thresholds(uint8_t touch, uint8_t release) {
  stopMode();
  for (uint8_t i=0; i<12; i++) {
    mpr121_write(ETTH0 + 2*i, touch);
    mpr121_write(ERTH0 + 2*i, release);
  }
  runMode();  
}


uint16_t  mpr121_filteredData(uint8_t ele) {
  if (ele > 12) return 0;
  return (mpr121_read2(E0_DATA_LB + ele*2));
}

uint16_t  mpr121_baselineData(uint8_t ele) {
  if (ele > 12) return 0;
  return (mpr121_read(E0BV + ele) << 2);
}

uint16_t  mpr121_touched(void) {
  return ( read2(TOUCHSTATUS0) & 0x0FFF);
}