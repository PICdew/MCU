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

#define load_0()\
	PORTC.1 = 0;\
	PORTC.2 = 0 ;

#define MAX7219_DIGTAL_MODE

#ifdef MAX7219_DIGTAL_MODE
#define MAX7219_CLEAR_LED	0x0f
#else
#define MAX7219_CLEAR_LED	0x00
#endif

//#ifndef MAX7219_DIGTAL_MODE
const uns8 var2led[10] = {
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
};
//#endif


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

void setCmdChip1(uns8 cmd,uns8 value){
	MAX7219_LOAD1_PIN = 0;
#asm
	nop;
	nop;
#endasm	
	send_spi(cmd);
	send_spi(value);
#asm
	nop;
	nop;
#endasm		
	MAX7219_LOAD1_PIN = 1;	/* max7219 , load rising active*/
#asm
	nop;
	nop;
#endasm		
	MAX7219_LOAD1_PIN = 0;
}

void setCmdChip2(uns8 cmd,uns8 value){
	MAX7219_LOAD2_PIN = 0;
#asm
	nop;
	nop;
#endasm	
	send_spi(cmd);
	send_spi(value);
#asm
	nop;
	nop;
#endasm		
	MAX7219_LOAD2_PIN = 1;	/* max7219 , load rising active*/
#asm
	nop;
	nop;
#endasm	
	MAX7219_LOAD2_PIN = 0;
}

void setCmdChipAll(uns8 cmd,uns8 value,uns8 cmd2,uns8 value2){
	MAX7219_LOAD2_PIN = 1;
	MAX7219_LOAD1_PIN = 1;
#asm
	nop;
	nop;
#endasm	
	send_spi(cmd2);
	send_spi(value2);
	send_spi(cmd);
	send_spi(value);
#asm
	nop;
	nop;
#endasm		
	MAX7219_LOAD2_PIN = 0;	/* max7219 , load rising active*/
	MAX7219_LOAD1_PIN = 0;
#asm
	nop;
	nop;
	nop;
	nop;
	nop;				
#endasm	
	MAX7219_LOAD2_PIN = 1;
	MAX7219_LOAD1_PIN = 1;
#asm
	nop;
	nop;
	nop;
	nop;
	nop;			
#endasm
}

void setCmdChipAllOne(uns8 cmd,uns8 value){
	MAX7219_LOAD2_PIN = 0;
	MAX7219_LOAD1_PIN = 0;
#asm
	nop;
	nop;
#endasm	
	send_spi(cmd);
	send_spi(value);
	send_spi(cmd);
	send_spi(value);	
#asm
	nop;
	nop;
#endasm		
	MAX7219_LOAD2_PIN = 0;	/* max7219 , load rising active*/
	MAX7219_LOAD1_PIN = 0;
#asm
	nop;
	nop;
	nop;
	nop;
	nop;				
#endasm	
	MAX7219_LOAD2_PIN = 1;
	MAX7219_LOAD1_PIN = 1;
#asm
	nop;
	nop;
	nop;
	nop;
	nop;			
#endasm
}

void setLED(){
	
}

void setCmdChipAllTwo(uns8 cmd,uns8 value){
	MAX7219_LOAD2_PIN = 0;
	MAX7219_LOAD1_PIN = 0;
#asm
	nop;
	nop;
#endasm	
	send_spi(cmd);
	send_spi(value);
	send_spi(0x0);
	send_spi(0x0);
#asm
	nop;
	nop;
#endasm		
	MAX7219_LOAD2_PIN = 0;	/* max7219 , load rising active*/
	MAX7219_LOAD1_PIN = 0;
#asm
	nop;
	nop;
	nop;
	nop;
	nop;				
#endasm	
	MAX7219_LOAD2_PIN = 1;
	MAX7219_LOAD1_PIN = 1;
#asm
	nop;
	nop;
	nop;
	nop;
	nop;			
#endasm
}


void setLED(uns8 cmd,uns8 value){
	setCmdChipAllOne(cmd,value);
	//setCmdChipAllTwo(cmd,value);
	//setCmdChipAll(cmd,value,cmd,value);
	//setCmdChip1(cmd,value);
	//setCmdChip2(cmd,value);
}

//cmd1,value1 => for first max7219
//cmd2,value2 => for second max7219
void set2LED(uns8 cmd1,uns8 value1,uns8 cmd2,uns8 value2){
	setCmdChipAllOne(cmd1,value1);
	//setCmdChipAllTwo(cmd2,value2);	
	//setCmdChipAll(cmd1,value1,cmd2,value2);
	//setCmdChip1(cmd1,value1);
	//setCmdChip2(cmd2,value2);
}

void clear(void){
	setLED(max7219_reg_digit0,MAX7219_CLEAR_LED);
	setLED(max7219_reg_digit1,MAX7219_CLEAR_LED);
	setLED(max7219_reg_digit2,MAX7219_CLEAR_LED);
	setLED(max7219_reg_digit3,MAX7219_CLEAR_LED);
	setLED(max7219_reg_digit4,MAX7219_CLEAR_LED);
	setLED(max7219_reg_digit5,MAX7219_CLEAR_LED);
	setLED(max7219_reg_digit6,MAX7219_CLEAR_LED);
	setLED(max7219_reg_digit7,MAX7219_CLEAR_LED);
}
void setIntensity(uns8 intensity){
	setLED(max7219_reg_intensity, intensity);
}

void max7129_init(void){
	uns8 ch,v;
	uns8 i;

	setLED(max7219_reg_scanLimit, 0x07);
#ifdef MAX7219_DIGTAL_MODE	
	setLED(max7219_reg_decodeMode, 0xff);  // Code B decode for digits 7–0	
#else
	setLED(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
#endif
	setLED(max7219_reg_shutdown, 0x01);    // not in shutdown mode
	setLED(max7219_reg_displayTest, 0x00); // no display test

	// empty registers, turn all LEDs off
	clear();
	
	//setIntensity(0x0f);    // the first 0x0f is the value you can set	
	setIntensity(0x08);    // the first 0x0f is the value you can set			
}