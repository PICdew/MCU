## 32U4

### ATmega16U4/ATmega32U4

* RSIC
    * 32x8 General Purpose Working Register
    * Up to 16 MIPS
* FLASH
    * 16/32K Flash
    * 1.25/2.5KB Internal SRAM
    * 512Byte/1KB Internal EEPROM

* 2x 16-bit Timer/Counter
* 4x 8bit PWM 
* 12x 10bit ADC
* 2.7 - 5.5V
* PortB,C,D,E,F

### Memory Mapping

two main memory
* Data Memory spcae 
* Program Memory space -> FLASH

![Imgur](http://i.imgur.com/lUZnXDm.png)


### 7688 flash MCU

avrdude -c linuxgpio -C /etc/avrdude.conf -p m32u4 -v -e -U flash:w:/root/ex0-blink.hex

## Other

*[32U4_Breakout_Board][1]

[1]:https://github.com/sparkfun/32U4_Breakout_Board
