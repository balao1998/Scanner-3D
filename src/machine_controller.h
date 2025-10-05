
#include <avr/io.h>

#include "Point3f.h"

#define MAX_POINTS 20


static Point3f m_points[MAX_POINTS];
static uint8_t m_points_size = 0;

static uint32_t m_tower_steps = 0;
static uint32_t m_base_steps = 0;

void machine_run();
void machine_reset();

void get_point();
void send_points();



