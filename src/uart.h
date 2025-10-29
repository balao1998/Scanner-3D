#pragma once

#include "Point3f.h"
#include <avr/interrupt.h>

#define MESSAGE_HEADER 0xAA
#define POINT_HEADER 0xBB
#define UBRR_VALUE 16

void uart_init();
void uart_send_byte(uint8_t data);
void uart_send_point(const Point3f *point);
void uart_send_string(const char *message);
