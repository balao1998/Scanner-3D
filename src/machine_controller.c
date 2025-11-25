
#include "machine_controller.h"
#include "adc.h"
#include "motor_control.h"
#include "trignometric_values.h"
#include "uart.h"
#include <stdint.h>

static Point3f m_points[MAX_POINTS];
static uint8_t m_points_size = 0;

static uint32_t m_tower_steps = 0;
static uint32_t m_tower_turns = 0;
static uint32_t m_base_steps = 0;
static uint8_t m_trig_index = 0;

static float m_line_slope = 0.0f;
static float m_line_constant = 0.0f;

uint8_t machine_run() {
  get_point();
  machine_motors_steps();

  if (m_points_size >= MAX_POINTS) {
    send_points();
    m_points_size = 0;
  }

  return m_tower_steps >= MAX_TOWER_STEPS;
}

void machine_calibration() {
  motor_steps(TOWER_MOTOR, CALIBRATION_STEP);
  int first_adc_value = adc_read();

  motor_steps(TOWER_MOTOR, CALIBRATION_STEP);
  int second_adc_value = adc_read();

  // y= 55 x=adc_value  float m = (55-40)/(x2 - x1) // constante = 55 - m*x2;
  m_line_slope = (55.0 - 40.0) / (second_adc_value - first_adc_value);

  m_line_constant = 55 - (m_line_slope * second_adc_value);
}

uint8_t machine_calibrated() { return m_line_slope != 0.0f; }

void machine_motors_steps() {
  motor_steps(BASE_MOTOR, BASE_STEPS_PER_RUN);
  m_base_steps += BASE_STEPS_PER_RUN;
  ++m_trig_index;

  if (m_trig_index >= SIZE_OF_TRIG_ARRAY) { // Full turn 360º
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

  // Send tower to reset!
  return motor_home();
}

void get_point() {
  int value = adc_read();
  if (value <= CENTER_DISTANCE) {
    uart_send_string("Invalid value\n");
    return;
  }

  float distance_mm = get_mm_point(value);
  float object_radius_mm = CENTER_DISTANCE - distance_mm;

  float x = object_radius_mm * sen_angle_values[m_trig_index];
  float y = object_radius_mm * cos_angle_values[m_trig_index];
  float z = m_tower_turns * MM_PER_TOWER_RUN;

  m_points[m_points_size++] = (Point3f){.x = x, .y = y, .z = z};
}

void send_points() {
  for (uint8_t i = 0; i < m_points_size; ++i) {
    uart_send_point(&m_points[i]);
  }
}

float get_mm_point(int value) {
  return (float)value * m_line_slope + m_line_constant;
}
