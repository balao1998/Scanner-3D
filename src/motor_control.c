

#include "motor_control.h"
#include "macros.h"
#include <util/delay.h>

void motors_init() {
  // TODO (Balao) Configure ports for tower and base motors
  DDRB = 0b00111111;
  PORTB = 0b1100000;

  CLEAR_BIT(DDRC, TOWER_HOME_PIN);
  SET_BIT(PORTC, TOWER_HOME_PIN);
}

void motor_step(uint8_t motor) {
  switch (motor) {
  case BASE_MOTOR:
    SET_BIT(MOTORS_PORT, BASE_MOTOR_STEP_PIN);
    _delay_us(STEP_DELAY_US);
    CLEAR_BIT(MOTORS_PORT, BASE_MOTOR_STEP_PIN);
    break;
  case TOWER_MOTOR:
    SET_BIT(MOTORS_PORT, TOWER_MOTOR_STEP_PIN);
    _delay_us(STEP_DELAY_US);
    CLEAR_BIT(MOTORS_PORT, TOWER_MOTOR_STEP_PIN);
    break;
  }
}

void motor_steps(uint8_t motor, uint16_t n_steps) {
  for (uint16_t i = 0; i < n_steps; ++i) {
    motor_step(motor);
    _delay_us(STEP_DELAY_US);
  }
}

void motor_change_direction(uint8_t motor, uint8_t direction) {
  switch (motor) {
  case BASE_MOTOR:
    if (direction == RIGHT)
      SET_BIT(MOTORS_PORT, BASE_MOTOR_DIR_PIN);
    else
      CLEAR_BIT(MOTORS_PORT, BASE_MOTOR_DIR_PIN);
    break;
  case TOWER_MOTOR:
    if (direction == RIGHT)
      SET_BIT(MOTORS_PORT, TOWER_MOTOR_DIR_PIN);
    else
      CLEAR_BIT(MOTORS_PORT, TOWER_MOTOR_DIR_PIN);
    break;
  }
}

void motor_home() {
  motor_change_direction(TOWER_MOTOR, DOWN);

  while (READ_BIT(TOWER_HOME_PORT, TOWER_HOME_PIN)) {
    motor_step(TOWER_MOTOR);
    _delay_us(STEP_DELAY_US);
  }
}
