
/*
 APP STATE MACHINES
*/

enum { APP_BOOTING=2, APP_ENTRY=3, APP_SET=4, APP_START=5, APP_STOP=6};

#define Delay_100MS  (100)
#define Delay_1000MS  (1000)

#define startTimer2(c)		\
	timer2L		= (c)%256;	\
	timer2H		= (c)/256;	\
	timeout2	= 0;

void initapp(void){
	startTimer2(Delay_100MS);
	state2 = APP_BOOTING;
	LED2Value = 0;
}

void app_entry(){
	uns8 i,key;

	if( (keyState ==1 ) && (keyEvent == 1) ){
		keyEvent = 0;
		writeUNS8Bit(keyValue);
		crlf();

		if( keyValue == K_START ) LED2Value=0;
		if( keyValue == K_SET ) LED2Value=1;
		if( keyValue == K_UP ) LED2Value=2;
		if( keyValue == K_DOWN ) LED2Value=3;
		if( keyValue == K_LEFT ) LED2Value=4;
		if( keyValue == K_RIGHT ) LED2Value=5;

		setCmdChip1(max7219_reg_digit1,LED2Value);
	}

	if(timeout2){

		setCmdChip1(max7219_reg_digit0,LEDCnt);

		LEDCnt++;

		if(LEDCnt>9) LEDCnt=0;

		startTimer2(Delay_1000MS);
	}
}

void fsm2( void)
{
	uns8 i=0;
	switch(state2){
		case APP_BOOTING:
			if(timeout2){
				state2	= APP_ENTRY;
			}
			break;

		case APP_ENTRY:
			app_entry();
			break;

		case APP_SET:
			break;

		case APP_START:
			break;

		case APP_STOP:
			break;
	}
}