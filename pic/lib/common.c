/*******************************************************************************
 * c Common Library
 *******************************************************************************/

uns8 v10base[3];
void set10base(uns8 v)
{
	v10base[0]=0;
	v10base[1]=0;
	v10base[2]=0;

	while(v >= 100){
		v -= 100;
		v10base[2]++;
	}
	while(v >= 10){
		v -= 10;
		v10base[1]++;
	}	
	v10base[0]=v;
}

/*
define DELAY_100 for device
*/
#define delay_us(C)\
	TMR0 = C;\
	T0IF = 0 ;\
    while ( !T0IF );
    
void delay1s(void){
	uns8 i,j;
	//1s
	for(j=0; j<100; j++){
		//10ms
		for(i=0; i<100; i++){
			delay_us(DELAY_100U);	//0.1ms
		}
	}
}

void delayms(uns16 cnt_ms){
	uns8 i;
	uns16 j;
	//1s
	for(j=0; j<cnt_ms; j++){
		//1ms
		for(i=0; i<10; i++){
			delay_us(DELAY_100U);	//0.1ms
		}
	}
}