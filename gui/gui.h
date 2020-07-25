#ifndef CEGUI_HEADER_FILE
#define CEGUI_HEADER_FILE
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "LPTHREAD/lpthread.h"
#include "constants.h"
#include "bridge.h"
#include "route.h"
#include "alien.h"
#include "path.h"


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
    // Menu Images
    ALLEGRO_BITMAP *alfaSelected;
    ALLEGRO_BITMAP *betaSelected;
    ALLEGRO_BITMAP *normalSelected;
    ALLEGRO_BITMAP *a;
    ALLEGRO_BITMAP *ap;
    ALLEGRO_BITMAP *b;
    ALLEGRO_BITMAP *bp;
    ALLEGRO_BITMAP *c;
    ALLEGRO_BITMAP *cp;
    ALLEGRO_BITMAP *i;
    ALLEGRO_BITMAP *ip;
    ALLEGRO_BITMAP *sideAlfa;
    ALLEGRO_BITMAP *sideBeta;
    ALLEGRO_BITMAP *open;
    ALLEGRO_BITMAP *close;


    PATH **map;
    BRIDGE *eastBridge;
    BRIDGE *midBridge;
    BRIDGE *westBridge;
    
    bool redraw;                // Controls when to repaint the window
    bool done;                  // Controls when to finisthe game
    bool run;                   // PAUSE FLAG
    // This var is used to handle pressed click
    int mouse_pressed;          // 1 true 0 false
    // This var uis used to handle released click
    int mouse_released;         // 1 true 0 false
    int x;                      // handle x mouse coordinate
    int y;                      // handle y mouse coordinate
    int generate;               // Automatic Alien Generation
    int waitTime;               // Wait for a period of time
    alien_config config;        // Config from file
    enum origin sideSelected;   // side selected 0 alfa, 1 is beta   
    ALIEN *alienSelected;       // selected alien
    NODE_ALIEN *head;           //head the first node
} GUI_CONTEXT;

int init_gui(GUI_CONTEXT *ctx);
int loop_gui(GUI_CONTEXT *ctx);
int finalize_gui(GUI_CONTEXT *ctx);
int set_background(GUI_CONTEXT *ctx);
int add_character(GUI_CONTEXT *ctx, int id, int type, int dx, int dy);
void *moveAlien(void *args);
void drawmap(PATH **map, GUI_CONTEXT *ctx);
void drawBridge(BRIDGE *bridge, GUI_CONTEXT *ctx);
void create_map(GUI_CONTEXT *ctx);
void handleMenu(GUI_CONTEXT *ctx);
void drawMenu(GUI_CONTEXT *ctx);
void clickedAlien(GUI_CONTEXT *ctx, NODE_ALIEN *head);
void applySemaphoreState(GUI_CONTEXT *ctx);
ALIEN *generateAlien(GUI_CONTEXT *ctx);

#endif
