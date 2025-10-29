#include "uart.h"
#include <stdio.h>

void uart_init() {
  // Set baud rate to 9600
  UBRR0H = (UBRR_VALUE >> 8); // High byte
  UBRR0L = UBRR_VALUE & 0xFF; // Low byte

  // Enable transmitter and receiver
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);

  // Set frame format: 8 data bits, 1 stop bit, no parity
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_send_byte(uint8_t data) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;

  UDR0 = data;
}

void uart_send_point(const Point3f *point) {
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%.2f,%.2f,%.2f\r\n", point->x, point->y,
           point->z);

  uart_send_string(buffer);
}

// Must send with a \n terminator to delimeter the msg :) !!!
void uart_send_string(const char *message) {
  while (*message) {
    uart_send_byte(*message++);
  }
}
