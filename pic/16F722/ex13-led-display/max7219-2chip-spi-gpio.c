/*
max7219 reference source 
https://code.google.com/archive/p/arudino-maxmatrix-library/
https://github.com/t3chguy/arudino-maxmatrix-library
*/

#define MAX7219_CLEAR_LED	    0x0f

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

#define clk_delay()	\
	nop();			\
	nop();			\
	nop();			\
	nop();			\
	nop();			\
	nop();			\
	nop();

#define clk1_0()	\
	LEDSCK1 = 0;	\
	clk_delay();

#define clk1_1()	\
	LEDSCK1 = 1;	\
	clk_delay();

#define clk2_0()	\
	LEDSCK2 = 0;	\
	clk_delay();

#define clk2_1()	\
	LEDSCK2 = 1;	\
	clk_delay();	

#define data_0() \
	LEDSDO = 0;	\
	clk_delay();

#define data_1() \
	LEDSDO = 1;	\
	clk_delay();

void send1_spi(uns8 dataout){
	uns8 i,mask,j;
	for(i=0; i<8 ;i++){
		j=7-i;
		mask = 1 << j;
		clk1_0();
		if(dataout & mask){
			data_1();
		}else{
			data_0();
		}
		clk1_1();
	}
	clk1_0();
}

void send2_spi(uns8 dataout){
	uns8 i,mask,j;
	for(i=0; i<8 ;i++){
		j=7-i;
		mask = 1 << j;
		clk2_0();
		if(dataout & mask){
			data_1();
		}else{
			data_0();
		}
		clk2_1();
	}
	clk2_0();
}



void ledchip1(uns8 cmd,uns8 value){
	LEDLOAD1 = 0;
	send1_spi(cmd);
	send1_spi(value);
	LEDLOAD1 = 1;
	LEDLOAD1 = 0;
}

void ledchip2(uns8 cmd,uns8 value){
	LEDLOAD2 = 0;
	send2_spi(cmd);
	send2_spi(value);
	LEDLOAD2 = 1;
	LEDLOAD2 = 0;
}

void ledchip(uns8 chipnumber, uns8 cmd, uns8 value){
	if(chipnumber == 1){
		ledchip1(cmd,value);
	}else if(chipnumber == 2){
		ledchip2(cmd,value);
	}
}

void ledclear1(void){
	ledchip1(max7219_reg_digit0,MAX7219_CLEAR_LED);
	ledchip1(max7219_reg_digit1,MAX7219_CLEAR_LED);
	ledchip1(max7219_reg_digit2,MAX7219_CLEAR_LED);
	ledchip1(max7219_reg_digit3,MAX7219_CLEAR_LED);
	ledchip1(max7219_reg_digit4,MAX7219_CLEAR_LED);
	ledchip1(max7219_reg_digit5,MAX7219_CLEAR_LED);
	ledchip1(max7219_reg_digit6,MAX7219_CLEAR_LED);
	ledchip1(max7219_reg_digit7,MAX7219_CLEAR_LED);
}
void ledclear2(void){
	ledchip2(max7219_reg_digit0,MAX7219_CLEAR_LED);
	ledchip2(max7219_reg_digit1,MAX7219_CLEAR_LED);
	ledchip2(max7219_reg_digit2,MAX7219_CLEAR_LED);
	ledchip2(max7219_reg_digit3,MAX7219_CLEAR_LED);
	ledchip2(max7219_reg_digit4,MAX7219_CLEAR_LED);
	ledchip2(max7219_reg_digit5,MAX7219_CLEAR_LED);
	ledchip2(max7219_reg_digit6,MAX7219_CLEAR_LED);
	ledchip2(max7219_reg_digit7,MAX7219_CLEAR_LED);
}

void setIntensity1(uns8 intensity){
	ledchip1(max7219_reg_intensity, intensity);
}
void setIntensity2(uns8 intensity){
	ledchip2(max7219_reg_intensity, intensity);
}

void ledcmd1_init(){
	ledchip1(max7219_reg_scanLimit, 0x07);
	ledchip1(max7219_reg_shutdown, 0x01);    // not in shutdown mode
	ledchip1(max7219_reg_decodeMode, 0xff); 
	ledchip1(max7219_reg_displayTest, 0x00); // no display test
	ledclear1();

	//setIntensity1(0x0f);
	setIntensity1(0x08);
}

void ledcmd2_init(){
	ledchip2(max7219_reg_scanLimit, 0x07);
	ledchip2(max7219_reg_shutdown, 0x01);    // not in shutdown mode
	ledchip2(max7219_reg_decodeMode, 0xff); 
	ledchip2(max7219_reg_displayTest, 0x00); // no display test
	ledclear2();

	//setIntensity2(0x0f);
	setIntensity2(0x08);
}

void ledcmd_init(){
	ledcmd1_init();
	ledcmd2_init();
}
