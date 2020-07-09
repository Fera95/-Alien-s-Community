#include "gui.h"


//allegro variable
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *path = NULL;
// 
//

bool done = false;
bool render = false;	

struct PATH alfaexit[17];
struct PATH alfaentrance[14];
struct PATH northavenue[50];




void must_init(bool test, const char *description)
{
    if (test)
        return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}
/**
*/
int init_gui(GUI_CONTEXT *ctx)
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ctx->timer = al_create_timer(1.0 / 30.0);
    must_init(ctx->timer, "timer");

    ctx->queue = al_create_event_queue();
    must_init(ctx->queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    ctx->disp = al_create_display(975, 678);
    must_init(ctx->disp, "display");

    ctx->font = al_create_builtin_font();
    must_init(ctx->font, "font");

    set_background(ctx);

    must_init(al_init_primitives_addon(), "primitives");
    al_register_event_source(ctx->queue, al_get_keyboard_event_source());
    al_register_event_source(ctx->queue, al_get_display_event_source(ctx->disp));
    al_register_event_source(ctx->queue, al_get_timer_event_source(ctx->timer));
    ctx->done = false;
    ctx->redraw = true;
    printf("Init success\n");
    return 0;
}

int loop_gui(GUI_CONTEXT *ctx)
{
    /**
     */
    al_start_timer(ctx->timer);
    /**
      */


    printf("Before Thread\n"); 
    struct ALIEN alien1;
    alien1.ctx = ctx;
    alien1.image = al_load_bitmap("assets/alfa25.png");
    alien1.x = 35;
    alien1.y = 400;
    

    pthread_t thread_id; 
    pthread_create(&thread_id, NULL, moveAlien, (void*)&alien1);

  	timer = al_create_timer(1.0 / 60);
   	al_start_timer(timer);

            
    while (1)
    {
        /**
         */
        al_wait_for_event(ctx->queue, &(ctx->event));
        /**
         */
        switch (ctx->event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            /**
             * Logic here 
             */
            ctx->redraw = true;
            break;
        /**
         */
        case ALLEGRO_EVENT_KEY_DOWN:
        /**
         */
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            ctx->done = true;
            break;
        }
        /**
         */
        if (ctx->done)
        {
            printf("Done\n");
            break;
        }
        /**
         */
        if (ctx->redraw && al_is_event_queue_empty(ctx->queue))
        {

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(ctx->background, 0, 0, 1);
            drawmap(ctx);
            al_draw_bitmap(ctx->alfaCommunity, 5, 470, 0);
            al_draw_bitmap(ctx->betaCommunity, 770, 100, 0);
            al_draw_bitmap(alien1.image, alien1.x, alien1.y, 0);
            al_flip_display();
            ctx->redraw = false;
        }
    }
    finalize_gui(ctx);
}

int finalize_gui(GUI_CONTEXT *ctx)
{
    al_destroy_font(ctx->font);
    al_destroy_display(ctx->disp);
    al_destroy_timer(ctx->timer);
    al_destroy_event_queue(ctx->queue);
    al_destroy_bitmap(ctx->background);
    al_destroy_bitmap(ctx->path);
}

int set_background(GUI_CONTEXT *ctx)
{
    must_init(al_init_image_addon(), "image addon");
    ctx->background = al_load_bitmap("assets/background.png");
    ctx->path = al_load_bitmap("assets/path.png");
    ctx->bridge = al_load_bitmap("assets/bridge.png");
    ctx->alfaCommunity = al_load_bitmap("assets/alfaplanet.png");
    ctx->betaCommunity = al_load_bitmap("assets/betaplanet.png");
    must_init(ctx->background, "background");
    must_init(ctx->path, "path");
    return 0;
}
// 

void *moveAlien(void *args)
{
    sleep(2);
    struct ALIEN *myAlien = (ALIEN*) args;
    while (myAlien->y>10)
    {
        // al_wait_for_event(myAlien->ctx->queue, &(myAlien->ctx->event));
        usleep(100);
        myAlien->y-=0.01;
    }
}

int add_character(GUI_CONTEXT *ctx, int id, int type, int dx, int dy){
    
}

int * create_map(int lenA){
    map = malloc(3*sizeof(PATH*));
    map[0] = alfaexit;
    map[1] = alfaentrance;
    map[2] = northavenue;

}

void drawmap(GUI_CONTEXT *ctx)//, int lenA, int lenB, int lenC)
{
    // Alfa Community Entrance
    int posxInit = 70;
    int posyInit = 500;
    for(int i=0;i<17;i++){
        alfaentrance[i].x = posxInit;
        alfaentrance[i].y = posyInit-30*i;
        alfaentrance[i].height = 30;
        alfaentrance[i].width = 40;
        alfaentrance[i].image = ctx->path;
        alfaentrance[i].blocked = 0;
        al_draw_bitmap(alfaentrance[i].image, alfaentrance[i].x, alfaentrance[i].y, 0);
    }

    // Alfa Community Exit
    posxInit = 150;
    posyInit = 490;
    for(int i=0;i<14;i++){
        alfaexit[i].x = posxInit;
        alfaexit[i].y = posyInit-30*i;
        alfaexit[i].height = 30;
        alfaexit[i].width = 40;
        alfaexit[i].image = ctx->path;
        alfaexit[i].blocked = 0;
        al_draw_bitmap(alfaexit[i].image, alfaexit[i].x, alfaexit[i].y, 0);
    }
    posxInit = 140;
    posyInit = 20;
    int k = 0;
    // North Avenue
    for(int i=0;i<50;i++){
        if(i==26){
            posxInit = 180;
            posyInit = 60;
            k=0;
        }
        northavenue[i].x = posxInit+30*k;
        northavenue[i].y = posyInit;
        northavenue[i].height = 40;
        northavenue[i].width = 30;
        northavenue[i].image = ctx->path;
        northavenue[i].blocked = 0;
        // al_draw_bitmap(northavenue[i].image, , northavenue[i].y, 0);
        al_draw_rotated_bitmap(northavenue[i].image,0,0,northavenue[i].x,northavenue[i].y, ALLEGRO_PI/2, 0);
        k++;
    }
}


    // for (int i=0; i<22; i++ ){
    //     al_draw_bitmap(ctx->paths,120 ,i*30, 0);
    //     al_draw_bitmap(ctx->paths,40 ,i*30, 0);
    //     }
    // for (int i=1; i<33; i++ ){
    //     al_draw_rotated_bitmap(ctx->paths,0,0,i*30,0, ALLEGRO_PI/2, 0);
    //     al_draw_rotated_bitmap(ctx->paths,0,0,i*30,30, ALLEGRO_PI/2, 0);
        // }