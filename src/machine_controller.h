
#include <avr/io.h>

#include "Point3f.h"

#define MAX_POINTS 40
#define MAX_TOWER_STEPS 12000
#define MM_PER_TOWER_RUN 1.25 / 2.0
#define STEP_SIZE_MM 2.0 / 200.0
#define BASE_STEPS_PER_RUN 5
#define TOWER_STEPS_PER_RUN 100
#define CENTER_DISTANCE 327

#define CALIBRATION_STEP 400

uint8_t machine_run();
uint8_t machine_reset();
void machine_calibration();
void machine_motors_steps();
float get_mm_point(uint16_t value);

void get_point();
void send_points();
