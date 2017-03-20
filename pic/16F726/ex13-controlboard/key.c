
/*
 KEY STATE MACHINES AND SOFTWARE TIMERS
*/


enum { OPEN=1, PRESSING=2, PRESSED=3,  OPENING=4};

enum { K_START_BIT = 0b.0000.0001, 
	   K_SET_BIT   = 0b.0000.0010, 
	   K_UP_BIT    = 0b.0000.0100, 
	   K_DOWN_BIT  = 0b.0000.1000,
	   K_LEFT_BIT  = 0b.0001.0000,
	   K_RIGHT_BIT = 0b.0010.0000 
	};

#define startTimer1(c)		\
	timer1 		= c;		\
	timeout1	= 0;

#define DEF_3S  (2000)
#define startTimer5(c)		\
	timer5L		= (c)%256;	\
	timer5H		= (c)/256;	\
	timeout5	= 0;	


void initkey(void){
    timeout1	= 1;
    timeout2 	= 1;
    timeout3    = 1;
    timeout4    = 1;
	keyState	= 0;
	keyEvent 	= 0;
	state1  	= OPEN;
	LEDCnt		= 0;
}

uns8 getKeyValue(){
	uns8 key= KEY_PORT;

	if( (key & K_START_BIT) == 0 ) 	return KEY_START;
	if( (key & K_SET_BIT) == 0 ) 	return KEY_SET;
	if( (key & K_UP_BIT) == 0 ) 	return KEY_UP;
	if( (key & K_DOWN_BIT) == 0 ) 	return KEY_DOWN;
	if( (key & K_LEFT_BIT) == 0 ) 	return KEY_LEFT;
	if( (key & K_RIGHT_BIT) == 0 ) 	return KEY_RIGHT;
	return 0;
}


void timerTick()
/*
 - decrements active timers
 - 20 MHz oscillator
 - period: 1 millisec. (prescaler divide by 32)
 - subClock: 0 .. 156
*/
{
    char sample, incr;

    sample = TMR0;  /* sampling the timer */
    incr = sample - previousTMR0;
    previousTMR0 = sample;

    subClock -= incr;
	
	if ( !Carry)  {
    /* new 1 millisec. tick */
		subClock += 156;
		
		if(!timeout1){
			timer1--;
			if(timer1 == 255)
				timeout1 = 1;
		}

		if(!timeout2){
			if(--timer2L == 255){
				if(--timer2H == 255)
					timeout2 = 1;
			}
		}

		if(!timeout3){
			timer3--;
			if(timer3 == 255)
				timeout3 = 1;
		}


		if(!timeout4){
			if(--timer4L == 255){
				if(--timer4H == 255)
					timeout4 = 1;
			}
		}

		if(!timeout5){
			if(--timer5L == 255){
				if(--timer5H == 255)
					timeout5 = 1;
			}
		}		
	}
}

void fsm1(){
	uns8 key;
	key = getKeyValue();

	switch(state1){	/* state machine 1 */
		case OPEN:
			if(key){
				startTimer1(50);
				state1 = PRESSING;
				keyValue = key;
			}
			break;
		
		case PRESSING:
			if(key != keyValue)
				state1 = OPEN;
			else if(timeout1){
				Key3S = 0;
				keyState = 1;
				state1 = PRESSED;				
				startTimer5(DEF_3S);
			}
			break;
		
		case PRESSED:
			if(key != keyValue){
				startTimer1(50);
				state1 = OPENING;
			}else{
				if(timeout5)
					Key3S = 1;
			}
			break;

		case OPENING:
			if(key == keyValue)
				state1=PRESSED;
			else if(timeout1){
				keyState = 0;
				keyEvent = 1;
				keyEventValue = keyValue;
				state1 	 = OPEN;			
			}
			break;
	}
}