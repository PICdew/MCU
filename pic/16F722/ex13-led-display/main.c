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

//OPTION  = 0x84 ;  1:32 prescaler ,20M/4/32 = 156.25K  = 6.4us 
#define	DELAY_100U		(unsigned char)(256 -(100.0/6.4) +2)  	

#include "int16CXX.h"
#include "lcd-sci.h"
#include "sci-lib.c"
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
void process_num(uns8 chipnumber,uns8 b0,uns8 b1,uns8 b2,uns8 b3,uns8 dp);

void main( void)
{
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
		process_cmd();
    }
}

void process_cmd(void){
	uns8 ret,cmd,b0,b1,b2,b3,dp;

	ret=getCmd();
	if(ret == 1){
		cmd = cmdret[0];
		b0  = cmdret[1];
		b1  = cmdret[2];
		b2  = cmdret[3];
		b3  = cmdret[4];
		dp  = cmdret[5];

		if(cmd == CHIP1NUM){
			process_num(1,b0,b1,b2,b3,dp);

		}else if(cmd == CHIP2NUM){
			dp = b2;
			process_num(2,b0,b1,b2,b3,dp);

		}else if(cmd == CHIP1REG){
			ledchip(1,b0,b1);

		}else if(cmd == CHIP2REG){
			ledchip(2,b0,b1);
		}
	}
}

void send_chip_num(uns8 chipnumber,uns8 num,uns8 value,uns8 dp){
	uns8 data;

	if( (value >=0) && (value < 0xa)){
		data = value;
		if(dp) data |= 0x80;

	}else if( dp && (value==NUM_CLEAR) ){
		data = 0x8F;

	}else if(value == NUM_CLEAR){
		data = MAX7219_CLEAR_LED;

	}else{
		return;
	}

	if( chipnumber <3){
		ledchip(chipnumber,num,data);
	}
}

void process_num(uns8 chipnumber,uns8 b0,uns8 b1,uns8 b2,uns8 b3,uns8 dp){
	uns8 n0,n1,n2,n3;

	n0 = b0 & 0xf;
	n1 = (b0 & 0xf0) >> 4;
	n2 = b1 & 0xf;
	n3 = (b1 & 0xf0) >> 4;

	send_chip_num(chipnumber,max7219_reg_digit0,n0,(dp & NUM0_DP));
	send_chip_num(chipnumber,max7219_reg_digit1,n1,(dp & NUM1_DP));
	send_chip_num(chipnumber,max7219_reg_digit2,n2,(dp & NUM2_DP));
	send_chip_num(chipnumber,max7219_reg_digit3,n3,(dp & NUM3_DP));

	if( chipnumber == 1 ){
		n0 = b2 & 0xf;
		n1 = (b2 & 0xf0) >> 4;
		n2 = b3 & 0xf;
		n3 = (b3 & 0xf0) >> 4;
		send_chip_num(chipnumber,max7219_reg_digit4,n0,(dp & NUM4_DP));
		send_chip_num(chipnumber,max7219_reg_digit5,n1,(dp & NUM5_DP));
		send_chip_num(chipnumber,max7219_reg_digit6,n2,(dp & NUM6_DP));
		send_chip_num(chipnumber,max7219_reg_digit7,n3,(dp & NUM7_DP));
	}
}