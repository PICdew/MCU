#include "16f726.h"
#include "c_16f726.h"

#pragma config = ( HS_ & WDTDIS_ & PWRTEN_ & MCLREN_ & UNPROTECT_ & BOREN_ & BORV25_ & PLLDIS_ & DEBUGDIS_ )

#pragma bit TRISC6   @ 0x87.6
#pragma bit TRISC7   @ 0x87.7
#pragma bit MAX7219_LOAD_PIN	@ PORTC.2

#pragma char KEY_PORT   @ PORTA

#pragma wideConstData p	

/* global variables */
#define NText 	12
const char text[NText] = "Hello world!";

//OPTION  = 0x84 ;  1:32 prescaler ,20M/4/32 = 156.25K  = 6.4us 
#define	DELAY_100U		(unsigned char)(256 -(100.0/6.4) +2)  	

enum { KEY_START=1, KEY_SET=2, KEY_UP=3,  KEY_DOWN=4, KEY_LEFT=5, KEY_RIGHT=6};

typedef union{
	char v;
	struct {
		char    light_en:1;
		char    light:1;
		char    app_run_finish:1;
	}b;
}VARStatusXX;	

typedef struct {
	uns8 pitchLED[3];
	uns8 pitchKeyPos;
	uns8 percentLED[2];
	uns8 scoreLED[3];
	uns8 scoreKeyPos;
	uns8 timeLED[4];
	uns8 timeKeyPos;
	uns8 dpLED[2];

	//app run
	uns16 pitchValue;
	uns16 pitchCnt;
	uns8 percentValue;
	uns16 scoreValue;
	uns16 scoreCnt;
	uns8 timeValue[2]; //[0]:sec, [1]:min
	uns8 timeCnt[2];

	//for led light
	VARStatusXX st;

	//for mode finish;
} VARLEDxx;

#include "lcd-sci.h"

#pragma rambank 0  // select default bank for the next definitions
/* key value */
uns8 previousTMR0;
uns8 subClock;
uns8 timer1, timer2L, timer2H, timer3, timer4L, timer4H, timer5L, timer5H, timer6L, timer6H, timer7L, timer7H;
bit timeout1, timeout2, timeout3, timeout4, timeout5, timeout6, timeout7;  //timeout5 key3S
uns8 state1, state2, state3, state3_1;
bit keyState;

bit keyEvent;
bit Key3S;
uns8 keyValue;
uns8 keyEventValue;
uns8 LEDCnt;
uns8 LED2Value;

bit bitScoreSensor, bitTakeBallSensor;

uns8	cntScoreSensor;
uns8 	cntTakeBallSensor;
bit    	portb4_0;
bit    	portb4_1;
bit    	portb5_0;
bit    	portb5_1;

#pragma rambank 1  // select default bank for the next definitions
VARLEDxx varled;
uns8 vxx[3];

#pragma rambank 2  // select default bank for the next definitions

#include "int16CXX.h"
#pragma origin 4
interrupt int_server(void)
{
  	int_save_registers    // W, STATUS (and PCLATH)
  	uns8 valportb,sv_FSR;
  	sv_FSR = FSR;

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
			cntScoreSensor++;

		//RB5 rising 0->1
		if( (portb5_1==0) && (portb5_0==1) )
			cntTakeBallSensor++;

		portb4_1 = portb4_0;
		portb5_1 = portb5_0;
	}
	FSR = sv_FSR;
   	int_restore_registers // W, STATUS (and PCLATH)
}

#pragma codepage 1   // select default codepage for following functions
#include "sci-lib.c"
#pragma codepage 2
#include "fsm.c"
#pragma codepage 3
#include "key.c"
#include "common.c"
#pragma codepage 0

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

	//SSPCON	= 0b.0010.0000;
	/*
	bit7   WCOL:  0
	bit6   SSPOV: 0 ,no receive overflow indicator bit(無使用SDI)
	bit5   SSPEN: 1 ,enable SCK,SDO,SDI
	bit4   CPK: 0, idle state for clk is a low level
	bit0-3 SSPM<3:0>: 0000 SPI Master, clk = fosc/4, 20MHZ/4=5Mhz

	max7219 clk 上升取DI，無動作clk為low
	16f726 => master, CKP = 0,CKE = 1
	*/
	SSPCON	= 0b.0010.0001;	// 1.25MHz

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
	init_spi();
	init_inputkey();

	initkey();
	initapp();

	//OPTION  = 0x81 ;  1:4 prescaler ,20M/4/4=1.25M = 0.8u
    //          0x82 ;  1:8                            1.6u
    //          0x83 ;  1:16            20M/4/16 = 312.5K = 3.2us
    //          0x84 ;  1:32            20M/4/32 = 156.25K  = 6.4us   
    //          0x85 ;  1:64            20M/4/64 =  78.125K = 12.8us
    //          0x86 ;  1:128          20M/4/128 =   39.0625K = 25.6us

								//  0100 1:32 , 20MHz , 10M/4/32=> 6.4us, 6.4*156 = 0.9984ms	
								//  0011 1:16 , 20MHz , 10M/4/16=> 3.2us, 3.2*156 = 0.499ms	
	OPTION	= 0x84;		//	0011 1:16 , 10MHz , 10M/4/16=> 6.4us, 6.4u*156 = 0.998ms	

	/*
	RB4 IR0 Sensor
	RB5 IR1 Sensor
	*/
	PORTB 	= 0b.0000.0000;
	ANSELB	= 0b.0000.0000; /* digital */
	TRISB 	= 0b.0011.0000; /* intput */
	IOCB	= 0b.0011.0000;	


	init_varled();	

	bitScoreSensor = 0;
	bitTakeBallSensor = 0;

	cntScoreSensor = 0;
	cntTakeBallSensor = 0;
	portb4_0 = 1;
	portb4_1 = 1;
	portb5_0 = 1;
	portb5_1 = 1;
	delayms(500);
	RBIE	= 1;
	GIE		= 1;

	//while(1);

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
