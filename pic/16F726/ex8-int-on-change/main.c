#include "16f726.h"
#include "c_16f726.h"

#pragma config = ( HS_ & WDTDIS_ & PWRTEN_ & MCLRDIS_ & UNPROTECT_ & BOREN_ & BORV25_ & PLLDIS_ & DEBUGDIS_ )

#pragma wideConstData p	

#pragma bit TRISC6   @ 0x87.6
#pragma bit TRISC7   @ 0x87.7

/* global variables */
#define CNT1S_V	5
uns8	cntNum1;
uns8 	cntNum2;
uns8 	valportb;
bit    portb4_0;
bit    portb4_1;
bit    portb5_0;
bit    portb5_1;

void int_writeUNS8(uns8 v);
void int_crlf(void);
void int_putch(uns8 byte);

#include "int16CXX.h"
#pragma origin 4
interrupt int_server(void)
{
  int_save_registers    // W, STATUS (and PCLATH)
	if(RBIF){
		RBIF	= 0;
		valportb = PORTB;

		if( valportb & 0x10)
			portb4_0 = 1;
		else
			portb4_0 = 0;

		if( valportb & 0x20)
			portb5_0 = 1;
		else
			portb5_0 = 0;

		//RB4 falling
		if( (portb4_1==1) && (portb4_0==0) )
			cntNum1++;

		//RB5 falling
		if( (portb5_1==1) && (portb5_0==0) )
			cntNum2++;

		int_writeUNS8(cntNum1);
		int_putch(',');
		int_writeUNS8(cntNum2);		
		int_crlf();

		portb4_1 = portb4_0;
		portb5_1 = portb5_0;
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
	uns8	i,j;
    if (TO == 1 && PD == 1 /* power up */)  {
    //   WARM_RESET:
         clearRAM(); // clear all RAM
    }
	TRISC	= 0xD0;			//TX,RX,SDI -input
	PIE1 	= 0;
	INTCON	= 0;
	init_comms();

	/*
	RB4 IR0 Sensor
	RB5 IR1 Sensor
	*/
	PORTB 	= 0b.0000.0000;
	ANSELB	= 0b.0000.0000; /* digital */
	TRISB 	= 0b.0011.0000; /* intput */
	IOCB	= 0b.0011.0000;	
	
	cntNum1 = 0;
	cntNum2	= 0;
	portb4_0 = 1;
	portb4_1 = 1;
	portb5_0 = 1;
	portb5_1 = 1;

	//delay for hardware stable
	for(j=0; j<255; j++){
		for(i=0; i<255; i++){
			#asm
			nop
			nop
			nop
			#endasm
		}
	}
	writeUNS8(0x66);
	crlf();

	RBIE	= 1;
	GIE	= 1;

	while(1);
}
