## Attiny85

* [Tiny AVR Programmer Hookup Guide ](https://learn.sparkfun.com/tutorials/tiny-avr-programmer-hookup-guide/attiny85-use-hints)

### test2

![Imgur](http://i.imgur.com/lC3p7S4.png)
* ATtiny13
ATtiny13 
64 Bytes EEPROM
64 Bytes Internal SRAM
1K Bytes Flash
One 8-bit Timer/Counter with Prescaler and Two PWM Channels
4-channel, 10-bit ADC

![Imgur](http://i.imgur.com/cxAwEa2.png)


![Imgur](http://i.imgur.com/r3lvQjF.png)
![Imgur](http://i.imgur.com/6AjjRXt.png)

Data Register– PORTx
Data Direction Register – DDRx
Port Input Pins – PINx
Pull-up Disable – PUD bit in MCUCR disables thepull-up function for all pins

![Imgur](http://i.imgur.com/N1hKadQ.png)
輸入CLK:Tn
輸出OCnA/OCnB
當輸入CLK(Tn)產生TCNTn計數，計數到跟暫存器值(OCRnA/OCRnB)相同或為0，此時輸出產生變化 

![Imgur](http://i.imgur.com/4Fcb8yY.png)

The Timer/Counter (TCNT0) and Output Compare Registers (OCR0A and OCR0B) are 8-bit registers
The double buffered Output Compare Registers (OCR0A and OCR0B) 


