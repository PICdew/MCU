avrdude -Cavrdude.conf -v -q -q -patmega32u4 -cavr109 -P/dev/ttyACM0 -b57600 -D -Uflash:w:ArduinoISP.cpp.hex:i
