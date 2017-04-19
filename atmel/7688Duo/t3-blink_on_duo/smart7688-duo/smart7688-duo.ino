void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial1.begin(57600);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int c = Serial1.read();
    if (c != -1) {
        switch(c) {
           case '0':                // turn off D13 when receiving "0"
              digitalWrite(13, 0);
              break;
           case '1':                // turn on D13 when receiving "1"
              digitalWrite(13, 1);
              break;
          case '2':
              Serial.print("Hello world");
              Serial1.print("Hello world OpenWrt\n");
              break;
        }
    }
}
