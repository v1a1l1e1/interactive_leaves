/******************************************/
/*                                        */
/*  REGISTRI MACRO E FUNZIONI MPR121      */
/*                                        */
/*            10/07/2019                  */
/*                                        */
/******************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c.h"

// touch status per i 13 canali
#define TOUCH_STATUS0 0x00
#define TOUCH_STATUS1 0x01

// out of range status per i 13 canali
#define OOR0 0x02
#define OOR1 0x03


// dati dai 13 elettrodi (LB=1Byte, HB=2bit)
#define E0_DATA_LB 0x04
#define E0_DATA_HB 0x05
#define E1_DATA_LB 0x06
#define E1_DATA_HB 0x07
#define E2_DATA_LB 0x08
#define E2_DATA_HB 0x09
#define E3_DATA_LB 0x0A
#define E3_DATA_HB 0x0B
#define E4_DATA_LB 0x0C
#define E4_DATA_HB 0x0D
#define E5_DATA_LB 0x0E
#define E5_DATA_HB 0x0F
#define E6_DATA_LB 0x10
#define E6_DATA_HB 0x11
#define E7_DATA_LB 0x12
#define E7_DATA_HB 0x13
#define E8_DATA_LB 0x14
#define E8_DATA_HB 0x15
#define E9_DATA_LB 0x16
#define E9_DATA_HB 0x17
#define E10_DATA_LB 0x18
#define E10_DATA_HB 0x19
#define E11_DATA_LB 0x1A
#define E11_DATA_HB 0x1B
#define EPROX_DATA_LB 0x1C
#define EPROX_DATA_HB 0x1D


// valore di baseline per i 13 canali
#define E0BV	0x1E
#define E1BV	0x1F
#define E2BV	0x20
#define E3BV	0x21
#define E4BV	0x22
#define E5BV	0x23
#define E6BV	0x24
#define E7BV	0x25
#define E8BV	0x26
#define E9BV	0x27
#define E10BV	0x28
#define E11BV	0x29
#define EPROXBV	0x2A


// impostazioni per il filtro 
// 3 stadi: Rising, Falling, Touched
// 4 valori per ogni stadio (tranne touched): 
//		MHD = Maximum Half Delta 
//		NHD = Noise Half Delta
//		NCL = Noise Count Limit
//		FDL = Filter Delay Count Limit 
#define MHDR 0x2B
#define NHDR 0x2C
#define NCLR 0x2D
#define FDLR 0x2E
#define MHDF 0x2F
#define NHDF 0x30
#define NCLF 0x31
#define FDLF 0x32
#define NHDT 0x33
#define NCLT 0x34
#define FDLT 0x35
#define MHDPROXR 0x36
#define NHDPROXR 0x37
#define NCLPROXR 0x38
#define FDLPROXR 0x39
#define MHDPROXF 0x3A
#define NHDPROXF 0x3B
#define NCLPROXF 0x3C
#define FDLPROXF 0x3D
#define NHDPROXT 0x3E
#define NCLPROXT 0x3F
#define FDLPROXT 0x40


// Threshold per il tocco e il rilascio
#define E0TTH 0x41
#define E0RTH 0x42
#define E1TTH 0x43
#define E1RTH 0x44
#define E2TTH 0x45
#define E2RTH 0x46
#define E3TTH 0x47
#define E3RTH 0x48
#define E4TTH 0x49
#define E4RTH 0x4A
#define E5TTH 0x4B
#define E5RTH 0x4C
#define E6TTH 0x4D
#define E6RTH 0x4E
#define E7TTH 0x4F
#define E7RTH 0x50
#define E8TTH 0x51
#define E8RTH 0x52
#define E9TTH 0x53
#define E9RTH 0x54
#define E10TTH 0x55
#define E10RTH 0x56
#define E11TTH 0x57
#define E11RTH 0x58
#define EPROXTTH 0x59
#define EPROXRTH 0x5A

// debounce in scanning 
#define DEBOUNCE_T_R 0x5B

// impostazioni globali corrente di carica e tempo di carica
#define CDC_GLOBAL 0x5C
#define CDT_GLOBAL 0x5D

// registro di controllo
#define ECR 0x5E

// impostazioni per singli elettrodi della corrente di carica
#define CDC0 0x5F
#define CDC1 0x60
#define CDC2 0x61
#define CDC3 0x62
#define CDC4 0x63
#define CDC5 0x64
#define CDC6 0x65
#define CDC7 0x66
#define CDC8 0x67
#define CDC9 0x68
#define CDC10 0x69
#define CDC11 0x6A
#define CDCPROX 0x6B

// impostazioni per singoli elettrodi del tempo di carica
#define CDT0_CDT1 0x6C
#define CDT2_CDT3 0x6D
#define CDT4_CDT5 0x6E
#define CDT6_CDT7 0x6F
#define CDT8_CDT9 0x70
#define CDT10_CDT11 0x71
#define CDTPROX 0x72

// funzionalità dei canali come GPIO
#define GPIOCR0 0x73
#define GPIOCR1 0x74
#define GPIO_DATA 0x75
#define GPIO_DIR 0x76
#define GPIO_EN 0x77
#define GPIO_DATA_SET 0x78
#define GPIO_DATA_CLEAR 0x79
#define GPIO_DATA_TOGGLE 0x7A

// Autoconfigurazione e Autoreconfigurazione
#define A_CONFIG0 0X7B
#define A_CONFIG1 0x7C
#define USL 0x7D
#define LSL 0x7E
#define TL 0x7D

// soft reset
#define SRST 0x80

#define MPR121_ADD8 0x5A
#define MPR121_ADD (MPR121_ADD << 1)
#define SLA_R (MPR121_ADD | 0x01)
#define SLA_W (MPR121_ADD & 0xFE)

#define TIMEOUT 50
#define ACK 

uint8_t mpr121_send_touch(void);
void mpr121_autoconf(void);
uint8_t mpr121_read(uint8_t add);
void mpr121_write(uint8_t add);
uint16_t mpr121_nirq(void);
void mpr121_init(void);
void mpr121_reset(void);
void mpr121_stopMode(void);
void mpr121_runMode( void )

uint8_t mpr121_init(void){
#ifndef I2C_INIT
	i2c_init();
#endif
	mpr121_write(SRST,0x63);
	i2c_wait(TIMEOUT);
	uint8_t c = mpr121_read(MPR121_CONFIG2); 
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
	status = mpr121_read(TOUCH_STATUS0);
	status |= ((mpr121_read(TOUCH_STATUS1) & 0x1F) << 8);
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
/* TODO ---> da commentare */  
	mpr121_write(MPR121_AUTOCONFIG0, 0x9F);  // FFI (must match FFI in 0x5C) | RETRY enabled (2 times) | BVA (must match CL in 0x5E) | ARE enabled| ACE enabled
	mpr121_write(MPR121_CONFIG1, 0x90); 		// FFI must match FFI in 0x7B
  
	mpr121_write(MPR121_CONFIG2, 0x20); 		// 0.5uS encoding, 1ms period
  
	mpr121_write(MPR121_UPLIMIT, 156);
	mpr121_write(MPR121_TARGETLIMIT, 140); // should correspond to ~562 ADC
	mpr121_write(MPR121_LOWLIMIT, 101);
  
	// preliminary setting for thresholds
	for (uint8_t i=0; i<12; i++) {
		mpr121_write(MPR121_TOUCHTH_0 + 2*i, 150);
		mpr121_write(MPR121_RELEASETH_0 + 2*i, 50);
  	}
	_delay_ms(5);
	runMode();	
}

void mpr121_stopMode( void )
{
	mpr121_write(ECR, 0xC0); // baseline tracking attiva (10 bit) + 0 elettrodi attivi
}

void mpr121_runMode( void )
{
	mpr121_write(ECR, 0xCF); // baseline tracking attiva (10 bit) + 12 elettrodi attivi
}


void mpr121_set_FFI(uint8_t ffi)
{
	uint8_t _ffi = (ffi & 0x03);
	uint8_t cdc = mpr121_read( MPR121_CONFIG1 );
	cdc = cdc & 0x3F;
	
	stopMode();
	mpr121_write(MPR121_CONFIG1, _ffi | cdc );
	runMode();
}

void mpr121_set_ESI(uint8_t esi)
{
	uint8_t _esi = esi & 0x07;
	_esi = _esi << 0;
	uint8_t cdt_sfi = mpr121_read( MPR121_CONFIG2 );
	cdt_sfi = cdt_sfi & 0xF8;
	
	stopMode();
	mpr121_write(MPR121_CONFIG2, cdt_sfi | _esi );
	runMode();
}

void mpr121_set_SFI(uint8_t sfi)
{
	uint8_t _sfi = sfi & 0x03;
	_sfi = _sfi << 3;
	uint8_t cdt_esi = mpr121_read( MPR121_CONFIG2 );
	cdt_esi = cdt_esi & 0xE7;
	
	stopMode();
	mpr121_write(MPR121_CONFIG2, cdt_esi | _sfi );
	runMode();
}

void mpr121_set_CDC(uint8_t cdc)
{
	uint8_t _cdc = cdc & 0x3F;
	_cdc = _cdc << 0;
	uint8_t ffi = mpr121_read( MPR121_CONFIG1 );
	ffi = ffi & 0xC0;
	
	stopMode();
	mpr121_write(MPR121_CONFIG1, ffi | _cdc );
	runMode();
}

void mpr121_set_CDT(uint8_t cdt)
{
	uint8_t _cdt = cdt & 0x07;
	_cdt = _cdt << 5;
	uint8_t sfi_esi = mpr121_read( MPR121_CONFIG2 );
	sfi_esi = sfi_esi & 0x1F;
	
	stopMode();
	mpr121_write(MPR121_CONFIG2, sfi_esi | _cdt );
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
	mpr121_write( MPR121_DEBOUNCE, (_release | _touch) );
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
  	mpr121_write(MPR121_FDLR, fdl);	// filter delay count
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
    mpr121_write(MPR121_TOUCHTH_0 + 2*i, touch);
    mpr121_write(MPR121_RELEASETH_0 + 2*i, release);
  }
  runMode();  
}


uint16_t  mpr121_filteredData(uint8_t t) {
  if (t > 12) return 0;
  return readRegister16(MPR121_FILTDATA_0L + t*2);
}

uint16_t  mpr121_baselineData(uint8_t t) {
  if (t > 12) return 0;
  uint16_t bl = mpr121_read(MPR121_BASELINE_0 + t);
  return (bl << 2);
}

uint16_t  Limulo_MPR121::touched(void) {
  uint16_t t = readRegister16(MPR121_TOUCHSTATUS_L);
  return t & 0x0FFF;
}

// print CDC / CDT
// print CDC and CDT in a more readable way
void mpr121_printCDC( void )
{
	uint8_t cdc = mpr121_read( MPR121_CONFIG1 ) & 0x3F;
	Serial.print("CDC: ");
	Serial.print( cdc, DEC);
	Serial.println(" uA;");
}


void mpr121_printCDT( void )
{
	uint8_t cdt = mpr121_read( MPR121_CONFIG2 ) & 0xE0;
	cdt = cdt >> 5;
	signed int exp = cdt - 2;
	float time = (float) pow( 2, exp );
	Serial.print("CDT: ");
	Serial.print( time, DEC);
	Serial.println(" uS;");
}
