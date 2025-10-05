

#include <util/delay.h>
#include "motor_control.h"
#include "macros.h"

void init_motors() {
    //TODO (Balao) Configure ports for tower and base motors
}

void motor_step(uint8_t motor) {
    switch (motor) {
        case BASE_MOTOR:
            SET_BIT(MOTORS_PORT, BASE_MOTOR_STEP_PIN);
            _delay_us(2);
            CLEAR_BIT(MOTORS_PORT, BASE_MOTOR_STEP_PIN);
            break;
        case TOWER_MOTOR:
            SET_BIT(MOTORS_PORT, TOWER_MOTOR_STEP_PIN);
            _delay_us(2);
            CLEAR_BIT(MOTORS_PORT, TOWER_MOTOR_STEP_PIN);
            break;
    }
}

void motor_steps(uint8_t motor, uint16_t n_steps) {
    for (uint16_t i = 0; i < n_steps; ++i) {
        motor_step(motor);
        _delay_us(2);
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
