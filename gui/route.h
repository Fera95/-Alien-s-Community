#ifndef ROUTE_HEADER_FILE
#define ROUTE_HEADER_FILE
#include "constants.h"

ROUTE * create_route ( BRIDGE* bridge, PATH **map, enum origin start );
void next_move ( float *next_x,float* next_y, ROUTE * alienRoute, float dx,float dy );
#endif