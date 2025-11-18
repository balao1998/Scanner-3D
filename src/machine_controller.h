
#include <avr/io.h>

#include "Point3f.h"

#define MAX_POINTS 20

void machine_run();
uint8_t machine_reset();

void get_point();
void send_points();
