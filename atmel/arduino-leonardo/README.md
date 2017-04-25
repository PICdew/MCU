# Arduino Leonardo

* [Arduino Leonardo][1]
* [ATmega32U4][2]

![Imgur](http://i.imgur.com/ZhImqHW.png)


## 使用Arduino Leonardo為燒錄器(ISP)

來源：[Arduino Leonardo As ISP][3]

記得先用arduino燒錄ISP程式
確認board device

arduino-1.6.5-r5

### 設定ArduinoISP範例程式

* 修改下例程式
RESET pin由 SS改為dig10，**記得device接線時需要接dig10**

```
#define RESET 10 //SS
#define LED_HB  13 //9
```


### 測試device

* avrdude -Cavrdude.conf -v -q -q -pattiny85 -cstk500v1 -P/dev/ttyACM0 -b19200

```
$ sudo avrdude -C/xx/avrdude.conf -v -q -q -pattiny85 -cstk500v1 -P/dev/ttyACM0 -b19200

xxx

         Using Port                    : /dev/ttyACM0
         Using Programmer              : stk500v1
         Overriding Baud Rate          : 19200
         AVR Part                      : ATtiny85
         Chip Erase delay              : 400000 us
         PAGEL                         : P00
         BS2                           : P00
         RESET disposition             : possible i/o
         RETRY pulse                   : SCK
         serial program mode           : yes
         parallel program mode         : yes
         Timeout                       : 200
         StabDelay                     : 100
         CmdexeDelay                   : 25
         SyncLoops                     : 32
         ByteDelay                     : 0
         PollIndex                     : 3
         PollValue                     : 0x53
         Memory Detail                 :
xxx
         Programmer Type : STK500
         Description     : Atmel STK500 Version 1.x firmware
         Hardware Version: 2
         Firmware Version: 1.18
         Topcard         : Unknown
         Vtarget         : 0.0 V
         Varef           : 0.0 V
         Oscillator      : Off
         SCK period      : 0.1 us
avrdude: safemode: hfuse reads as DF
avrdude: safemode: efuse reads as FF
avrdude: safemode: hfuse reads as DF
avrdude: safemode: efuse reads as FF
```

### flash device

* sudo make flash

```
$ sudo make flash
avrdude -Cavrdude.conf -v  -cstk500v1 -P/dev/ttyACM0 -b19200 -p attiny85 -U flash:w:main.hex:i
xx

         Using Port                    : /dev/ttyACM0
         Using Programmer              : stk500v1
         Overriding Baud Rate          : 19200
         AVR Part                      : ATtiny85
         Chip Erase delay              : 400000 us
         PAGEL                         : P00
         BS2                           : P00
         RESET disposition             : possible i/o
         RETRY pulse                   : SCK
         serial program mode           : yes
         parallel program mode         : yes
         Timeout                       : 200
         StabDelay                     : 100
         CmdexeDelay                   : 25
         SyncLoops                     : 32
         ByteDelay                     : 0
         PollIndex                     : 3
         PollValue                     : 0x53
         Memory Detail                 :

xxx

         Programmer Type : STK500
         Description     : Atmel STK500 Version 1.x firmware
         Hardware Version: 2
         Firmware Version: 1.18
         Topcard         : Unknown
         Vtarget         : 0.0 V
         Varef           : 0.0 V
         Oscillator      : Off
         SCK period      : 0.1 us

avrdude: AVR device initialized and ready to accept instructions

Reading | ################################################## | 100% 0.05s

avrdude: Device signature = 0x1e930b (probably t85)
avrdude: safemode: hfuse reads as DF
avrdude: safemode: efuse reads as FF
avrdude: NOTE: "flash" memory has been specified, an erase cycle will be performed
         To disable this feature, specify the -D option.
avrdude: erasing chip
avrdude: reading input file "main.hex"
avrdude: writing flash (74 bytes):

Writing | ################################################## | 100% 0.18s

avrdude: 74 bytes of flash written
avrdude: verifying flash memory against main.hex:
avrdude: load data flash data from input file main.hex:
avrdude: input file main.hex contains 74 bytes
avrdude: reading on-chip flash data:

Reading | ################################################## | 100% 0.12s

avrdude: verifying ...
avrdude: 74 bytes of flash verified

avrdude: safemode: hfuse reads as DF
avrdude: safemode: efuse reads as FF
avrdude: safemode: Fuses OK (E:FF, H:DF, L:62)

avrdude done.  Thank you.
```

[1]:https://www.arduino.cc/en/Main/ArduinoBoardLeonardo
[2]:http://www.microchip.com/wwwproducts/en/ATmega32u4
[3]:http://www.instructables.com/id/Arduino-Leonardo-as-Isp/
