
#include "16f726.h"
#include "c_16f726.h"

#pragma config = ( HS_ & WDTDIS_ & PWRTEN_ & MCLRDIS_ & UNPROTECT_ & BOREN_ & BORV25_ & PLLDIS_ & DEBUGDIS_ )
#pragma bit TRISC6   @ 0x87.6
#pragma bit TRISC7   @ 0x87.7


/* global variables */
#define NText 	12
const char text[NText] = "Hello world!";
#define NLed 	4
const uns8 led[NLed]={0b.0000.1110,0b.0000.1101,0b.0000.1011,0b.0000.0111};


//OPTION  = 0x81 ;  1:4 prescaler ,10M/4/2=1.25M = 0.8u
#define	DELAY_100U		(unsigned char)(256 -(100.0/0.8) +2)
#define delay_us(C)\
	TMR0 = C;\
	T0IF = 0 ;\
    while ( !T0IF );

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

void init_led(void){
	/* low active, B0-B3 */
	PORTB 	= 0b.0000.0000;
	ANSELB 	= 0b.0000.0000;	/* ananlog input off,digital i/o enable */
	TRISB 	= 0b.0000.0000; /* output */
}

void delay1s(void){
	uns8 i,j;
	//1s
	for(j=0; j<100; j++){
		//10ms
		for(i=0; i<100; i++){
			delay_us(DELAY_100U);	//0.1ms
		}
	}
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
	init_led();

	OPTION  = 0x81;  //1:4 prescaler ,10M/4/4=1.25M = 0.8u

	crlf();
	for(i=0; i<NText; i++) putch(text[i]);	putch(0x0A);	putch(0x0D);
	for(i=0; i<NText; i++) putch(text[i]);  putch(0x0A);	putch(0x0D);
	for(i=0; i<NText; i++) putch(text[i]);  putch(0x0A);	putch(0x0D);
	for(i=0; i<NLed; i++){
		writeUNS8Bit(led[i]);
		crlf();
	}
	crlf();	
	while(1){
		for(i=0; i<NLed; i++){
			PORTB = led[i];
			writeUNS8Bit(led[i]);
			crlf();
			delay1s();
		}
	}
}
