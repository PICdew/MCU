//Start of Text
//End of Text
//Data Link Escape


#define RXBUF_MAX	10
#define CMD_SIZE    1
#define VALUE_SIZE  3

#define STX		0x0f
#define ETX		0x04
#define DLE     0x05

/*
          N1
          N80[43]   
 N2                 N0
N80[765]            N80[210]
          N3
          N81[3210]
*/

//
// <STX> <CMD><VALUE> <CHK> <ETX>
//  <1>  <1><3>       <1>   <1>

//CMD:SET ,set nx value
//CMD:CLEAR , clear nx VALUE
//
//SETN0,XX
//SETN80,X
//SETN81,X
//SETN82,X
//SETN1,XX
//SETN2,XX
//SETN3,XX

#define CLEAR0_ALL		0x40
#define CLEAR0_N80		0x41
#define CLEAR0_N81		0x42
#define CLEAR0_N0    	0x43
#define CLEAR0_N1    	0x44
#define CLEAR0_N2    	0x45
#define CLEAR0_N3    	0x46

#define CLEAR_ALL		0x50
#define CLEAR_N80		0x51
#define CLEAR_N81		0x52
#define CLEAR_N0    	0x53
#define CLEAR_N1    	0x54
#define CLEAR_N2    	0x55
#define CLEAR_N3    	0x56

#define SET_N0      	0x60
#define SET_N1      	0x61
#define SET_N2      	0x62
#define SET_N3      	0x63

#define SET_N80_0       0x70
#define SET_N80_1       0x71
#define SET_N80_2       0x72
#define SET_N80_3      	0x73
#define SET_N80_4     	0x74
#define SET_N80_5     	0x75
#define SET_N80_6     	0x76
#define SET_N80_7     	0x77

#define SET_N81_0     	0x78
#define SET_N81_1     	0x79
#define SET_N81_2     	0x7a
#define SET_N81_3     	0x7b






