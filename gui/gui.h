#ifndef CEGUI_HEADER_FILE
#define CEGUI_HEADER_FILE
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

/**
 * 
 * */ 
#include "path.h"
#include "bridge.h"
#include "route.h"

#define COMMUNITY_ALFA_POSX 125
#define COMMUNITY_ALFA_POSY 520

#define COMMUNITY_BETA_POSX 820
#define COMMUNITY_BETA_POSY 100

/**
 * This is the basic structure to render a gui
 */
typedef struct GUI_CONTEXT
{
    /**
     * We need a timer and an event queue to 
     * ensure the game runs at a consistent speed; 
     */ 
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *queue;
    /**
     * Pointer to the Alegro display
     */ 
    ALLEGRO_DISPLAY *disp;
    //  Allegro font pointer 
    ALLEGRO_FONT *font;
    /**
     * Controls when to finisthe game
     */ 
    bool done;
    /**
     *  Controls when to repaint the window
     */
    bool redraw;
    /**
     * Abstraction of General/GUI envent
     */ 
    ALLEGRO_EVENT event;

    /**
     * Background representation
     */
    ALLEGRO_BITMAP *background;
    ALLEGRO_BITMAP *pathImage;
    ALLEGRO_BITMAP *queueImage;
    ALLEGRO_BITMAP *passImage;
    ALLEGRO_BITMAP *alfaCommunity;
    ALLEGRO_BITMAP *betaCommunity;

    PATH ** map;
    BRIDGE * eastBridge;
    BRIDGE * midBridge;
    BRIDGE * westBridge;

} GUI_CONTEXT;
/**
 * Movement Bouncer
 */ 
  
typedef struct ALIEN
{
    // IDENTIFER
    int id;
    // COORDENADAS
    float x, y;
    // SPEED
    float dx, dy;
    // TYPE
    enum alienType type;
    // highway
    ROUTE * way;
    // IMAGE
    ALLEGRO_BITMAP * image;
    // CONTEXT
    GUI_CONTEXT *ctx;

    
} ALIEN;

int init_gui(GUI_CONTEXT *ctx);
int loop_gui(GUI_CONTEXT *ctx);
int finalize_gui(GUI_CONTEXT *ctx);
int set_background(GUI_CONTEXT *ctx);
int add_character(GUI_CONTEXT *ctx, int id, int type, int dx, int dy);
void *moveAlien(void *args);
void drawmap(PATH **map);
void drawBridge(BRIDGE* bridge);
void create_map(GUI_CONTEXT *ctx);
#endif
