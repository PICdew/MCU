#include <SoftwareSerial.h>

SoftwareSerial SOBDUart(9,10);  //RX,TX
char buf[40];

//This is a character buffer that will store the data from the serial port
char rxData[40];
char rxIndex=0;
int  rxCnt;

void setup() {
  // put your setup code here, to run once:

  SOBDUart.begin(38400);  //OBD default baud rate

  Serial.begin(115200);
  Serial1.begin(57600);
  pinMode(13, OUTPUT);

  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  int b=0;
  int cnt=0;
  unsigned char c;
  int i;
  char inChar;

  int v = Serial1.read();
    if (v != -1) {
        switch(v) {
           case '0':                // turn off D13 when receiving "0"
              digitalWrite(13, 0);
              break;
           case '1':                // turn on D13 when receiving "1"
              digitalWrite(13, 1);
              break;
        }
    }  

  if( (v== '0' ) || (v== '1' ) ){
  

    //Delete any data that may be in the serial port before we begin.
    SOBDUart.flush();
  
    Serial.println("ATZ");
    SOBDUart.println("ATZ");  //reset OBD, will response version
    delay(5000);
    memset(buf,0,40);

    rxIndex=0;
    while(SOBDUart.available()>0){
      inChar=SOBDUart.read();
      rxData[rxIndex++]=inChar;

      if(inChar == 0x0a){

        //sprintf(buf,"[size:%d]:",rxIndex,rxData[i],rxData[i]); 
        //Serial.print(buf);
        //for(i=0; i<rxIndex; i++){
        //  memset(buf,0,40);
        //  sprintf(buf,"(%c,0x%x)",rxData[i],rxData[i]); 
        //  Serial.print(buf);
        //}
        //Serial.println("");
        
        rxData[rxIndex]='\0';
        memset(buf,0,40);
        sprintf(buf,"[cnt:%d]:rxData:%s",cnt,rxData); 
        Serial.print(buf);
        cnt++;
        rxIndex=0;
      }
      
    }
/*    
    getResponse();

    for(i=0; i<rxCnt; i++){
      sprintf(buf,"[%d]:%c,%x",i,rxData[i],rxData[i]); 
    }
    sprintf(buf,"cnt:%d,rxData:%s",cnt,rxData); 
    Serial.println(buf);

    memset(buf,0,40);
    getResponse();

    for(i=0; i<rxCnt; i++){
      sprintf(buf,"[%d]:%c,%x",i,rxData[i],rxData[i]); 
    }
    sprintf(buf,"cnt:%d,rxData:%s",rxData,cnt); 
    Serial.println(buf);
*/    
    cnt++;    
 }
}

//https://github.com/sparkfun/OBD-II_UART
//The getResponse function collects incoming data from the UART into the rxData buffer
// and only exits when a carriage return character is seen. Once the carriage return
// string is detected, the rxData buffer is null terminated (so we can treat it as a string)
// and the rxData index is reset to 0 so that the next string can be copied.
void getResponse(void){
  char inChar=0;
  rxCnt=0;
  //Keep reading characters until we get a carriage return
  while(inChar != '\r'){
    //If a character comes in on the serial port, we need to act on it.
    if(SOBDUart.available() > 0){
      //Start by checking if we've received the end of message character ('\r').
      if(SOBDUart.peek() == '\r'){
        //Clear the Serial buffer
        inChar=SOBDUart.read();
        //Put the end of string character on our data string
        rxData[rxIndex]='\0';
        rxCnt = rxIndex;
        //Reset the buffer index so that the next character goes back at the beginning of the string.
        rxIndex=0;
      }
      //If we didn't get the end of message character, just add the new character to the string.
      else{
        //Get the new character from the Serial port.
        inChar = SOBDUart.read();
        //Add the new character to the string, and increment the index variable.
        rxData[rxIndex++]=inChar;
      }
    }
  }
}
