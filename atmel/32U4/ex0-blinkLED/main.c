/* Created 05/22/2012 by Jordan McConnell at Sparkfun Electronics
 * This code is beerware, you know what I'm sayin'?
 *
 * Built on WinXP SP3 and WinAVR-20100110, AVRDUDE 5.10
 *
 * This code is a simple example of digital input for Sparkfun's
 * 32U4 Breakout Board using C and standard AVR libraries.  It
 * teaches you how to read the status of a digital pin to determine
 * whether its current status is HIGH or LOW.
 *
 * Pin PF0 is used as a digital input.  If its status is HIGH,
 * pin PD5 toggles each second, if it's status is LOW, pin PD6
 * toggles each second instead.
 *
 * The user can connect LED's to pins PD5 and PD6 or use a multimeter
 * to verify operation.  If pin PF0 is left unconnected, it's status
 * will be HIGH due to internal pullup resistors, and PD5 toggles.
 * If PF0 is connected to ground, PD6 toggles each second instead.
 */

// Libraries for register names and the delay function
#include <avr/io.h>
#include <util/delay.h>

// Macros to make bit manipulation easier
#define set_bit(address,bit) (address |= (1<<bit))
#define clear_bit(address,bit) (address &= ~(1<<bit))
#define toggle_bit(address,bit) (address ^= (1<<bit))

// This macro is for checking if a certain bit is set in a given register.
// This is useful here for checking the status of individual input pins.
#define check_bit(address,bit) ((address & (1<<bit)) == (1<<bit))

int main(void)
{
    set_bit(DDRC,7); // Pin PC7 is now configured as an OUTPUT

    while(1)
    {
        set_bit(PORTC,7);
        _delay_ms(1000); 

        clear_bit(PORTC,7);
        _delay_ms(1000); 
    }

    return 0;
}
