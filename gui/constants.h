
#ifndef CONSTANTS_HEADER_FILE
#define CONSTANTS_HEADER_FILE

#include <stdlib.h>
#include <string.h>

// PATH
#define EXIT_COUNT 11
#define ENTRY_COUNT 11
#define AVENUE_COUNT 16


/**
 * GUI
 */ 
#define COMMUNITY_ALFA_POSX 125
#define COMMUNITY_ALFA_POSY 520

#define COMMUNITY_BETA_POSX 820
#define COMMUNITY_BETA_POSY 100

// BRIDGE
#define TOTAL_LENGTH 12
#define EAST_POSX 270
#define MID_POSX 430
#define WEST_POSX 630
#define INIT_POSY 100
#define END_POSY 540  //INIT_POSY - 40*TOTAL_LENGTH
#define MAX_LENGTH 10

enum bridgePosition{east, mid, west} ;


typedef struct PATH
{
    // Bloqueado
    int blocked;
    // COORDENADAS
    float x, y;
    int width, height;
    int alienID;
    
} PATH;


typedef struct BRIDGE
{
    int length;
    int strength;
    int queueSize;
    int scheduler;
    int planner;
    int planner_north_count;
    int planner_south_count;
    int planner_time;
    // CONFIG
    enum bridgePosition position;
    double crossTime;
    int countAliens;
    int blocked;
    int full;
    int yield; // 1 SOUTH 0 NORTH
    // double quatum;
    
    void *northHead;
    void *southHead;

    PATH *queueNorth;
    PATH *queueSouth;
    PATH *exitNorth;
    PATH *exitSouth;
    PATH *pass;
    
}
BRIDGE;

typedef struct 
{
    int length;
    int strength;
    int queueSize;
    int scheduler;
    int planner;
    int planner_time;
    int planner_north_count;
    int planner_south_count;
} config_bridge;

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

typedef struct alien_config
{
    int base_speed;
    int rto_time;
    int medium_time_creation;
    int normal_percent;
    int alfa_percent;
    int beta_percent;

    
}alien_config;

typedef struct ALIEN
{
    int selected;           // FLAG TO SELECT ALIEN
    int deleted;            // FLAG TO FREE MEMORY
    int id;                 // IDENTIFER
    int tickets;            // LOTTERY ALGORITHM
    int alienPriority;      // PRIORITY ALGORITHM
    int weight;             // ALIEN WEIGHT TO PASS THE BRIDGE
    float x, y;             // COORDS IN SPACE
    float dx, dy;           // SPEED
    enum alienType type;    // TYPE OF ALIEN
    ROUTE * way;            // ROUTE THE ALIEN IS GOING TO TAKE
    
} ALIEN;

typedef struct NODE_ALIEN
{
    struct ALIEN * data;
    struct NODE_ALIEN * next;

}NODE_ALIEN;


typedef struct INVADER
{
    int id;         // IDENTIFER
    float x, y;     // COORDENADAS
    float dx, dy;   // SPEED
    ROUTE * way;    // HIGHWAY    
} INVADER;


#endif