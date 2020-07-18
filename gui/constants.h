
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
    int alienID;
    
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
    // 
    int blocked;
    // 
    int countAliens;
    int length;
    int strength;
    int queueSize;
    int full;
    int yield; // 1 SOUTH 0 NORTH
    int scheduler;
    double quatum;
    double crossTime;
    enum bridgePosition position;
    
    void *northList;
    void *southList;

    PATH *queueNorth;
    PATH *queueSouth;
    PATH *exitNorth;
    PATH *exitSouth;
    PATH *pass;
    
}
BRIDGE;


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


/**
 * ALIEN
 */ 
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


/**
 * GUI
 */ 

#define COMMUNITY_ALFA_POSX 125
#define COMMUNITY_ALFA_POSY 520

#define COMMUNITY_BETA_POSX 820
#define COMMUNITY_BETA_POSY 100

/**
 * CONFIGURATION
 */ 
#define WEST_BRIDGE_CONF_PATH  "../config/top_band.conf"
#define MID_BRIDGE_CONF_PATH  "../config/mid_band.conf"
#define EAST_BRIDGE_CONF_PATH  "../config/bottom_band.conf"
#define AUX_FILE_PATH  "../config/config.aux"


struct configBridge
{
    enum bridgePosition pos;
    int length;
    int strength;
    // scheduler option
    int scheduler;
};


#endif