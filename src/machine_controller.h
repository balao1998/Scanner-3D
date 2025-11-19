
#include <avr/io.h>

#include "Point3f.h"

#define MAX_POINTS 20
#define MAX_TOWER_STEPS 12000
#define MM_PER_TOWER_TURN 1.25
#define STEP_SIZE_MM 2.0 / 200.0
#define BASE_STEPS_PER_RUN 5
#define TOWER_STEPS_PER_RUN 200
#define CENTER_DISTANCE 327

uint8_t machine_run();
uint8_t machine_reset();
void machine_motors_steps();

void get_point();
void send_points();
