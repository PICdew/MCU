
/*
 APP STATE MACHINES
*/

enum { APP_BOOTING=2, APP_ENTRY=3, APP_SET=4, APP_SET_FINISH=5, APP_START=6, APP_STOP=7};
enum { SET_PITCH=1, SET_SCORE=2, SET_TIME=3, SET_TIME_DOWNCNT=4 };

#define Delay_100MS  (100)
#define Delay_1000MS  (1000)
#define Delay_500MS  (500)
#define Delay_300MS  (300)

#define startTimer2(c)		\
	timer2L		= (c)%256;	\
	timer2H		= (c)/256;	\
	timeout2	= 0;

#define startTimer3(c)		\
	timer3 		= c;		\
	timeout3	= 0;

#define startTimer4(c)		\
	timer4L		= (c)%256;	\
	timer4H		= (c)/256;	\
	timeout4	= 0;	


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

	if( keyEvent == 1 ){
		keyEvent = 0;
		key = keyEventValue;
	}
	return key;	
}

uns8 clearByLight(void){
	uns8 val = 0;

	if(state3 == SET_PITCH){
		val = varled.pitchLED[varled.pitchKeyPos];
		varled.pitchLED[varled.pitchKeyPos] = NUM_CLEAR;

	}else if(state3 == SET_SCORE){
		val = varled.scoreLED[varled.scoreKeyPos];
		varled.scoreLED[varled.scoreKeyPos] = NUM_CLEAR;

	}else if((state3 == SET_TIME) || (state3 == SET_TIME_DOWNCNT)){
		val = varled.timeLED[varled.timeKeyPos];
		varled.timeLED[varled.timeKeyPos] = NUM_CLEAR;
	}
	return val;
}

void restoreByLight(uns8 val){

	if(state3 == SET_PITCH){
		varled.pitchLED[varled.pitchKeyPos] = val;

	}else if(state3 == SET_SCORE){
		varled.scoreLED[varled.scoreKeyPos] = val;

	}else if((state3 == SET_TIME) || (state3 == SET_TIME_DOWNCNT)){
		varled.timeLED[varled.timeKeyPos] = val;
	}
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
	varled.pitchLED[1] = 0;
	varled.pitchLED[2] = 0;
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
	varled.light.v = 0;
	varled.light.b.en = 1;		
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

	varled.light.v = 0;
	varled.light.b.en = 1;	
}


uns8 getCntUp(uns8 cnt,uns8 max){
	uns8 ret;
	ret = cnt + 1;
	if(ret > max) ret=0;
	return ret;
}
uns8 getCntDown(uns8 num,uns8 max){
	uns8 ret=0;

	if(num >= 1){
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
		varled.pitchKeyPos = pos;

	}else if(key == KEY_RIGHT){
		if(pos >= 1){
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
		varled.scoreKeyPos = pos;
		

	}else if(key == KEY_RIGHT){
		if(pos >= 1){
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
		varled.timeKeyPos = pos;

	}else if(key == KEY_RIGHT){
		if(pos >= 1){
			pos--;
			varled.timeKeyPos = pos;
		}
	}	
	
}

uns8 checkSetFinish(void){
	uns8 ret = 0;

	if(state3 == SET_PITCH){
		if( varled.pitchLED[0] || varled.pitchLED[1] || varled.pitchLED[2] )
			ret = 1;

	}else if(state3 == SET_SCORE){
		if( varled.scoreLED[0] || varled.scoreLED[1] || varled.scoreLED[2] )
			ret = 1;

	}else if((state3 == SET_TIME) || (state3 == SET_TIME_DOWNCNT)){
		if( varled.timeLED[0] || varled.timeLED[1] || varled.timeLED[2] || varled.timeLED[3] )
			ret = 1;
	}
	return ret;
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
			varled.pitchLED[1] = 0;
			varled.pitchLED[2] = 0;
			varled.pitchKeyPos = 0;

		}else if(state3 == SET_SCORE){
			varled.scoreLED[0] = 0;
			varled.scoreLED[1] = 0;
			varled.scoreLED[2] = 0;			
			varled.scoreKeyPos = 0;

		}else if((state3 == SET_TIME) || (state3 == SET_TIME_DOWNCNT)){
			varled.timeLED[0] = 0;
			varled.timeLED[1] = 0;
			varled.timeLED[2] = 0;
			varled.timeLED[3] = 0;
			varled.timeKeyPos = 0;
			varled.dpLED[1] = NUM2_DP;
		}
	}
	
	varled.percentLED[0] = state3;

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
	}	
}

void app_set_time_task(void){
	uns8 val;

	//0.3S
	if(timeout4 ){
		startTimer4(Delay_300MS);
		if(varled.light.b.en == 1){
			if(varled.light.b.light == 0){
				val = clearByLight();
				varLED2display();
				restoreByLight(val);
				varled.light.b.light = 1;
			}else{
				varLED2display();
				varled.light.b.light = 0;
			}
		}else{
			varLED2display();
		}
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
	uns8 key,finish;

	switch(state2){
		case APP_BOOTING:
			if(timeout2){
				state2	= APP_SET;
				init_display();
			}
			break;

		case APP_SET:
			key = get_keyvalue();		
			finish = 0;
			if( (Key3S==1) && (keyState == 1) && (keyValue == KEY_SET) ){
				finish = checkSetFinish();

				if(finish){
					state2 = APP_SET_FINISH;
					varLED2display();
				}
			}else if(key){
				fsm3(key);
				app_set_mode_display(key);
			}

			if( finish == 0)
				app_set_time_task();
			break;

		case APP_SET_FINISH:
			key = get_keyvalue();
			if( (key == KEY_SET) && (Key3S==0)){
				state2 = APP_SET;
			}else if( key == KEY_START){
				state2 = APP_START;
			}
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