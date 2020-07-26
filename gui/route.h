#ifndef ROUTE_HEADER_FILE
#define ROUTE_HEADER_FILE
#include "constants.h"
#include "scheduler.h"

ROUTE * create_route ( BRIDGE** bridge, PATH **map, enum origin start );
int can_move( ALIEN *alienMoving, PATH *nextPATH, int pos);
void next_move (ALIEN *alien );
void set_sorted_path(NODE_ALIEN *sorted_list, PATH *queuePATH, int sizequeue);


#endif