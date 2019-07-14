/******************************************/
/*                                        */
/*          REGISTRI MPR121               */
/*                                        */
/*            10/07/2019                  */
/*                                        */
/******************************************/


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

uint16_t mpr121_send_touch_status(void);
void mpr121_autoconf(void);