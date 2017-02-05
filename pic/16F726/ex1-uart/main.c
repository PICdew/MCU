
#include "16f726.h"
#include "c_16f726.h"

#pragma config = ( HS_ & WDTDIS_ & PWRTEN_ & MCLRDIS_ & UNPROTECT_ & BOREN_ & BORV25_ & PLLDIS_ & DEBUGDIS_ )
#pragma bit TRISC6   @ 0x87.6
#pragma bit TRISC7   @ 0x87.7

/* global variables */
#define NText 12
const char text[NText] = "Hello world!";

void putch(unsigned char byte) 
{
	/* output one byte */
	while(!TXIF)	/* set when register is empty */
		continue;
	TXREG = byte;
}
unsigned char getch() {
	/* retrieve one byte */
	while(!RCIF)	/* set when register is not empty */
		continue;
	return RCREG;	
}
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
	
	for(i=0; i<NText; i++) putch(text[i]);	putch(0x0A);	putch(0x0D);
	for(i=0; i<NText; i++) putch(text[i]);  putch(0x0A);	putch(0x0D);
	for(i=0; i<NText; i++) putch(text[i]);  putch(0x0A);	putch(0x0D);
	while(1);
}
