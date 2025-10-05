#include <avr/io.h>

#define LEFT 0
#define RIGHT 1

#define BASE_MOTOR 0
#define TOWER_MOTOR 1

#define MOTORS_PORT PORTB

#define BASE_MOTOR_STEP_PIN PB0
#define BASE_MOTOR_DIR_PIN PB1
#define TOWER_MOTOR_STEP_PIN PB2
#define TOWER_MOTOR_DIR_PIN PB3


void motors_init();
void motor_step(uint8_t motor);
void motor_steps(uint8_t motor, uint16_t n_steps);
void motor_change_direction(uint8_t motor, uint8_t direction);

