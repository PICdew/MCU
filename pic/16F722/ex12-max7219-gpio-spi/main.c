#include "16f722.h"
#include "c_16f722.h"

#pragma config = ( HS_ & WDTDIS_ & PWRTEN_ & MCLREN_ & UNPROTECT_ & BOREN_ & BORV25_ & PLLDIS_ & DEBUGDIS_ )

#pragma bit TRISC6   @ 0x87.6
#pragma bit TRISC7   @ 0x87.7
#pragma bit LEDLOAD1	@ PORTC.2
#pragma bit LEDLOAD2	@ PORTC.1
#pragma bit LEDSCK1     @ PORTC.3
#pragma bit LEDSCK2     @ PORTC.4
#pragma bit LEDSDO      @ PORTC.5

#pragma char KEY_PORT   @ PORTA

#pragma wideConstData p	

/* global variables */
#define NText 	12
const char text[NText] = "Hello world!";

//OPTION  = 0x84 ;  1:32 prescaler ,20M/4/32 = 156.25K  = 6.4us 
#define	DELAY_100U		(unsigned char)(256 -(100.0/6.4) +2)  	

/* key value */
uns8 previousTMR0;
uns8 subClock;
uns8 timer1, timer2L, timer2H;
bit timeout1, timeout2;
uns8 state1, state2;
bit keyState;

bit keyEvent;
uns8 keyValue;
uns8 LEDCnt;
uns8 LED2Value;


enum { K_START = 0b.0000.0001, 
	   K_SET   = 0b.0000.0010, 
	   K_UP    = 0b.0000.0100, 
	   K_DOWN  = 0b.0000.1000,
	   K_LEFT  = 0b.0001.0000,
	   K_RIGHT = 0b.0010.0000 
	};

#include "sci-lib.c"
#include "common.c"
#include "max7219-2chip-spi-gpio.c"
#include "fsm.c"
#include "key.c"


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
void init_gpio(void){
    //RC1 chip2 load
    //RC2 chip1 load
    //RC3 chip1 sck
    //RC4 chip2 sck
    //RC5 chip1,chip2 sdo

	PORTC 	= 0b.0000.0000;
	TRISC 	= 0b.1100.0001; /* output */
}

/*
intput key

                               Up(RA2)
set(RA1)           Left(RA4)             Right(RA5)
	                           Down(RA3)
start(RA0)

RB4  IR sensing
RB5  IR sensing
*/

void init_inputkey(void){
	PORTA 	= 0b.0000.0000;
	ANSELA	= 0b.0000.0000; /* digital */
	TRISA 	= 0b.1111.1111; /* intput */	

	state1  = OPEN;
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
	init_gpio();
	init_inputkey();

	initkey();
	initapp();

	crlf();
	for(i=0; i<NText; i++) putch(text[i]);	crlf();
	for(i=0; i<NText; i++) putch(text[i]);  crlf();
	for(i=0; i<NText; i++) putch(text[i]);  crlf();
	crlf();

	//OPTION  = 0x81 ;  1:4 prescaler ,20M/4/4=1.25M = 0.8u
    //          0x82 ;  1:8                            1.6u
    //          0x83 ;  1:16            20M/4/16 = 312.5K = 3.2us
    //          0x84 ;  1:32            20M/4/32 = 156.25K  = 6.4us   
    //          0x85 ;  1:64            20M/4/64 =  78.125K = 12.8us
    //          0x86 ;  1:128          20M/4/128 =   39.0625K = 25.6us

								//  0100 1:32 , 20MHz , 10M/4/32=> 6.4us, 6.4*156 = 0.9984ms	
								//  0011 1:16 , 20MHz , 10M/4/16=> 3.2us, 3.2*156 = 0.499ms	
	OPTION	= 0x84;		//	0011 1:16 , 10MHz , 10M/4/16=> 6.4us, 6.4u*156 = 0.998ms	

	delayms(10);

	ledcmd_init();

    while(1){
        i++;
        if(i>8) i=0;
       j=i+1; 
    ledchip1(max7219_reg_digit0,i);
    ledchip1(max7219_reg_digit1,i);
    ledchip1(max7219_reg_digit2,i);
    ledchip1(max7219_reg_digit3,i);
    ledchip1(max7219_reg_digit4,i);
    ledchip1(max7219_reg_digit5,i);
    ledchip1(max7219_reg_digit6,i);
    ledchip1(max7219_reg_digit7,i);

    ledchip2(max7219_reg_digit0,j);
    ledchip2(max7219_reg_digit1,j);
    ledchip2(max7219_reg_digit2,j);
    ledchip2(max7219_reg_digit3,j);
    ledchip2(max7219_reg_digit4,j);
    ledchip2(max7219_reg_digit5,j);
    ledchip2(max7219_reg_digit6,j);
    ledchip2(max7219_reg_digit7,j);
			delay1s();
    }

	while(1){
		timerTick();
        /* Not more than 1 (2) millisec.
           between each call to timerTick() */

        /* .. sample analog channels */
        /* .. do other IO communication */
        /* .. do global processing and testing */
		fsm1();
		fsm2();
	}
}
