// main.c
//
// A simple blinky program for ATtiny85
// Connect red LED at pin 2 (PB3)
//
// electronut.in

#include <avr/io.h>
#include <util/delay.h>


int main (void)
{
  // set PC7 to be output
	DDRC = 0b10000000;
  while (1) {

    // flash# 1:
    // set PC7 high
    PORTC = 0b10000000;
    _delay_ms(20);
    // set PC7 low
    PORTC = 0b00000000;
    _delay_ms(20);

    // flash# 2:
    // set PC7 high
    PORTC = 0b10000000;
    //_delay_ms(200);
    // set PC7 low
    PORTC = 0b00000000;
    _delay_ms(200);
    }
}
