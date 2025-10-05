
#include "machine_controller.h"
#include "motor_control.h"
#include "adc.h"
#include "uart.h"

#define STEP_SIZE_MM 2.0 / 200.0
#define BASE_STEPS_PER_RUN 5
#define TOWER_STEPS_PER_RUN 10 

void machine_run() {

    get_point();

    //send to pc
    if(m_points_size >= MAX_POINTS) {
        send_points();
        //Set to 0 to fill array again
        m_points_size = 0;
    }

    //Set size to 0

}

void machine_reset() {
    m_tower_steps = 0;
    m_base_steps = 0;
    m_points_size = 0;

    //Send tower to reset! steping until it reaches base! (TODO)
}

void get_point() {
    //Move base and get X coordinate
    motor_steps(BASE_MOTOR, BASE_STEPS_PER_RUN);
    m_base_steps += BASE_STEPS_PER_RUN;
    float x = m_base_steps * STEP_SIZE_MM; 
    
    //Move tower and get Y coordinate
    motor_steps(TOWER_MOTOR, TOWER_STEPS_PER_RUN);
    m_tower_steps += TOWER_STEPS_PER_RUN;
    float y = m_tower_steps * STEP_SIZE_MM;

    //Read sensor and get Z coordinate
    float z = adc_read_mm();

    //Add to array of points
    m_points[m_points_size++] = (Point3f) {.x = x, .y = y, .z = z};
}

void send_points() {
    for(uint8_t i = 0; i < m_points_size; ++i) {
        uart_send_point(&m_points[i]);
    }
}