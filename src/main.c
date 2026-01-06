
#define F_CPU 16000000UL // Clock de 16 MHz
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "adc.h"
#include "machine_controller.h"
#include "macros.h"
#include "motor_control.h"
#include "uart.h"

enum StateMachine {
  STAND_BY,
  RUNNING,
  RESETING,
  RESET_TO_RUN,
  RESET_TO_CALIBRATE,
  CALIBRATING
};

void init_timer_1ms(void);

void pwm_init(void);
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
  adc_init();
  pwm_init();
  motors_init();
  uart_init();
  sei();
}

void check_state() {
  if (uart_available()) {
    const uint8_t incoming_char = uart_read();

    switch (incoming_char) {
    case 'P':
    case 'p':
      m_state = STAND_BY;
      uart_send_string("STANDING_BY");
      break;
    case 'S':
    case 's':
      m_state = RESET_TO_RUN;
      uart_send_string("RESETTING TO RUN\n");
      break;
    case 'H':
    case 'h':
      m_state = RESETING;
      uart_send_string("RESETTING\n");
      break;
    case 'c':
    case 'C':
      m_state = RESET_TO_CALIBRATE;
      uart_send_string("CALIBRATING\n");
      break;
    }
  }
}

void loop() {

  while (1) {
    check_state();
    switch (m_state) {
    case STAND_BY:
      // motor_steps(BASE_MOTOR, 200);
      break;
    case RUNNING:
      if (!machine_calibrated()) {
        machine_calibration();
        m_state = RESET_TO_RUN;
      }
      if (machine_run()) {
        m_state = STAND_BY;
      }
      break;
    case RESETING:
      if (machine_reset()) {
        m_state = STAND_BY;
      }
      break;
    case RESET_TO_RUN:
      if (machine_reset()) {
        m_state = RUNNING;
        machine_go_to_start();
      }
      break;
    case CALIBRATING:
      machine_go_to_start();
      machine_calibration();
      m_state = STAND_BY;
      break;
    case RESET_TO_CALIBRATE:
      if (machine_reset()) {
        m_state = CALIBRATING;
      }
      break;
    }
  }
}

void init_timer_1ms() {
  TCCR2A = 0b00000010; // WGM21 CTC
  TCCR2B = 0b00000101; // PRESCALLER DE 128
  OCR2A = 124;         //(16000000/128)=125KHz T=8us (1000/8)=125
  TIMSK2 = 0b00000010; // OCIE2A ATIVA INTERRUPT EM COMPARE MATCH
}

void pwm_init(void) {
  DDRB |= (1 << PB1);     // PB1 como saída
  OCR1A = 31249;          // Define meio ciclo (0.5s com prescaler 256)
  TCCR1A = (1 << COM1A0); // Configura para INVERTER (Toggle) o pino PB1
  TCCR1B = (1 << WGM12) | (1 << CS12); // Modo CTC + Prescaler 256
}
