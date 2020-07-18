#ifndef ROUTE_HEADER_FILE
#define ROUTE_HEADER_FILE
#include "constants.h"

ROUTE * create_route ( BRIDGE** bridge, PATH **map, enum origin start );
void next_move (ALIEN *alien );
int can_move( ALIEN *alienMoving, PATH *nextPATH, int pos);
void draw_sorted_queue(NODE_ALIEN *sorted_list, PATH *queuePATH, int sizequeue);


#endif