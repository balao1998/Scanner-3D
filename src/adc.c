#include "adc.h"
#include <stdint.h>

void adc_init(void) {
  ADMUX = 0b01000000; //	REFS0 a 1, referencia 5vdc interna ADLAR A 0
                      // AJUSTA A DIREITA
  ADCSRA =
      0b10000111; //	ADEN A 1 ADC ENABLE, DIVISOR DE CLOCK 128 CONFIGURAÇAO
                  // DESTE ADC É MAIS ESTÁVEL ENTRE OS 50KHz E OS 200KHz
}

uint16_t adc_read(void) {
  uint32_t res = 0;

  for (uint16_t i = 0; i < NUMBER_OF_READS; ++i) {
    ADCSRA |= (1 << ADSC); // Start conversion
    while (ADCSRA & (1 << ADSC))
      ; // Wait for completion

    res += ADC; // Read 10-bit result (ADCL+ADCH)
  }

  return res / NUMBER_OF_READS; // Still 10-bit (0–1023)
}
