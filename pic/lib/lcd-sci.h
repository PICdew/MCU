//Start of Text
//End of Text
//Data Link Escape


#define RXBUF_MAX	10
#define CMD_SIZE    1

#define STX		0x0f
#define ETX		0x04
#define DLE     0x05

//
// <STX> <CMD><VALUE>   <CHK> <ETX>
//  <1>  <1><max 4byte>  <1>   <1>



/*
          N1
          CHIP1[43]   
 N2                 N0
CHIP1[765]            CHIP1[210]
          N3
          CHIP2[3210]


CMD : DISPLAY-CHIP1 (8number) / DISPLAY-CHIP2 (4number)

<01>  <23>  <45>  <67>     4byte
<0xFF><0xFF><0xFF><0xFF>
0-9 : display number
0xA : dp display
0xB : clear
0xC :
0xD :
0xE :
0xF : don't care

CMD : REGGIST-CHIP1
<value> 1byte 

CMD : REGISTER-CHP2
<value> 1byte
*/



#define CHIP1NUM    	0x60
#define CHIP2NUM    	0x61
#define CHIP1REG    	0x62
#define CHIP2REG    	0x63

#define NUM_DP			0xA
#define NUM_CLEAR       0xB
#define NUM_NULL        0xF