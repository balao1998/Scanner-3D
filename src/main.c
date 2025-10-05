
#define F_CPU 16000000UL		// Clock de 16 MHz
#include <avr/io.h>
#include <avr/interrupt.h>

#include "motor_control.h"
#include "macros.h"

enum StateMachine {
	STAND_BY,
	PRODUCTION,
	RESETING
};

void init_timer_1ms(void);
void adc_init(void);
void pwm_init(void); // FAST PWM A 976.56Hz
void init();


// TODO (Balao) Move adc to its own file!
uint8_t adc_read(void);

//Function for main loop
void loop(void);

//Counters for time events
volatile uint8_t counter_10ms = 0;
volatile uint8_t counter_1Hz = 0;

//State of machine
enum StateMachine m_state = STAND_BY;


ISR(TIMER2_COMPA_vect) {
	counter_10ms++;
}

int main(void) {
	init();
	loop();
}

void init() {
	init_timer_1ms();
	adc_init();
	pwm_init();
	init_motors();
	sei();
}

void loop() {
	while (1) {
		if (counter_10ms == 10){
			counter_10ms = 0;
			counter_1Hz++;
			uint8_t value = adc_read();
			OCR1A = value;
		}
		if (counter_1Hz == 50){
			counter_1Hz = 0;
			TOGGLE_BIT(PORTB,PB0);  //PORTB ^= (1 << PB0);
		}
	}
}

void init_timer_1ms(){
	TCCR2A = 0b00000010;		//WGM21 CTC
	TCCR2B = 0b00000101;		// PRESCALLER DE 128
	OCR2A = 124;				//(16000000/128)=125KHz T=8us (1000/8)=125
	TIMSK2 = 0b00000010;		//OCIE2A ATIVA INTERRUPT EM COMPARE MATCH
	DDRB |= (1 << PB0);			//1Hz Led Pin
}

void adc_init(void) {
	ADMUX = 0b01100000;			//	REFS0 a 1, referencia 5vdc interna ADLAR A 1 AJUSTA A ESQUERDA
	ADCSRA = 0b10000111;		//	ADEN A 1 ADC ENABLE, DIVISOR DE CLOCK 128 CONFIGURAÇAO DESTE ADC É MAIS ESTÁVEL ENTRE OS 50KHz E OS 200KHz
	
}

void pwm_init(void) {
	DDRB |= (1 << PB1);				// PB1 como saída

	TCCR1A = 0b10000001;			// COM1A1 A 1 OC1A ON COMPARE MATCH NÃO INVERTIDO WGM10 A 1 PARA FAST PWM 8 BITS
	TCCR1B = 0b00001011;			//WGM12 A 1 PARA FAST PWM 8 BITS PRESCALLER 64
}

uint8_t adc_read(void) {
	unsigned char i;
	int res = 0;
	
	for (i=0; i<8; i++)
	{
		ADCSRA |= (1 << ADSC);
		while (ADCSRA & (1 << ADSC));
		res += ADCH;
	}
	return res/8;
}

