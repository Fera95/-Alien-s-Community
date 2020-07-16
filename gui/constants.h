
#ifndef CONSTANTS_HEADER_FILE
#define CONSTANTS_HEADER_FILE

#include <stdlib.h>


// PATH
#define EXIT_COUNT 11
#define ENTRY_COUNT 11
#define AVENUE_COUNT 16

typedef struct PATH
{
    // Bloqueado
    int blocked;
    // COORDENADAS
    float x, y;
    int width, height;
    
} PATH;

// BRIDGE
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


// ROUTE

enum origin { alfaPlanet, betaPlanet };
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
    enum origin start;
    enum direction dirx;
    enum direction diry;
    int pos;
    int limit;
    int finished;


}ROUTE;





// ALIEN
enum alienType { alfa, beta, normal };
enum priority { high, medium, low };


typedef struct ALIEN
{
    // IDENTIFER
    int id;
    // COORDENADAS
    float x, y;
    // SPEED
    float dx, dy;
    // THREAD
    // pthread_t idThread;
    // TYPE
    enum alienType type;
    // Priority
    enum priority alienPriority;
    // highway
    ROUTE * way;
    
} ALIEN;

typedef struct NODE_ALIEN
{
    struct ALIEN * data;
    struct NODE_ALIEN * next;

}NODE_ALIEN;


typedef struct INVADER
{
    // IDENTIFER
    int id;
    // COORDENADAS
    float x, y;
    // SPEED
    float dx, dy;
    // THREAD
    // highway
    ROUTE * way;
    // IMAGE
    
} INVADER;


// GUI

#define COMMUNITY_ALFA_POSX 125
#define COMMUNITY_ALFA_POSY 520

#define COMMUNITY_BETA_POSX 820
#define COMMUNITY_BETA_POSY 100



#endif