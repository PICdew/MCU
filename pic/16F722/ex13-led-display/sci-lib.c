typedef struct {
	uns8	buf[RXBUF_MAX];
	uns8    cnt;
	struct{
		uns8	stx:1;
		uns8	etx:1;
		uns8    dle:1;
		uns8    cmdfull:1;
	}b;
} Rxx;

#define CMDxx_SIZE 3

#define puttx(c)	\
	while(!TXIF)	\
		continue;	\
	TXREG = c;		\

typedef struct {
	uns8 cmd[CMDxx_SIZE];
	uns8 b0[CMDxx_SIZE];
	uns8 b1[CMDxx_SIZE];
	uns8 b2[CMDxx_SIZE];
	uns8 b3[CMDxx_SIZE];
	uns8 b4[CMDxx_SIZE]; //dp
	uns8 tail;
	uns8 head;
} CMDxx;


Rxx rxdata;
CMDxx cmddata;
uns8  cmdret[6]; //cmd,b0,b1,b2,b3,b4

// <STX> <CMD><VALUE> <CHK> <ETX>
//  <1>  <1><3>       <1>   <1>
uns8 calculateBufChkSum(uns8 len);
void putch(uns8 byte);
void writeUNS8(uns8 v);


#pragma origin 4
interrupt int_server(void)
{
    int_save_registers    // W, STATUS (and PCLATH)
    uns8 sv_FSR,ch,sum,cnt,len,i;
    sv_FSR = FSR;
	if(RCIF){
		//rx interrupte ,RCREG
		ch = RCREG;

		if(rxdata.b.dle){
			if(rxdata.b.stx){
				rxdata.buf[rxdata.cnt] = ch;
				rxdata.cnt++;
			}
			rxdata.b.dle = 0;

		}else if(ch == DLE){
			rxdata.b.dle = 1;

		}else if(ch == ETX){
			rxdata.b.etx = 1;
			rxdata.b.stx = 0;

			if(rxdata.cnt >= 3){
				len = rxdata.cnt - 1;
				sum = calculateBufChkSum(len);

				if(sum != rxdata.buf[len]){
					rxdata.b.etx = 0;
				}
			}else{
				rxdata.b.etx = 0;
			}

		}else if( ch == STX){
			rxdata.cnt = 0;
			rxdata.b.stx = 1;
			rxdata.b.etx = 0;
			rxdata.b.dle = 0;

		}else if(rxdata.b.stx){
			rxdata.buf[rxdata.cnt] = ch;
			rxdata.cnt++;		

		}
	}


// <STX> <CMD><VALUE> <CHK> <ETX>
//  <1>  <1><3>       <1>   <1>
	if(rxdata.b.etx){
		if(rxdata.b.cmdfull == 0){
			len = cmddata.head;
			cmddata.cmd[len] = rxdata.buf[0];
			cmddata.b0[len] = rxdata.buf[1];
			cmddata.b1[len] = rxdata.buf[2];
			cmddata.b2[len] = rxdata.buf[3];
			cmddata.b3[len] = rxdata.buf[4];
			cmddata.b4[len] = rxdata.buf[5];

			len = len + 1;
			if(len >= CMDxx_SIZE) len = 0;

			if( len == cmddata.tail ){
				rxdata.b.cmdfull = 1;
			}else{
				cmddata.head = len;
			}
		}
		rxdata.b.etx = 0;
	}
	FSR = sv_FSR;
    int_restore_registers // W, STATUS (and PCLATH)
}

uns8 calculateBufChkSum(uns8 len){
	uns8 i,sum;
	
	sum=0;
	for(i=0; i<len; i++) sum += rxdata.buf[i];
	return sum;
}

uns8 getCmd(void){
	uns8 ret,tail;

	if( cmddata.head == cmddata.tail ){
		ret  = 0;
	}else{
		ret  = 1;
		tail = cmddata.tail;
		cmdret[0]  = cmddata.cmd[tail];
		cmdret[1]   = cmddata.b0[tail];
		cmdret[2]   = cmddata.b1[tail];	
		cmdret[3]   = cmddata.b2[tail];
		cmdret[4]   = cmddata.b3[tail];
		cmdret[5]   = cmddata.b4[tail];

		tail =  tail +1;
		if(tail >= CMDxx_SIZE ) tail = 0;
		cmddata.tail = tail;

		rxdata.b.cmdfull = 0;
	}

	return ret;
}