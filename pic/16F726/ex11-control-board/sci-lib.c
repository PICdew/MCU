typedef struct {
	uns8	buf[RXBUF_MAX];
	uns8    len;
} Txx;

typedef struct {
	uns8 cmd;
	uns8 n[8];
} CMDxx;

//#define putch(c)	\
//	while(!TXIF)	\
//		continue;	\
//	TXREG = c;		\

Txx txdata;
CMDxx cmddata;

uns8 calculateBufChkSum(uns8 len);
uns8 enable_dle(uns8 ch);
uns8 getbx(uns8 n0,uns8 n1);
uns8 createTXData(void);


const uns8 hex2char[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

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
void writeUNS8(uns8 v){
	uns8 i;
	i = v >>4;
	putch(hex2char[i]);		//high 4-bit
	i = v & 0x0f;
	putch(hex2char[i]);		//low 4-bit
}

void send_sci(void)
{
	uns8 i,ch,ret;

	ret = createTXData();
	if(ret == 0) return;

/*
	for(i=0; i<txdata.len; i++){
		ch = txdata.buf[i];
		writeUNS8(ch);
		crlf();
	}
	return;
*/

	// <STX> <CMD><VALUE> <CHK> <ETX>
	putch(STX);
	for(i=0; i<txdata.len; i++){
		ch = txdata.buf[i];
		
		if( enable_dle(ch) )
			putch(DLE);

		putch(ch);
	}
	putch(ETX);
}

uns8 createTXData(void){
	uns8 ret;

	if( (cmddata.cmd == CHIP1NUM) || (cmddata.cmd == CHIP2NUM) ){
		//cmd,b0,b1,b2,b3
		txdata.buf[0] = cmddata.cmd;
		txdata.buf[1] = getbx(cmddata.n[0],cmddata.n[1]);
		txdata.buf[2] = getbx(cmddata.n[2],cmddata.n[3]);

		if(cmddata.cmd == CHIP1NUM){
			txdata.buf[3] = getbx(cmddata.n[4],cmddata.n[5]);
			txdata.buf[4] = getbx(cmddata.n[6],cmddata.n[7]);
			txdata.buf[5] = calculateBufChkSum(5);
			txdata.len    = 6;
		}else{
			txdata.buf[3] = calculateBufChkSum(3);
			txdata.len    = 4;
		}

		ret = 1;
	}else if( (cmddata.cmd == CHIP1REG) || (cmddata.cmd == CHIP2REG)){
		//cmd,b0,b1
		txdata.buf[0] = cmddata.cmd;
		txdata.buf[1] = cmddata.n[0]; 	//register
		txdata.buf[2] = cmddata.n[1];    //value
		txdata.buf[3] = calculateBufChkSum(3);    //cmd,reg,value
		txdata.len = 4;
		ret = 1;

	}else
		ret = 0;

	return ret;
}

uns8 enable_dle(uns8 ch){
	uns8 dle;
	dle = 0;

	if(ch == STX) dle = 1;
	else if(ch == ETX) dle = 1;
	else if(ch == DLE) dle = 1;

	return dle;
}

uns8 calculateBufChkSum(uns8 len){
	uns8 i,sum;
	
	sum=0;
	for(i=0; i<len; i++) sum += txdata.buf[i];
	return sum;
}

uns8 getbx(uns8 n0,uns8 n1){
	uns8 bx;
	bx =  (n1 & 0xf)<<4;
	bx = bx | (n0 & 0xf);
	return bx;
}