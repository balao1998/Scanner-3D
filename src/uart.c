#include "uart.h"

#define MESSAGE_HEADER 0xAA
#define POINT_HEADER 0xBB
#define UBRR_VALUE  103

void uart_init() {
    // Set baud rate to 9600
    UBRR0H = (UBRR_VALUE >> 8);       // High byte
    UBRR0L = UBRR_VALUE & 0xFF;       // Low byte

    // Enable transmitter and receiver
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_send_byte(uint8_t data) {
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void uart_send_point(const Point3f * point) {
    uart_send_byte(POINT_HEADER);
    uint8_t * ptr = (uint8_t *) point;
    for(uint8_t i = 0; i < sizeof(Point3f) ; ++i) {
        uart_send_byte(ptr[i]);
    }
    uart_send_byte('\n');
}

//Must send with a \n terminator to delimeter the msg :) !!!
void uart_send_string(const char * message) {
    uart_send_byte(MESSAGE_HEADER);
    
    while(*message) {
        uart_send_byte(*message++);
    }
}