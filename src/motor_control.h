#include <avr/io.h>

#define LEFT 0
#define RIGHT 1

#define DOWN 0
#define UP 1

#define BASE_MOTOR 0
#define TOWER_MOTOR 1

#define MOTORS_PORT PORTB

#define BASE_MOTOR_STEP_PIN PB0
#define BASE_MOTOR_DIR_PIN PB2
#define TOWER_MOTOR_STEP_PIN PB3
#define TOWER_MOTOR_DIR_PIN PB4

#define TOWER_HOME_PIN PC5
#define TOWER_HOME_PORT PINC

#define STEP_DELAY_US 1250

void motors_init();
void motor_step(uint8_t motor);
void motor_steps(uint8_t motor, uint16_t n_steps);
void motor_change_direction(uint8_t motor, uint8_t direction);
uint8_t motor_home();
