#ifndef BRIDGE_HEADER_FILE
#define BRIDGE_HEADER_FILE

#include "path.h"

// #include <allegro5/allegro5.h>
// #include <allegro5/allegro_font.h>
// #include <allegro5/allegro_image.h>
// #include <allegro5/allegro_primitives.h>

// CONSTANTS
#define TOTAL_LENGTH 12
#define EAST_POSX 270
#define MID_POSX 430
#define WEST_POSX 630
#define INIT_POSY 100
#define END_POSY 540  //INIT_POSY - 40*TOTAL_LENGTH
#define MAX_LENGTH 10

 enum bridgePosition{east, mid, west} ;
typedef struct BRIDGE
{
    // Bloqueado
    int blocked;
    // 
    int length;
    int weight;
    int queueSize;
    enum bridgePosition position;
    PATH *queueNorth;
    PATH *queueSouth;
    PATH *exitNorth;
    PATH *exitSouth;
    PATH *pass;
    
    // CONTEXT
    // GUI_CONTEXT *ctx;
    
}
BRIDGE;

BRIDGE * create_bridge( int length, int weight, enum bridgePosition position, ALLEGRO_BITMAP *queueImage, ALLEGRO_BITMAP *passImage, ALLEGRO_BITMAP *pathImage);


#endif

