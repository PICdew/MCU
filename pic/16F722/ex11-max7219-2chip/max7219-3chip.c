/*
max7219 reference source 
https://code.google.com/archive/p/arudino-maxmatrix-library/
https://github.com/t3chguy/arudino-maxmatrix-library
*/

/*

  **A**
  *   *
  F   B
  *   *
  **G**
  *   *
  E   C
  *   *
  **D**  *DP
  
  D7 6543210
  DP ABCDEFG
0 0  1111110
1 0  0110000
2 0  1101101
3 0  1111001
4 0  0110011
5 0  1011011
6 0  1011111
7 0  1110000
8 0  1111111
9 0  1111011 

	0b.0111.1110,
	0b.0011.0000,
	0b.0110.1101,
	0b.0111.1001,
	0b.0011.0011,
	0b.0101.1011,
	0b.0101.1111,
	0b.0111.0000,
	0b.0111.1111,
	0b.0111.1011 

	0b.1000.0001,
	0b.1100.1111,
	0b.1001.0010,
	0b.1000.0110,
	0b.1100.1100,
	0b.1010.0100,
	0b.1010.0000,
	0b.1000.1111,
	0b.1000.0000,
	0b.1000.0100 	
*/

#define send_spi(C)\
	SSPBUF = C;\
	SSPIF = 0 ;\
    while ( !SSPIF );

/*
#define load_0()\
	PORTC.1 = 0;\
	PORTC.2 = 0;

#define load_1()\
	PORTC.1 = 1;\
	PORTC.2 = 1;	
*/

#define load_0()\
	PORTC.2 = 0;

#define load_1()\
	PORTC.2 = 1;

#define MAX7219_DIGTAL_MODE

#ifdef MAX7219_DIGTAL_MODE
#define MAX7219_CLEAR_LED	0x0f
#else
#define MAX7219_CLEAR_LED	0x00
#endif

#define max7219_reg_noop        0x00
#define max7219_reg_digit0      0x01
#define max7219_reg_digit1      0x02
#define max7219_reg_digit2      0x03
#define max7219_reg_digit3      0x04
#define max7219_reg_digit4      0x05
#define max7219_reg_digit5      0x06
#define max7219_reg_digit6      0x07
#define max7219_reg_digit7      0x08
#define max7219_reg_decodeMode  0x09
#define max7219_reg_intensity   0x0a
#define max7219_reg_scanLimit   0x0b
#define max7219_reg_shutdown    0x0c
#define max7219_reg_displayTest 0x0f


void ledchip1(uns8 cmd,uns8 value){
	PORTC.2 = 0;
	send_spi(cmd);
	send_spi(value);
	PORTC.2 = 1;
#asm
	nop;
	nop;
	nop;
#endasm	
	PORTC.2 = 0;
}

void ledchip2(uns8 cmd,uns8 value){
	PORTC.1 = 0;
	send_spi(cmd);
	send_spi(value);
	send_spi(0x0);
	send_spi(0x0);
	PORTC.1 = 1;
#asm
	nop;
	nop;
	nop;
#endasm		
	PORTC.1 = 0;
}

void ledcmd(uns8 cmd,uns8 value){
	ledchip1(cmd,value);
	ledchip2(cmd,value);
}

void ledcmd_init(){
	ledcmd(max7219_reg_shutdown, 0x01);    // not in shutdown mode
	ledcmd(max7219_reg_decodeMode, 0xff); 
}