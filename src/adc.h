#pragma once

#include <avr/io.h>

#define NUMBER_OF_READS 80
#define ADC_MAX_MM 30.0f
#define ADC_MIN_MM 3.0f
#define IGNORE_ADC_VALUE 327

void adc_init(void);
uint16_t adc_read(void);
float adc_read_mm();
