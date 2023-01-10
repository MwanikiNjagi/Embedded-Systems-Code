#define F_CPU 16000000UL // CPU clock frequency
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB |= (1 << 5); // Set pin 13 (Arduino Uno) as output

    while (1) {
        PORTB |= (1 << 5); // Set pin 13 high (LED on)
        _delay_ms(5000);   // Wait for 5 seconds
        PORTB &= ~(1 << 5); // Set pin 13 low (LED off)
        _delay_ms(5000);   // Wait for 5 seconds
    }

    return 0;
}