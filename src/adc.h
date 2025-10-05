#pragma once

#include <avr/io.h>

#define NUMBER_OF_READS 8
#define ADC_MAX_MM 30.0f
#define ADC_MIN_MM 3.0f

void adc_init(void);
uint8_t adc_read(void);
float adc_read_mm();