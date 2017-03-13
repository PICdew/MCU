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
	rxdata.b.task = 0;
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
#define CLEAR0_ALL		0x40
#define CLEAR0_N80		0x41
#define CLEAR0_N81		0x42
#define CLEAR0_N0    	0x43
#define CLEAR0_N1    	0x44
#define CLEAR0_N2    	0x45
#define CLEAR0_N3    	0x46

#define CLEAR_ALL		0x50
#define CLEAR_N80		0x51
#define CLEAR_N81		0x52
#define CLEAR_N0    	0x53
#define CLEAR_N1    	0x54
#define CLEAR_N2    	0x55
#define CLEAR_N3    	0x56

#define SET_N0      	0x60
#define SET_N1      	0x61
#define SET_N2      	0x62
#define SET_N3      	0x63

#define SET_N80_0       0x70
#define SET_N80_1       0x71
#define SET_N80_2       0x72
#define SET_N80_3      	0x73
#define SET_N80_4     	0x74
#define SET_N80_5     	0x75
#define SET_N80_6     	0x76
#define SET_N80_7     	0x77

#define SET_N81_0     	0x78
#define SET_N81_1     	0x79
#define SET_N81_2     	0x7a
#define SET_N81_3     	0x7b
*/

//MAX7219_CLEAR_LED

void process_cmd(void){

	uns8 ret,cmd,value2,value1,value0;

	ret=getCmd(cmd,value2,value1,value0);
	if(ret == 1){
		if(cmd == CLEAR0_ALL){
			ledclear1_all0();
			ledclear2_all0();

		}else if(cmd == CLEAR_ALL){
			ledclear1();
			ledclear2();

		}else if(cmd == CLEAR0_N80){
			ledclear1_all0();

		}else if(cmd == CLEAR_N80){
			ledclear1();			

		}else if(cmd == CLEAR0_N81){
			ledclear2_all0();

		}else if(cmd == CLEAR_N81){
			ledclear2();	

		}else if(cmd == SET_N80_0){
		    ledchip1(max7219_reg_digit0,value0);

		}else if(cmd == SET_N80_1){
		    ledchip1(max7219_reg_digit1,value0);		    

		}else if(cmd == SET_N80_2){
		    ledchip1(max7219_reg_digit2,value0);		    

		}else if(cmd == SET_N80_3){
		    ledchip1(max7219_reg_digit3,value0);		    

		}else if(cmd == SET_N80_4){
		    ledchip1(max7219_reg_digit4,value0);		    

		}else if(cmd == SET_N80_5){
		    ledchip1(max7219_reg_digit5,value0);		    

		}else if(cmd == SET_N80_6){
		    ledchip1(max7219_reg_digit6,value0);		    

		}else if(cmd == SET_N80_7){
		    ledchip1(max7219_reg_digit7,value0);		    

		}else if(cmd == SET_N81_0){
		    ledchip2(max7219_reg_digit0,value0);		    

		}else if(cmd == SET_N81_1){
		    ledchip2(max7219_reg_digit1,value0);

		}else if(cmd == SET_N81_2){
		    ledchip2(max7219_reg_digit2,value0);

		}else if(cmd == SET_N81_3){
		    ledchip2(max7219_reg_digit3,value0);
		    		    		    
		}else if(cmd == SET_N0){
    		ledchip1(max7219_reg_digit0,value0);
    		ledchip1(max7219_reg_digit1,value1);
    		ledchip1(max7219_reg_digit2,value2);

		}else if(cmd == CLEAR0_N0){
    		ledchip1(max7219_reg_digit0,0);
    		ledchip1(max7219_reg_digit1,0);
    		ledchip1(max7219_reg_digit2,0);

		}else if(cmd == CLEAR_N0){
    		ledchip1(max7219_reg_digit0,MAX7219_CLEAR_LED);
    		ledchip1(max7219_reg_digit1,MAX7219_CLEAR_LED);
    		ledchip1(max7219_reg_digit2,MAX7219_CLEAR_LED);    		

		}else if(cmd == SET_N1){
    		ledchip1(max7219_reg_digit3,value0);
    		ledchip1(max7219_reg_digit4,value1);

		}else if(cmd == CLEAR0_N1){
    		ledchip1(max7219_reg_digit3,0);
    		ledchip1(max7219_reg_digit4,0);

		}else if(cmd == CLEAR_N1){
    		ledchip1(max7219_reg_digit3,MAX7219_CLEAR_LED);
    		ledchip1(max7219_reg_digit4,MAX7219_CLEAR_LED);

		}else if(cmd == SET_N2){
    		ledchip1(max7219_reg_digit5,value0);
    		ledchip1(max7219_reg_digit6,value1);
    		ledchip1(max7219_reg_digit7,value2);

		}else if(cmd == CLEAR0_N2){    		
    		ledchip1(max7219_reg_digit5,0);
    		ledchip1(max7219_reg_digit6,0);
    		ledchip1(max7219_reg_digit7,0);

		}else if(cmd == CLEAR_N2){    		
    		ledchip1(max7219_reg_digit5,MAX7219_CLEAR_LED);
    		ledchip1(max7219_reg_digit6,MAX7219_CLEAR_LED);
    		ledchip1(max7219_reg_digit7,MAX7219_CLEAR_LED);

		}else if(cmd == SET_N3){
    		ledchip2(max7219_reg_digit0,value0);
    		ledchip2(max7219_reg_digit1,value1);
    		ledchip2(max7219_reg_digit2,value2);

		}else if(cmd == CLEAR0_N3){    		
    		ledchip2(max7219_reg_digit0,0);
    		ledchip2(max7219_reg_digit1,0);
    		ledchip2(max7219_reg_digit2,0);
    		ledchip2(max7219_reg_digit3,0);    		

		}else if(cmd == CLEAR_N3){    		
    		ledchip2(max7219_reg_digit0,MAX7219_CLEAR_LED);
    		ledchip2(max7219_reg_digit1,MAX7219_CLEAR_LED);
    		ledchip2(max7219_reg_digit2,MAX7219_CLEAR_LED);
    		ledchip2(max7219_reg_digit3,MAX7219_CLEAR_LED);    		
		}
	}
}
