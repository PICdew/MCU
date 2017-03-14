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

#include "int16CXX.h"
#include "lcd-sci.h"
#include "sci-lib.c"

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


#include "common.c"
#include "max7219-2chip-spi-gpio.c"


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

	rxdata.cnt = 0;
	rxdata.b.stx = 0;
	rxdata.b.etx = 0;
	rxdata.b.dle = 0;
	rxdata.b.cmdfull = 0;
	cmddata.tail = 0;
	cmddata.head = 0;
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

void process_cmd(void);
void process_num(uns8 chipnumber,uns8 b0,uns8 b1,uns8 b2,uns8 b3);

void main( void)
{
	uns8	i,j;
	uns8	cmd,value1,value0;
	uns8	ret;
    if (TO == 1 && PD == 1 /* power up */)  {
    //   WARM_RESET:
         clearRAM(); // clear all RAM
    }
	TRISC	= 0xD0;			//TX,RX,SDI -input
	PIE1 	= 0;
	INTCON	= 0;
	init_comms();
	init_gpio();

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

	//enable rx interrupt
	RCIE 	= 1;
	PEIE    = 1;
	GIE		= 1;

    while(1){
        i++;
        if(i>8) i=0;
       j=i+1; 
    //ledchip1(max7219_reg_digit0,i);
    //ledchip1(max7219_reg_digit1,i);
    //ledchip1(max7219_reg_digit2,i);
    //ledchip1(max7219_reg_digit3,i);
    //ledchip1(max7219_reg_digit4,i);
    //ledchip1(max7219_reg_digit5,i);
    //ledchip1(max7219_reg_digit6,i);
    //ledchip1(max7219_reg_digit7,i);

    ledchip2(max7219_reg_digit0,j);
    ledchip2(max7219_reg_digit1,j);
    ledchip2(max7219_reg_digit2,j);
    ledchip2(max7219_reg_digit3,j);
    ledchip2(max7219_reg_digit4,j);
    ledchip2(max7219_reg_digit5,j);
    ledchip2(max7219_reg_digit6,j);
    ledchip2(max7219_reg_digit7,j);
			delay1s();
			process_cmd();
/*
			ret = getCmd(cmd,value1,value0);

			if(ret == 0){
				putch(cmd);
				putch(value1);
				putch(value0);
				crlf();
			}
*/
    }
}

/*
#define CHIP1NUB    	0x60
#define CHIP2NUB    	0x61
#define CHIP1REG    	0x62
#define CHIP2REG    	0x63

#define NUM_DP			0xA
#define NUM_CLEAR       0xB
#define NUM_NULL        0xF
*/

void process_cmd(void){

	uns8 ret,cmd,b3,b2,b1,b0;

	ret=getCmd(cmd,b0,b1,b2,b3);
	if(ret == 1){
		if(cmd == CHIP1NUB){
			process_num(1,b0,b1,b2,b3);

		}else if(cmd == CHIP2NUB){
			process_num(2,b0,b1,b2,b3);

		}else if(cmd == CHIP1REG){
			//ledchip(1,b0,b1);

		}else if(cmd == CHIP2REG){
			//ledchip(2,b0,b1);
		}
	}
}

void send_chip_num(uns8 chipnumber,uns8 num,uns8 value){
	uns8 data=NUM_NULL;

	if( (value >=0) && (value < 0xa)){
		data = value;

	}else if(value == NUM_DP){
		data = 0x80;

	}else if(value == NUM_CLEAR){
		data = MAX7219_CLEAR_LED;

	}else{
		return;
	}

	if( chipnumber <3){
		ledchip(chipnumber,num,data);
	}
}

void process_num(uns8 chipnumber,uns8 b0,uns8 b1,uns8 b2,uns8 b3){
	uns8 n0,n1,n2,n3;

	n0 = b0 & 0xf;
	n1 = (b0 & 0xf0) >> 4;
	n2 = b1 & 0xf;
	n3 = (b1 & 0xf0) >> 4;
	send_chip_num(chipnumber,max7219_reg_digit0,n0);
	send_chip_num(chipnumber,max7219_reg_digit1,n1);
	send_chip_num(chipnumber,max7219_reg_digit2,n2);
	send_chip_num(chipnumber,max7219_reg_digit3,n3);

	if( chipnumber == 1 ){
		n0 = b2 & 0xf;
		n1 = (b2 & 0xf0) >> 4;
		n2 = b3 & 0xf;
		n3 = (b3 & 0xf0) >> 4;
		send_chip_num(chipnumber,max7219_reg_digit4,n0);
		send_chip_num(chipnumber,max7219_reg_digit5,n1);
		send_chip_num(chipnumber,max7219_reg_digit6,n2);
		send_chip_num(chipnumber,max7219_reg_digit7,n3);
	}
}