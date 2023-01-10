#define F_CPU 16000000UL // CPU clock frequency
#include <avr/io.h>

const uint8_t segmentPins[8] = { 2, 3, 4, 5, 6, 7, 8, 9 }; // segment pins
const uint8_t digitPins[4] = { 10, 11, 12, 13 };         // digit pins
const uint8_t numMap[10] = { // mapping of digits to segments
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

void setup() {
    // configure segment pins as outputs
    for (uint8_t i = 0; i < 8; i++) {
        DDRD |= (1 << segmentPins[i]);
    }
    // configure digit pins as outputs
    for (uint8_t i = 0; i < 4; i++) {
        DDRD |= (1 << digitPins[i]);
    }
}

void loop() {
    for (uint8_t digit = 0; digit < 4; digit++) {
        // turn off all digits
        for (uint8_t i = 0; i < 4; i++) {
            PORTD &= ~(1 << digitPins[i]);
        }
        // turn on current digit
        PORTD |= (1 << digitPins[digit]);
        // send segments data
        for (uint8_t i = 0; i < 8; i++) {
            PORTD = (PORTD & ~(1 << segmentPins[i])) | (numMap[digit] & (1 << i));
        }
        // delay between digits
        _delay_ms(2);
    }
}