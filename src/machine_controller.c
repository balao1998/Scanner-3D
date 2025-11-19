
#include "machine_controller.h"
#include "adc.h"
#include "motor_control.h"
#include "trignometric_values.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>

static Point3f m_points[MAX_POINTS];
static uint8_t m_points_size = 0;

static uint32_t m_tower_steps = 0;
static uint32_t m_tower_turns = 0;
static uint32_t m_base_steps = 0;
static uint8_t m_trig_index = 0;

uint8_t machine_run() {
  get_point();
  machine_motors_steps();

  // send to pc
  if (m_points_size >= MAX_POINTS) {
    send_points();

    // Set to 0 to fill array again
    m_points_size = 0;
  }

  return m_tower_steps >= MAX_TOWER_STEPS;
}

void machine_motors_steps() {
  motor_steps(BASE_MOTOR, BASE_STEPS_PER_RUN);
  // motor_step(BASE_MOTOR);
  m_base_steps += BASE_STEPS_PER_RUN;
  ++m_trig_index;

  if (m_trig_index >= sizeof(cos_angle_values)) { // Full turn 360º
    m_trig_index = 0;
    motor_steps(TOWER_MOTOR, TOWER_STEPS_PER_RUN);
    m_tower_steps += TOWER_STEPS_PER_RUN;
    ++m_tower_turns;
  }
}

uint8_t machine_reset() {
  m_tower_steps = 0;
  m_base_steps = 0;
  m_points_size = 0;
  m_tower_turns = 0;
  m_trig_index = 0;

  // Send tower to reset! steping until it reaches base! (TODO)
  return motor_home();
}

// void get_point() {
//   // Move base and get X coordinate
//   motor_steps(BASE_MOTOR, BASE_STEPS_PER_RUN);
//   m_base_steps += BASE_STEPS_PER_RUN;
//   float x = m_base_steps * STEP_SIZE_MM;
//
//   // Move tower and get Y coordinate
//   motor_steps(TOWER_MOTOR, TOWER_STEPS_PER_RUN);
//   m_tower_steps += TOWER_STEPS_PER_RUN;
//   float y = m_tower_steps * STEP_SIZE_MM;
//
//   uint16_t value = adc_read();
//
//   char buffer[1024];
//   snprintf(buffer, sizeof(buffer), "Value of adc : %d\n", value);
//   // uart_send_string(buffer);
//
//   // Add to array of points
//   // m_points[m_points_size++] = (Point3f){.x = x, .y = y, .z = z};
// }

void get_point() {
  uint16_t value = adc_read();
  if (value <= IGNORE_ADC_VALUE) {
    uart_send_string("Invalid value\n");
    return;
  }

  // char buffer[1024];
  // snprintf(buffer, sizeof(buffer), "Value of adc : %d\n", value);
  // uart_send_string(buffer);

  float z = m_tower_turns * MM_PER_TOWER_TURN;

  float object_radius = value - CENTER_DISTANCE;

  float x = object_radius * sen_angle_values[m_trig_index];
  float y = object_radius * cos_angle_values[m_trig_index];

  m_points[m_points_size++] = (Point3f){.x = x, .y = y, .z = z};
}

void send_points() {
  for (uint8_t i = 0; i < m_points_size; ++i) {
    uart_send_point(&m_points[i]);
  }
}
