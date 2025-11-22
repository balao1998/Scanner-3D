#pragma once

#include <avr/io.h>

#define NUMBER_OF_READS 800

void adc_init(void);
uint16_t adc_read(void);
