#ifndef ROUTE_HEADER_FILE
#define ROUTE_HEADER_FILE

#include "bridge.h"
#include "path.h"



enum alienType{alfa,beta};
enum direction{left, right, up, down};
typedef struct ROUTE 
{
    // 
    BRIDGE * bridge;
    PATH * exit;
    PATH * avenue1;
    PATH * cross2avenue;
    PATH * avenue2;
    PATH * cross2entry;
    PATH * entry;
    PATH * current;
    enum alienType type;
    enum direction dirx;
    enum direction diry;
    int pos;
    int limit;
    int finished;


}ROUTE;


ROUTE * create_route(BRIDGE* bridge, PATH **map, enum alienType type, float dx, float dy  );
void next_move(float *next_x,float* next_y, ROUTE * alienRoute, float dx,float dy);
#endif