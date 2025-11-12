
#include "Point3f.h"
#include "uart.h"
#define F_CPU 16000000UL // Clock de 16 MHz
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "adc.h"
#include "macros.h"
#include "motor_control.h"

enum StateMachine { STAND_BY, PRODUCTION, RESETING };

void init_timer_1ms(void);

void pwm_init(void); // FAST PWM A 976.56Hz
void init();

// Function for main loop
void loop(void);

// Counters for time events
volatile uint16_t counter_10ms = 0;
volatile uint8_t counter_1Hz = 0;

// State of machine
enum StateMachine m_state = STAND_BY;

ISR(TIMER2_COMPA_vect) { counter_10ms++; }

int main(void) {
  init();
  loop();
}

void init() {
  init_timer_1ms();
  // adc_init();
  // pwm_init();
  motors_init();
  // uart_init();
  sei();
}

void loop() {
  motor_change_direction(BASE_MOTOR, UP);
  while (1) {
    // if (counter_10ms >= 1) {
    //   counter_10ms = 0;
    // counter_1Hz++;
    // uint8_t value = adc_read();
    // OCR1A = value;
    // Point3f point = {69, 42.5f, 123.3f};
    // uart_send_point(&point);

    // TOGGLE_BIT(PORTB, PB0);

    motor_home();
    // motor_step(BASE_MOTOR);
    // _delay_us(STEP_DELAY_US);
    // }
  }
}

void init_timer_1ms() {
  TCCR2A = 0b00000010; // WGM21 CTC
  TCCR2B = 0b00000101; // PRESCALLER DE 128
  OCR2A = 124;         //(16000000/128)=125KHz T=8us (1000/8)=125
  TIMSK2 = 0b00000010; // OCIE2A ATIVA INTERRUPT EM COMPARE MATCH
}

void pwm_init(void) {
  // DDRB |= (1 << PB1); // PB1 como saída
  //
  // TCCR1A = 0b10000001; // COM1A1 A 1 OC1A ON COMPARE MATCH NÃO INVERTIDO
  // WGM10 A
  //                      // 1 PARA FAST PWM 8 BITS
  // TCCR1B = 0b00001011; // WGM12 A 1 PARA FAST PWM 8 BITS PRESCALLER 64
}
