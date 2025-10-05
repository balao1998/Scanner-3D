#pragma once


#include <avr/interrupt.h>
#include "Point3f.h"

void uart_init();
void uart_send_byte(uint8_t data);
void uart_send_point(const Point3f * point);
void uart_send_string(const char * message);