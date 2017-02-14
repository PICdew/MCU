#include "16f726.h"
#include "c_16f726.h"

#pragma config = ( HS_ & WDTDIS_ & PWRTEN_ & MCLRDIS_ & UNPROTECT_ & BOREN_ & BORV25_ & PLLDIS_ & DEBUGDIS_ )

#pragma wideConstData p	

#pragma bit TRISC6   @ 0x87.6
#pragma bit TRISC7   @ 0x87.7

/* global variables */
#define CNT1S_V	5
uns8	cnt1s;
uns8 	cntNum;

void int_writeUNS8(uns8 v);
void int_crlf(void);

#include "int16CXX.h"
#pragma origin 4
interrupt int_server(void)
{
  int_save_registers    // W, STATUS (and PCLATH)
	if(INTF){
		INTF	= 0;

		int_writeUNS8(cntNum);
		int_crlf();
		cntNum++;
	}
   int_restore_registers // W, STATUS (and PCLATH)
}

uns8 int_hex2char(uns8 i){
	skip(i);
    #pragma return[] = '0' '1' '2' '3' '4' '5' '6' '7' '8' '9' 'A' 'B' 'C' 'D' 'E' 'F'
	return '0';
}

void int_putch(uns8 byte) 
{
							/* output one byte */
	while(!TXIF)			/* set when register is empty */
		continue;
	TXREG = byte;
}

void int_writeUNS8(uns8 v){
	uns8 i;
	i = v >>4;
	int_putch(int_hex2char(i));		//high 4-bit
	i = v & 0x0f;
	int_putch(int_hex2char(i));		//low 4-bit
}

void int_crlf(void){
	int_putch(0x0D);
	int_putch(0x0A);
}

#include "sci-lib.c"

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

	PORTB 	= 0b.0000.0000;
	ANSELB	= 0b.0000.0000; /* digital */
	TRISB 	= 0b.1111.1111; /* intput */	
	
	writeUNS8(0x66);
	crlf();

	cntNum 	= 0;
	/* http://saeedsolutions.blogspot.tw/2012/11/pic16f877-external-interrupt-code.html */
	INTEDG = 0; /* RB0 falling interrupt */
	INTE = 1;
	GIE	= 1;

	while(1);
}
