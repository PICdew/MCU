#include "16f726.h"
#include "c_16f726.h"

#pragma config = ( HS_ & WDTDIS_ & PWRTEN_ & MCLRDIS_ & UNPROTECT_ & BOREN_ & BORV25_ & PLLDIS_ & DEBUGDIS_ )

#pragma bit TRISC6   @ 0x87.6
#pragma bit TRISC7   @ 0x87.7
#pragma bit MAX7219_LOAD_PIN	@ PORTC.2

#pragma wideConstData p	

//OPTION  = 0x81 ;  1:4 prescaler ,10M/4/2=1.25M = 0.8u
#define	DELAY_100U		(unsigned char)(256 -(100.0/0.8) +2)  

/* global variables */
#define NText 	12
const char text[NText] = "Hello world!";

#include "sci-lib.c"
#include "max7219.c"
#include "common.c"

void init_comms(void){
	TRISC6 = 1;		// TX
	SPBRG = 129;	// 20MHz,9600,16F722	
//	RCSTA = 0x90;
//	TXSTA = 0x24;

	SPEN = 1;		//Serial Port Enable Bit
	SYNC = 0;		//0:Asychronous
	BRGH = 1;		//1:High Speed
	TXEN = 1;

	TRISC7 = 1;		// RX
	CREN = 1;
}

//master
//RC2      --> max7219 load (active low)
//RC3 SCK  --> max7219 sck
//RC4 SDI no used
//RC5 SDO  --> max7219 DI
void init_spi(void){

	SSPCON	= 0b.0010.0000;
	/*
	bit7   WCOL:  0
	bit6   SSPOV: 0 ,no receive overflow indicator bit(無使用SDI)
	bit5   SSPEN: 1 ,enable SCK,SDO,SDI
	bit4   CPK: 0, idle state for clk is a low level
	bit0-3 SSPM<3:0>: 0000 SPI Master, clk = fosc/4, 20MHZ/4=5Mhz

	max7219 clk 上升取DI，無動作clk為low
	16f726 => master, CKP = 0,CKE = 1
	*/

	CKE 	= 1;
	/*
	CKE   1,Data stable on rising edge of SCK
	SSPSTAT.6
	*/

	/* low active, B0-B3 */
	/* MAX7219_LOAD_PIN = 1 */
	PORTC 	= 0b.0000.0100;
	TRISC 	= 0b.0000.0000; /* output */
}

void main( void)
{
	uns8	i;
    if (TO == 1 && PD == 1 /* power up */)  {
    //   WARM_RESET:
         clearRAM(); // clear all RAM
    }
	TRISC	= 0xD0;			//TX,RX,SDI -input
	PIE1 	= 0;
	INTCON	= 0;
	init_comms();
	init_spi();

	OPTION  = 0x81;  //1:4 prescaler ,10M/4/4=1.25M = 0.8u

	crlf();
	for(i=0; i<NText; i++) putch(text[i]);	crlf();
	for(i=0; i<NText; i++) putch(text[i]);  crlf();
	for(i=0; i<NText; i++) putch(text[i]);  crlf();
	crlf();

	max7129_init();
#ifdef MAX7219_DIGTAL_MODE			
	setCommand(max7219_reg_digit0,0);
#else
	setCommand(max7219_reg_digit0,var2led[0]);
#endif	

	while(1){
		for(i=0; i<255; i++){		
			set10base(i);
			setCommand(max7219_reg_digit0,v10base[0]);
			setCommand(max7219_reg_digit1,v10base[1]);
			setCommand(max7219_reg_digit2,v10base[2]);	
			delayms(200);		
		}
	}	
}
