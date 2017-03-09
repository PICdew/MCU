/*******************************************************************************
 *
 * 12F6877 CC5X Config
 *
 *******************************************************************************/
#ifndef CONFIG_16F722_
#define	CONFIG_16F722_

// *	PIC 16F726 chip
// Configuration Mask Definitions
#define CONFIG_ADDR_ 	0x2007

// Oscillator configurations 
#define RCCLKO_		0x3FFF
#define RCIO_		0x3FFE
#define INTCLKO_	0x3FFD
#define INTIO_		0x3FFC
#define EC_			0x3FFB
#define HS_			0x3FFA
#define XT_			0x3FF9
#define LP_			0x3FF8
// Watchdog timer enable 
#define WDTEN_		0x3FFF
#define WDTDIS_		0x3FF7
// Power up timer enable 
#define PWRTEN_		0x3FEF
#define PWRTDIS_	0x3FFF
// MCLR pin function 
#define MCLREN_		0x3FFF
#define MCLRDIS_	0x3FDF
// Protection of flash memory 
#define PROTECT_	0x3FBF
#define UNPROTECT_	0x3FFF
// Brown out reset enable 
#define BOREN_		0x3FFF
#define BOREN_XSLP_	0x3EFF
#define BORDIS_		0x3CFF
// Brown out reset voltage 
#define BORV25_		0x3BFF
#define BORV19_		0x3FFF
// INTOSC PLL enable 
#define PLLEN_		0x3FFF
#define PLLDIS_		0x2FFF
// Debugger enable 
#define DEBUGEN_	0x1FFF
#define DEBUGDIS_	0x3FFF

#define CONFIG_ADDR2_	0x2008
// Voltage regulator capacitor enable - this setting ignored in 16LF devices
#define VCAPRA0_		0x3FCF
#define VCAPRA5_		0x3FDF
#define VCAPRA6_		0x3FEF
#define VCAPDIS_		0x3FFF

#endif