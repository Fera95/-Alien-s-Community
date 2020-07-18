#ifndef CEGUI_HEADER_FILE
#define CEGUI_HEADER_FILE
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#include "constants.h"
#include "bridge.h"
#include "route.h"
#include "alien.h"

/**
 * Basic Structure to render a gui
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
    //   Abstraction of General/GUI envent
    ALLEGRO_EVENT event;

    // Background representation
    ALLEGRO_BITMAP *background;
    ALLEGRO_BITMAP *pathImage;
    ALLEGRO_BITMAP *queueImage;
    ALLEGRO_BITMAP *passImage;
    ALLEGRO_BITMAP *alfaCommunity;
    ALLEGRO_BITMAP *betaCommunity;
    ALLEGRO_BITMAP *betaImage;
    ALLEGRO_BITMAP *alfaImage;
    ALLEGRO_BITMAP *normalImage;

    PATH ** map;
    BRIDGE * eastBridge;
    BRIDGE * midBridge;
    BRIDGE * westBridge;
    // Controls when to repaint the window
    bool redraw;
    // Controls when to finisthe game
    bool done;

    // This var is used to handle pressed click
    int mouse_pressed;// 1 true 0 false
    // This var uis used to handle released click
    int mouse_released;// 1 true 0 false
    // handle x mouse coordinate
    int x;
    // handle y mouse coordinate
    int y;
} GUI_CONTEXT;


int init_gui(GUI_CONTEXT *ctx);
int loop_gui(GUI_CONTEXT *ctx);
int finalize_gui(GUI_CONTEXT *ctx);
int set_background(GUI_CONTEXT *ctx);
int add_character(GUI_CONTEXT *ctx, int id, int type, int dx, int dy);
void *moveAlien(void *args);
void drawmap(PATH **map, GUI_CONTEXT *ctx);
void drawBridge(BRIDGE* bridge, GUI_CONTEXT *ctx);
void create_map(GUI_CONTEXT *ctx);
ALIEN * generateAlien (GUI_CONTEXT *ctx);
#endif
