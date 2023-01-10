#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define GPS_BAUD 9600
#define UBRR_VAL ((F_CPU/(GPS_BAUD * 16UL)) - 1)

void uart_init() {
    UBRR0H = UBRR_VAL >> 8;
    UBRR0L = UBRR_VAL;
    UCSR0B = (1<<RXEN0) | (1<<TXEN0); // Enable receiver and transmitter
    UCSR0C = (3<<UCSZ00); // 8-bit data
}

void uart_putchar(char data) {
    while (!(UCSR0A & (1<<UDRE0))); // Wait until buffer empty
    UDR0 = data; // Send data
}

char uart_getchar() {
    while (!(UCSR0A & (1<<RXC0))); // Wait for data to be received
    return UDR0; // Get data from buffer
}

void parse_gps(char *buf, int *latitude, int *longitude) {
    // Parse the latitude and longitude from the GPS data
    char *p = buf;
    char *lat = strstr(p, ",");
    if (lat) {
        *lat = '\0';
        *latitude = atoi(p);
        p = lat + 1;
    }
    char *lat_dir = strstr(p, ",");
    if (lat_dir) {
        p = lat_dir + 1;
    }
    char *lon = strstr(p, ",");
    if (lon) {
        *lon = '\0';
        *longitude = atoi(p);
    }
}

int main(void) {
    uart_init();
    int latitude = 0, longitude = 0;
    char buf[100];
    int i = 0;

    while (1) {
        char c = uart_getchar();
        if (c == '$') { // Start of sentence
            memset(buf, 0, sizeof(buf));
            i = 0;
        } else if (c == '\r') { // End of sentence
            buf[i] = '\0';
            if (strstr(buf, "GPRMC")) {
                parse_gps(buf, &latitude, &longitude);
                printf("Latitude: %d, Longitude: %d\n", latitude, longitude);
            }
        } else if (i < sizeof(buf) - 1) {
            buf[i++] = c;
        }
    }
}