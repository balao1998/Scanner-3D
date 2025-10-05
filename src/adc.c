#include "adc.h"


void adc_init(void) {
	ADMUX = 0b01100000;			//	REFS0 a 1, referencia 5vdc interna ADLAR A 1 AJUSTA A ESQUERDA
	ADCSRA = 0b10000111;		//	ADEN A 1 ADC ENABLE, DIVISOR DE CLOCK 128 CONFIGURAÇAO DESTE ADC É MAIS ESTÁVEL ENTRE OS 50KHz E OS 200KHz
}

uint8_t adc_read(void) {
	int res = 0;
	
	for (uint8_t i=0; i < NUMBER_OF_READS; ++i)
	{
		ADCSRA |= (1 << ADSC);
		while (ADCSRA & (1 << ADSC));
		res += ADCH;
	}
	return res / NUMBER_OF_READS;
}

float adc_read_mm() {
    uint8_t adc_value = adc_read();
    return (adc_value * ADC_MAX_MM / 255) + ADC_MIN_MM;
}