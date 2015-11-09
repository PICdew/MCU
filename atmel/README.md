## ATMEL MCU Example

- [WinAVR使用指南][2]

在Windows平台上，免費的AVR編程軟體主要有兩個：AVR Studio和WinAVR。他們都使用GCC作為編譯器。前者具有一個完整的IDE和軟體硬體，但體積臃腫；後者簡單高效，但操作較為複雜。

- USBASP win7 32bit driver 及燒錄方式

driver 請選用usbasp_driver_and_Progisp_and_AVR_fighter/usbasp/usbasp/driver

請使用Progisp燒錄

目前使用AVR_fighter會遇到Flash校驗失敗問題


- 請先下載[WinAVR-20100110][1]
或是由atmel tools目錄內下載

### ATMEL Example使用方式

- make 產生.bin

- make clean：清除bin


## Makefile

- ATTINY85 Makefile

 [參考來源][3]


[1]:http://sourceforge.net/projects/winavr/
[2]:https://zh.wikibooks.org/zh-tw/WinAVR使用指南
[3]:https://gist.github.com/electronut/8a4c297213620958ebef