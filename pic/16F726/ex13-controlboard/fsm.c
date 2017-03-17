
/*
 APP STATE MACHINES
*/

enum { APP_BOOTING=2, APP_ENTRY=3, APP_SET=4, APP_START=5, APP_STOP=6};
enum { SET_PITCH=1, SET_SCORE=2, SET_TIME=3, SET_TIME_DOWNCNT=4 };

#define Delay_100MS  (100)
#define Delay_1000MS  (1000)

#define startTimer2(c)		\
	timer2L		= (c)%256;	\
	timer2H		= (c)/256;	\
	timeout2	= 0;

#define startTimer3(c)		\
	timer3 		= c;		\
	timeout3	= 0;


void initapp(void){
	startTimer2(Delay_100MS);
	state2 = APP_BOOTING;
	state3 = SET_PITCH;		//last state3 value
	state3_1 = SET_PITCH;   //old state3 value
	LED2Value = 0;
}

void getHex2Vxx(uns8 v);

void getSensor(void){
	uns8 i,j;

	i = cntScoreSensor;
	j = cntPitchSensor;
	cntScoreSensor = 0;
	cntPitchSensor = 0;

	if(i) bitScoreSensor = 1;
	if(j) bitPitchSensor = 1; 
}

uns8 get_keyvalue(void){
	uns8 key = 0;

	if( (keyState ==1 ) && (keyEvent == 1) ){
		keyEvent = 0;
		key = keyValue;
	}
	return key;	
}

void varLED2display(void){
	cmddata.cmd = CHIP1NUM;
	cmddata.n[0] = varled.pitchLED[0];
	cmddata.n[1] = varled.pitchLED[1];
	cmddata.n[2] = varled.pitchLED[2];
	cmddata.n[3] = varled.percentLED[0];	
	cmddata.n[4] = varled.percentLED[1];
	cmddata.n[5] = varled.scoreLED[0];
	cmddata.n[6] = varled.scoreLED[1];
	cmddata.n[7] = varled.scoreLED[2];
	cmddata.dp   = varled.dpLED[0];
	send_sci();

	cmddata.cmd = CHIP2NUM;
	cmddata.n[0] = varled.timeLED[0];
	cmddata.n[1] = varled.timeLED[1];
	cmddata.n[2] = varled.timeLED[2];
	cmddata.n[3] = varled.timeLED[3];	
	cmddata.dp   = varled.dpLED[1];	
	send_sci();
}

void init_varled(void){
	varled.pitchLED[0] = 0;
	varled.pitchLED[1] = 2;
	varled.pitchLED[2] = NUM_CLEAR;
	varled.pitchKeyPos = 1;

	varled.percentLED[0] = SET_PITCH;
	varled.percentLED[1] = NUM_CLEAR;

	varled.scoreLED[0] = NUM_CLEAR;
	varled.scoreLED[1] = NUM_CLEAR;
	varled.scoreLED[2] = NUM_CLEAR;
	varled.scoreKeyPos = 0;
	varled.dpLED[0] = 0;
	
	varled.timeLED[0] = NUM_CLEAR;
	varled.timeLED[1] = NUM_CLEAR;
	varled.timeLED[2] = NUM_CLEAR;
	varled.timeLED[3] = NUM_CLEAR;	
	varled.timeKeyPos = 0;
	varled.dpLED[1] = 0;

	varled.pitchValue = 0;
	varled.percentValue = 0;
	varled.timeValue[0] = 0;
	varled.timeValue[1] = 0;
}

void set_init_varled(void){
	varled.pitchLED[0] = NUM_CLEAR;
	varled.pitchLED[1] = NUM_CLEAR;
	varled.pitchLED[2] = NUM_CLEAR;
	varled.pitchKeyPos = 0;

	varled.percentLED[0] = SET_PITCH;
	varled.percentLED[1] = NUM_CLEAR;

	varled.scoreLED[0] = NUM_CLEAR;
	varled.scoreLED[1] = NUM_CLEAR;
	varled.scoreLED[2] = NUM_CLEAR;
	varled.scoreKeyPos = 0;
	varled.dpLED[0] = 0;
	
	varled.timeLED[0] = NUM_CLEAR;
	varled.timeLED[1] = NUM_CLEAR;
	varled.timeLED[2] = NUM_CLEAR;
	varled.timeLED[3] = NUM_CLEAR;	
	varled.timeKeyPos = 0;
	varled.dpLED[1] = 0;

	varled.pitchValue = 0;
	varled.percentValue = 0;
	varled.timeValue[0] = 0;
	varled.timeValue[1] = 0;
}


uns8 getCntUp(uns8 cnt,uns8 max){
	uns8 ret;
	ret = cnt + 1;
	if(ret > max) ret=0;
	return ret;
}
uns8 getCntDown(uns8 num,uns8 max){
	uns8 ret=0;

	if(num > 1){
		ret = num -1;
	}else{
		ret = max;
	}
	return ret;
}
void updatePitchLED(uns8 key){
	uns8 pos = varled.pitchKeyPos;
	uns8 posVal = varled.pitchLED[pos];

	if(key == KEY_UP){
		posVal=getCntUp(posVal,9);
		varled.pitchLED[pos]=posVal;

	}else if(key == KEY_DOWN){
		posVal=getCntDown(posVal,9);
		varled.pitchLED[pos]=posVal;

	}else if(key == KEY_LEFT){
		pos++;
		if(pos > 2) pos =2;
		varled.pitchLED[pos] = 1;
		varled.pitchKeyPos = pos;

	}else if(key == KEY_RIGHT){
		if(pos >= 1){
			varled.pitchLED[pos]=NUM_CLEAR;
			pos--;
			varled.pitchKeyPos = pos;
		}
	}

}
void updateScoreLED(uns8 key){
	uns8 pos = varled.scoreKeyPos;
	uns8 posVal = varled.scoreLED[pos];

	if(key == KEY_UP){
		posVal=getCntUp(posVal,9);
		varled.scoreLED[pos]=posVal;
		
	}else if(key == KEY_DOWN){
		posVal=getCntDown(posVal,9);
		varled.scoreLED[pos]=posVal;

	}else if(key == KEY_LEFT){
		pos++;
		if(pos > 2) pos =2;
		varled.scoreLED[pos] = 1;
		varled.scoreKeyPos = pos;
		

	}else if(key == KEY_RIGHT){
		if(pos >= 1){
			varled.scoreLED[pos]=NUM_CLEAR;
			pos--;
			varled.scoreKeyPos = pos;
		}

	}

}

void updateTimeLED(uns8 key){
	uns8 pos = varled.timeKeyPos;
	uns8 posVal = varled.timeLED[pos];

	if(key == KEY_UP){
		posVal=getCntUp(posVal,6);
		varled.timeLED[pos]=posVal;

	}else if(key == KEY_DOWN){
		posVal=getCntDown(posVal,6);
		varled.timeLED[pos]=posVal;

	}else if(key == KEY_LEFT){
		pos++;
		if(pos > 3) pos =3;
		varled.timeLED[pos] = 1;
		varled.timeKeyPos = pos;

	}else if(key == KEY_RIGHT){
		if(pos >= 1){
			varled.timeLED[pos]=NUM_CLEAR;
			pos--;
			varled.timeKeyPos = pos;
		}
	}	
	
}

void app_set_mode_display(uns8 key){
	uns8 keychange=0;
	uns8 keypos=0;

	if(state3_1 != state3) {
	//mode change
		state3_1 = state3;		
		set_init_varled();

		if(state3 == SET_PITCH){
			varled.pitchLED[0] = 0;
			varled.pitchLED[1] = 1;
			varled.pitchKeyPos = 1;
			//varled.pitchLED[2] = 0;

		}else if(state3 == SET_SCORE){
			varled.scoreLED[0] = 0;
			varled.scoreLED[1] = 1;
			varled.scoreKeyPos = 1;
			//varled.scoreLED[2] = 0;


		}else if((state3 == SET_TIME) || (state3 == SET_TIME_DOWNCNT)){
			varled.timeLED[0] = 0;
			varled.timeLED[1] = 1;
			varled.timeKeyPos = 1;
			//varled.timeLED[2] = 0;
			//varled.timeLED[3] = 0;
			varled.dpLED[1] = NUM2_DP;
		}
	}
	
	varled.percentLED[0] = state3;
	varled.percentLED[1] = key; 

	if( (key == KEY_UP) || (key == KEY_DOWN) || (key == KEY_LEFT) || (key == KEY_RIGHT) ){

		if(state3 == SET_PITCH){ 
			updatePitchLED(key);

		}else if(state3 == SET_SCORE){ 
			updateScoreLED(key);

		}else if( (state3 == SET_TIME) || (state3 == SET_TIME_DOWNCNT) ){
			updateTimeLED(key);
			varled.dpLED[1] = NUM2_DP;
		}
	}
}


void init_display(void){
	cmddata.cmd = CHIP1NUM;
	cmddata.n[0] = NUM_CLEAR;
	cmddata.n[1] = NUM_CLEAR;
	cmddata.n[2] = NUM_CLEAR;
	cmddata.n[3] = NUM_CLEAR;	
	cmddata.n[4] = NUM_CLEAR;
	cmddata.n[5] = NUM_CLEAR;
	cmddata.n[6] = NUM_CLEAR;
	cmddata.n[7] = NUM_CLEAR;
	send_sci();

	cmddata.cmd = CHIP2NUM;
	cmddata.n[0] = NUM_CLEAR;
	cmddata.n[1] = NUM_CLEAR;
	cmddata.n[2] = NUM_CLEAR;
	cmddata.n[3] = NUM_CLEAR;	
	send_sci();
}

void time_task(void){
	
	//50ms
	if(timeout3){
		startTimer3(50);
	}

	//1S
	if(timeout2){
		startTimer2(Delay_1000MS);
		varLED2display();
	}	
}

void fsm3(uns8 key){
	switch(state3){
		case SET_PITCH:
			if(key == KEY_SET){
				state3 = SET_SCORE;
			}
			break;

		case SET_SCORE:
			if(key == KEY_SET){			
				state3 = SET_TIME;
			}		
			break;

		case SET_TIME:
			if(key == KEY_SET){			
				state3 = SET_TIME_DOWNCNT;
			}
			break;

		case SET_TIME_DOWNCNT:
			if(key == KEY_SET){			
				state3 = SET_PITCH;
			}
			break;
	}
}

void fsm2( void)
{
	uns8 key;

	switch(state2){
		case APP_BOOTING:
			if(timeout2){
				state2	= APP_SET;
				init_display();
			}
			break;

		case APP_SET:
			key = get_keyvalue();
			if(key == KEY_START){
				state2 = APP_START;
			}else if(key){
				fsm3(key);
				app_set_mode_display(key);
			}
			time_task();
			break;

		case APP_START:
			break;

		case APP_STOP:
			break;
	}
}

void getHex2Vxx(uns8 v){
	vxx[2]=0;
	vxx[1]=0;
	vxx[0]=0;

	while(v >= 100){
		v -=100;
		vxx[2]++;
	}

	while(v >= 10){
		v -=10;
		vxx[1]++;
	}

	vxx[0] = v;
}