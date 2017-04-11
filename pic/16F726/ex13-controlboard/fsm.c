
/*
 APP STATE MACHINES
*/

enum { APP_BOOTING=2, APP_ENTRY=3, APP_SET=4, APP_TAKE_BALL=5, APP_RUN=6, APP_PAUSE=7, APP_FINISH=8};
enum { SET_PITCH=1, SET_SCORE=2, SET_TIME=3, SET_TIME_DOWNCNT=4 };

#define Delay_100MS  (100)
#define Delay_1000MS  (1000)
#define Delay_3000MS  (3000)
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

#define startTimer6(c)		\
	timer6L		= (c)%256;	\
	timer6H		= (c)/256;	\
	timeout6	= 0;

#define startTimer7(c)		\
	timer7L		= (c)%256;	\
	timer7H		= (c)/256;	\
	timeout7	= 0;

void initapp(void){
	startTimer2(Delay_100MS);
	state2 = APP_BOOTING;
	state3 = SET_PITCH;		//last state3 value
	state3_1 = SET_PITCH;   //old state3 value
	LED2Value = 0;
}

void getHex2Vxx(uns8 v);
void get16Hex2Vxx(uns16 v);
uns16 getDec2Hex(uns8 v0, uns8 v10, uns8 v100);
void varLED2Update(void);

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

void varLED2displayOff(void){
	cmddata.cmd = CHIP1NUM;
	cmddata.n[0] = NUM_CLEAR;
	cmddata.n[1] = NUM_CLEAR;
	cmddata.n[2] = NUM_CLEAR;
	cmddata.n[3] = NUM_CLEAR;	
	cmddata.n[4] = NUM_CLEAR;
	cmddata.n[5] = NUM_CLEAR;
	cmddata.n[6] = NUM_CLEAR;
	cmddata.n[7] = NUM_CLEAR;
	cmddata.dp   = 0;
	send_sci();

	cmddata.cmd = CHIP2NUM;
	cmddata.n[0] = NUM_CLEAR;
	cmddata.n[1] = NUM_CLEAR;
	cmddata.n[2] = NUM_CLEAR;
	cmddata.n[3] = NUM_CLEAR;	
	cmddata.dp   = 0;	
	send_sci();
}

void init_varled(void){
	varled.pitchLED[0] = 0;
	varled.pitchLED[1] = 0;
	varled.pitchLED[2] = 0;
	varled.pitchKeyPos = 0;

	varled.percentLED[0] = SET_PITCH;
	varled.percentLED[1] = NUM_CLEAR;

	varled.scoreLED[0] = 0;
	varled.scoreLED[1] = 0;
	varled.scoreLED[2] = 0;
	varled.scoreKeyPos = 0;
	varled.dpLED[0] = 0;
	
	varled.timeLED[0] = 0;
	varled.timeLED[1] = 0;
	varled.timeLED[2] = 0;
	varled.timeLED[3] = 0;	
	varled.timeKeyPos = 0;
	varled.dpLED[1] = NUM2_DP;

	varled.pitchValue = 0;
	varled.percentValue = 0;
	varled.timeValue[0] = 0;
	varled.timeValue[1] = 0;
	varled.st.v = 0;
	varled.st.b.light_en = 1;		
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
	uns8 v0,v1;

	varled.pitchValue=0;
	varled.pitchCnt=0;
	
	//varled.percentValue = 0;
	//uns8 percentValue;
	
	varled.scoreValue=0;
	varled.scoreCnt=0;

	varled.timeCnt[0]	=0;	
	varled.timeCnt[1] 	=0;

	if(state3 == SET_PITCH){
		if( varled.pitchLED[0] || varled.pitchLED[1] || varled.pitchLED[2] ){
			ret = 1;
			varled.pitchValue = getDec2Hex(varled.pitchLED[0],varled.pitchLED[1],varled.pitchLED[2]);
			//v0 = varled.pitchValue & 0xff;
			//v1 = (varled.pitchValue >> 8);
			//putch(0xf1);
			//putch(v0);
			//putch(0xf2);
			//putch(v1);
		}

	}else if(state3 == SET_SCORE){
		if( varled.scoreLED[0] || varled.scoreLED[1] || varled.scoreLED[2] ){
			ret = 1;
			varled.scoreValue = getDec2Hex(varled.scoreLED[0],varled.scoreLED[1],varled.scoreLED[2]);
			//v0 = varled.scoreValue & 0xff;
			//v1 = (varled.scoreValue >> 8);
			//putch(0xf3);
			//putch(v0);
			//putch(0xf4);
			//putch(v1);			
		}

	}else if((state3 == SET_TIME) || (state3 == SET_TIME_DOWNCNT)){
		if( varled.timeLED[0] || varled.timeLED[1] || varled.timeLED[2] || varled.timeLED[3] ){
			ret = 1;
			v0 = getDec2Hex(varled.timeLED[0],varled.timeLED[1],0);
			v1 = getDec2Hex(varled.timeLED[2],varled.timeLED[3],0);

			if( state3 == SET_TIME ){
				varled.timeValue[0] = v0;
				varled.timeValue[1] = v1;
			}else{
				varled.timeCnt[0]	= v0;	
				varled.timeCnt[1] 	= v1;
			}

			//putch(0xf5);
			//putch(varled.timeValue[0]);
			//putch(0xf6);
			//putch(varled.timeValue[1]);				
		}
	}
	return ret;
}
void app_set_mode_display(uns8 key){
	uns8 keychange=0;
	uns8 keypos=0;

	if(state3_1 != state3) {
	//mode change
		state3_1 = state3;		
		init_varled();
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

void app_set_time_task(void){
	uns8 val;

	//0.3S
	if(timeout4 ){
		startTimer4(Delay_300MS);
		if(varled.st.b.light_en == 1){
			if(varled.st.b.light == 0){
				val = clearByLight();
				varLED2display();
				restoreByLight(val);
				varled.st.b.light = 1;
			}else{
				varLED2display();
				varled.st.b.light = 0;
			}
		}else{
			varLED2display();
		}
	}		
}

void app_pause_time_task(void){
	uns8 val;

	//0.3S
	if(timeout4 ){
		startTimer4(Delay_300MS);
		if(varled.st.b.light == 0){
			varLED2displayOff();
			varled.st.b.light = 1;
		}else{
			varLED2display();
			varled.st.b.light = 0;
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

void timeCountUp(void){
	varled.timeCnt[0]++;
	if(varled.timeCnt[0] == 60){
		varled.timeCnt[0] = 0;
		varled.timeCnt[1]++;
	}
}
void timeCountDown(void){
	if(varled.timeCnt[0] == 0){
		varled.timeCnt[0] = 59;

		if(varled.timeCnt[1] > 0)
			varled.timeCnt[1]--;
	}else{
		varled.timeCnt[0]--;
	}
}
void pitchCount(void){
	if(cntTakeBallSensor){
		cntTakeBallSensor = 0;
		varled.pitchCnt++;
	}
}
uns8 scoreCount(void){
	if(cntScoreSensor){
		cntScoreSensor = 0;
		varled.scoreCnt++;
		return 1;
	}
	return 0;
}

uns8 get_percent_value(void){
	uns8 ret=0;
	uns16 v=0;
	uns8 cnt=100;
	uns16 subV = varled.pitchCnt;

	// v = varled.scoreCnt x 100;
	while(cnt){
		--cnt;
		v += varled.scoreCnt;
	}

	// ret = v / varled.pitchCnt;
	while(v){
		if(v >= varled.pitchCnt){
			v -= varled.pitchCnt;
			ret++;
		}else
		 	v = 0;
	}

	return ret;
}

void calPercentValue(void){
	uns8 cal=0;

	if(varled.pitchCnt != 0){
		if(varled.scoreCnt == 0){
			varled.percentValue = 0;
		}else{
			cal = 1;
		}
	}else{
		varled.percentValue = 0;
	}

	if(cal == 1){
		if(varled.scoreCnt < varled.pitchCnt){
			varled.percentValue = get_percent_value();
		}else{
			varled.percentValue = 99;
		}
	}
}
void varLED2Update(void){

	get16Hex2Vxx(varled.pitchCnt);
	varled.pitchLED[2] = vxx[2];
	varled.pitchLED[1] = vxx[1];
	varled.pitchLED[0] = vxx[0];

	get16Hex2Vxx(varled.scoreCnt);	
	varled.scoreLED[2] = vxx[2];
	varled.scoreLED[1] = vxx[1];
	varled.scoreLED[0] = vxx[0];


	calPercentValue();
	get16Hex2Vxx(varled.percentValue);
	varled.percentLED[0] = vxx[0];
	varled.percentLED[1] = vxx[1];

	getHex2Vxx(varled.timeCnt[0]);
	varled.timeLED[0] = vxx[0];
	varled.timeLED[1] = vxx[1];

	getHex2Vxx(varled.timeCnt[1]);
	varled.timeLED[2] = vxx[0];
	varled.timeLED[3] = vxx[1];	

	varled.dpLED[1] = NUM2_DP;
}

void app_running_time_task(){
	uns8 v0,v1,getScore;

	//3S
	if(state3 == SET_PITCH){
		if( (varled.pitchCnt == varled.pitchValue)  && timeout7 ){
				varled.st.b.app_run_finish = 1;
				return;
		}
	}

	//300ms
	if(timeout6){
		getScore = 0;
		startTimer6(Delay_300MS);

		if(state3 != SET_PITCH) 
			pitchCount();

		getScore=scoreCount();

		if(state3 == SET_PITCH) {
			if( varled.pitchCnt < varled.pitchValue ){
				pitchCount();
				if(varled.pitchCnt == varled.pitchValue){
					startTimer7(Delay_3000MS);
				}

			}else{
				if(getScore){
					varled.st.b.app_run_finish = 1;
				}
			}

		}else if(state3 == SET_SCORE){
			if(varled.scoreCnt == varled.scoreValue)
				varled.st.b.app_run_finish = 1;	
		}
	}

	//1S
	if(timeout2){
		startTimer2(Delay_1000MS);

		if(state3 == SET_TIME_DOWNCNT){
			timeCountDown();
			if( (varled.timeCnt[0] == 0) && (varled.timeCnt[1] == 0) )
				varled.st.b.app_run_finish = 1;

		}else if( (state3 == SET_TIME) || (state3 == SET_PITCH) || (state3 == SET_SCORE)){
			timeCountUp();
			if(state3 == SET_TIME){
				if( (varled.timeCnt[0] == varled.timeValue[0]) && (varled.timeCnt[1] == varled.timeValue[1]) )
					varled.st.b.app_run_finish = 1;
			}
		}

		varLED2Update();
		varLED2display();

		//v0 = varled.pitchValue & 0xff;
		//v1 = (varled.pitchValue >> 8);
		//putch(0xf1);
		//putch(v0);
		//putch(0xf2);
		//putch(v1);
		//v0 = varled.pitchCnt & 0xff;
		//v1 = (varled.pitchCnt >> 8);
		//putch(0xf3);
		//putch(v0);
		//putch(0xf4);
		//putch(v1);
	}else{
		if(varled.st.b.app_run_finish == 1){

			varLED2Update();
			varLED2display();
		}
	}
}

void app_run(void){
	app_running_time_task();
}


void app_set_other_mode_clear(void){

	if(state3 == SET_PITCH){ 
		varled.scoreCnt = 0;
		varled.timeCnt[0] = 0;
		varled.timeCnt[1] = 0;

	}else if(state3 == SET_SCORE){
		varled.pitchCnt = 0;
		varled.timeCnt[0] = 0;
		varled.timeCnt[1] = 0;
		
	}else if( (state3 == SET_TIME) || (state3 == SET_TIME_DOWNCNT) ){
		varled.scoreCnt = 0;
		varled.pitchCnt = 0;
		varled.timeCnt[0] = varled.timeValue[0];
		varled.timeCnt[1] = varled.timeValue[1];
	}
}

void init_mode_value(uns8 mode){

	if(state3 == SET_PITCH){
		varled.pitchCnt = varled.pitchValue;

	}else if( state3 == SET_SCORE ){
		varled.scoreCnt = varled.scoreValue;

	}else if( (state3 == SET_TIME) || (state3 == SET_TIME_DOWNCNT) ){
		varled.timeCnt[0] = varled.timeValue[0]; 
		varled.timeCnt[1] = varled.timeValue[1];

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

			if( varled.st.b.app_run_finish == 1){

				varled.st.b.app_run_finish = 0;
				app_set_other_mode_clear();
				init_mode_value(state3);
				varLED2Update();
				varled.percentLED[0] = state3;
				varled.percentLED[1] = NUM_CLEAR;
				varLED2display();

			}else if( key == KEY_START ){
				finish = checkSetFinish();

				if(finish){
					state2 = APP_TAKE_BALL;
					cntScoreSensor = 0;
					cntTakeBallSensor = 0;
					varLED2display();
				}
			}else if(key){
				fsm3(key);
				app_set_mode_display(key);
			}

			if( finish == 0)
				app_set_time_task();
			break;

		case APP_TAKE_BALL:
				if(cntTakeBallSensor){
					cntScoreSensor = 0;
					state2 = APP_RUN;
				}
			break;

		case APP_RUN:
			app_run();
			
			key = get_keyvalue();
			finish = varled.st.b.app_run_finish;

			if( finish == 1 ){
				state2 = APP_FINISH;
			
			}else if( (Key3S==1) && (key == KEY_START) ){
				varled.st.b.app_run_finish = 1;
				state2 = APP_SET;

			}else if( key == KEY_START){
				state2 = APP_PAUSE;
			}
		
			break;

		case APP_PAUSE:
			app_pause_time_task();
			key = get_keyvalue();
			if( key == KEY_START){
				state2 = APP_RUN;
				cntScoreSensor = 0;

			}else if( key == KEY_SET){
				varled.st.b.app_run_finish = 1;
				state2 = APP_SET;
			}		
			break;

		case APP_FINISH:
			key = get_keyvalue();
			if( key == KEY_SET){
				state2 = APP_SET;
			}		
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

void get16Hex2Vxx(uns16 v){
	vxx[2]=0;
	vxx[1]=0;
	vxx[0]=0;

	while(v >= 1000){
		v-=1000;
	}

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

uns16 getDec2Hex(uns8 v0, uns8 v10, uns8 v100){
	uns16 v=0;

	if(v0) v += v0;

	while(v10){
		v10--;
		v += 10;
	}

	while(v100){
		v100--;
		v += 100;
	}

	return v;
}
