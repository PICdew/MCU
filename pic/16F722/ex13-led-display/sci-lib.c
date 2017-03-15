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

#define CMDxx_SIZE 4

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
	uns8 tail;
	uns8 head;
} CMDxx;


Rxx rxdata;
CMDxx cmddata;
uns8  cmdret[5]; //cmd,b0,b1,b2,b3

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
		//putch(ch);
		//RxCH = ch;
		//rxdata.buf[rxdata.cnt] = ch;
		//rxdata.cnt++;
		//if(rxdata.cnt > RXBUF_MAX) rxdata.cnt=0;

		//writeUNS8(ch);
	//	putch(ch);

	//i = ch >>4;
	//puttx(hex2char2[i]);		//high 4-bit
	//i = ch & 0x0f;
	//puttx(hex2char2[i]);		//low 4-bit		


		if(rxdata.b.dle){
			if(rxdata.b.stx){
				rxdata.buf[rxdata.cnt] = ch;
				rxdata.cnt++;
			}
			rxdata.b.dle = 0;

		}else if(ch == DLE){
			//putch(DLE);
			rxdata.b.dle = 1;

		}else if(ch == ETX){
			//putch(ETX);
			rxdata.b.etx = 1;
			rxdata.b.stx = 0;

			if(rxdata.cnt >= 3){
				len = rxdata.cnt - 1;
				sum = calculateBufChkSum(len);
				
				//putch(0xf1);
				//putch(rxdata.cnt);
				//putch(0xf1);
				//for(i=0; i<rxdata.cnt; i++)
				//	putch(rxdata.buf[i]);

				//putch(0xf2);
				//putch(sum);

				if(sum != rxdata.buf[len]){
					rxdata.b.etx = 0;
					//putch(0xf2);
				}
			}else{
				rxdata.b.etx = 0;
			}

		}else if( ch == STX){
			rxdata.cnt = 0;
			rxdata.b.stx = 1;
			rxdata.b.etx = 0;
			rxdata.b.dle = 0;
			//putch(STX);

		}else if(rxdata.b.stx){
			rxdata.buf[rxdata.cnt] = ch;
			rxdata.cnt++;		

		}

		//TXREG = ch;
		//RCIF = 0;
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

		tail =  tail +1;
		if(tail >= CMDxx_SIZE ) tail = 0;
		cmddata.tail = tail;

		rxdata.b.cmdfull = 0;
	}

	return ret;
}
void putch(uns8 byte) 
{
							/* output one byte */
	while(!TXIF)			/* set when register is empty */
		continue;
	TXREG = byte;
}

#define CR()	putch(0x0D);
#define LF()	putch(0x0A);
void crlf(void){
	CR();
	LF();
}

const uns8 hex2char[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
void writeUNS8(uns8 v){
	uns8 i;
	i = v >>4;
	putch(hex2char[i]);		//high 4-bit
	i = v & 0x0f;
	putch(hex2char[i]);		//low 4-bit
}
void printCmd(void){
	uns8 tail,cmd,value2,value1,value0;

	if( cmddata.head != cmddata.tail ){
		tail = cmddata.tail;
		cmd  = cmddata.cmd[tail];
		value2   = cmddata.b2[tail];		
		value1   = cmddata.b1[tail];
		value0   = cmddata.b0[tail];

		tail =  tail +1;
		if(tail >= CMDxx_SIZE ) tail = 0;
		cmddata.tail = tail;
		rxdata.b.cmdfull = 0;		

	//	tail = tail + 0x30;

		putch('S');
		putch(tail);
		putch('-');
		writeUNS8(cmd);
		writeUNS8(value2);		
		writeUNS8(value1);
		writeUNS8(value0);
		putch('*');
		crlf();


	}
}