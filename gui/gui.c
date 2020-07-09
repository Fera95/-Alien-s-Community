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
#define EXIT_COUNT 11
#define ENTRY_COUNT 11
#define AVENUE_COUNT 16
#define MAPSIZE 8

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

    create_map(ctx);

    ctx->done = false;
    ctx->redraw = true;
    printf("Init success\n");
    return 0;
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

void create_map(GUI_CONTEXT *ctx) //, int lenA, int lenB, int lenC)
{
    PATH *alfaexit = malloc(EXIT_COUNT * sizeof(PATH));
    PATH *alfaentry = malloc(ENTRY_COUNT * sizeof(PATH));
    PATH *northavenueA = malloc(AVENUE_COUNT * sizeof(PATH));
    PATH *northavenueB = malloc(AVENUE_COUNT * sizeof(PATH));

    PATH *betaexit = malloc(EXIT_COUNT * sizeof(PATH));
    PATH *betaentry = malloc(ENTRY_COUNT * sizeof(PATH));
    PATH *southavenueA = malloc(AVENUE_COUNT * sizeof(PATH));
    PATH *southavenueB = malloc(AVENUE_COUNT * sizeof(PATH));

    // Alfa Community enter
    int posxInit = 70;
    int posyInit = 500;
    for (int i = 0; i < ENTRY_COUNT; i++)
    {
        alfaentry[i].x = posxInit;
        alfaentry[i].y = posyInit - 40 * i;
        alfaentry[i].height = 40;
        alfaentry[i].width = 40;
        alfaentry[i].image = ctx->path;
        alfaentry[i].blocked = 0;
    }

    // Alfa Community Exit
    posxInit = 150;
    posyInit = 500;
    for (int i = 0; i < EXIT_COUNT; i++)
    {
        alfaexit[i].x = posxInit;
        alfaexit[i].y = posyInit - 40 * i;
        alfaexit[i].height = 40;
        alfaexit[i].width = 40;
        alfaexit[i].image = ctx->path;
        alfaexit[i].blocked = 0;
    }
    posxInit = 70;
    posyInit = 20;
    int k = 0;
    // North Avenue
    for (int i = 0; i < AVENUE_COUNT; i++)
    {
        northavenueB[i].x = posxInit + 40 * k;
        northavenueB[i].y = posyInit;
        northavenueB[i].height = 40;
        northavenueB[i].width = 40;
        northavenueB[i].image = ctx->path;
        northavenueB[i].blocked = 0;
        k++;
    }
    posxInit = 70;
    posyInit += 40;
    k = 0;
    for (int i = 0; i < AVENUE_COUNT; i++)
    {
        northavenueA[i].x = posxInit + 40 * k;
        northavenueA[i].y = posyInit;
        northavenueA[i].height = 40;
        northavenueA[i].width = 40;
        northavenueA[i].image = ctx->path;
        northavenueA[i].blocked = 0;
        k++;
    }
    // #########################################
    // Beta Community Exit
    posxInit = 810;
    posyInit = 120;
    for (int i = 0; i < EXIT_COUNT; i++)
    {
        betaexit[i].x = posxInit;
        betaexit[i].y = posyInit + 40 * i;
        betaexit[i].height = 40;
        betaexit[i].width = 40;
        betaexit[i].image = ctx->path;
        betaexit[i].blocked = 0;
    }

    // Beta Community enter
    posxInit = 890;
    posyInit = 80 + EXIT_COUNT * 40;
    for (int i = 0; i < ENTRY_COUNT; i++)
    {
        betaentry[i].x = posxInit;
        betaentry[i].y = posyInit - 40 * i;
        betaentry[i].height = 40;
        betaentry[i].width = 40;
        betaentry[i].image = ctx->path;
        betaentry[i].blocked = 0;
    }

    posxInit = 890;
    posyInit = 560;
    k = 0;
    // South Avenue
    for (int i = 0; i < AVENUE_COUNT; i++)
    {
        southavenueB[i].x = posxInit - 40 * k;
        southavenueB[i].y = posyInit;
        southavenueB[i].height = 40;
        southavenueB[i].width = 40;
        southavenueB[i].image = ctx->path;
        southavenueB[i].blocked = 0;
        k++;
    }
    posxInit = 930 - AVENUE_COUNT * 40;
    posyInit += 40;
    k = 0;
    for (int i = 0; i < AVENUE_COUNT; i++)
    {
        southavenueA[i].x = posxInit + 40 * k;
        southavenueA[i].y = posyInit;
        southavenueA[i].height = 40;
        southavenueA[i].width = 40;
        southavenueA[i].image = ctx->path;
        southavenueA[i].blocked = 0;
        k++;
    }
    // ##################################
    // map construccion
    ctx->map = (PATH **)malloc(MAPSIZE * sizeof(PATH *));
    ctx->map[0] = alfaexit;
    ctx->map[1] = alfaentry;
    ctx->map[2] = northavenueA;
    ctx->map[3] = northavenueB;
    ctx->map[4] = southavenueA;
    ctx->map[5] = southavenueB;
    ctx->map[6] = betaexit;
    ctx->map[7] = betaentry;
}

int loop_gui(GUI_CONTEXT *ctx)
{
    /**
     */
    al_start_timer(ctx->timer);

    timer = al_create_timer(1.0 / 60);
    al_start_timer(timer);

    printf("Before Thread\n");
    struct ALIEN alien1;
    alien1.ctx = ctx;
    alien1.image = al_load_bitmap("assets/alfa25.png");
    alien1.x = 35;
    alien1.y = 400;
    alien1.type = alfa;

    struct ALIEN alienBeta;
    alienBeta.ctx = ctx;
    alienBeta.image = al_load_bitmap("assets/beta25.png");
    alienBeta.x = 600;
    alienBeta.y = 600;
    alienBeta.type = beta;

    //Generate aliens arrays:  // TODO this will be removed soon
    pthread_t thread_id_beta;
    pthread_create(&thread_id_beta, NULL, moveAlien, (void *)&alienBeta);

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, moveAlien, (void *)&alien1);

    // pthread_join(&thread_id, NULL);
    // pthread_join(&thread_id_beta, NULL);

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
            drawmap(ctx->map);
            al_draw_bitmap(ctx->alfaCommunity, 5, 470, 0);
            al_draw_bitmap(ctx->betaCommunity, 775, 20, 0);
            al_draw_bitmap(alien1.image, alien1.x, alien1.y, 0);
            al_draw_bitmap(alienBeta.image, alienBeta.x, alienBeta.y, 0);
            al_flip_display();
            ctx->redraw = false;
        }
    }
    finalize_gui(ctx);
}

void drawmap(PATH **map)
{
    PATH *alfaexit = map[0];
    PATH *alfaentry = map[1];
    PATH *northavenueA = map[2];
    PATH *northavenueB = map[3];
    PATH *southavenueA = map[4];
    PATH *southavenueB = map[5];
    PATH *betaexit = map[6];
    PATH *betaentry = map[7];

    for (int i = 0; i < ENTRY_COUNT; i++)
    {
        al_draw_bitmap(alfaentry[i].image, alfaentry[i].x, alfaentry[i].y, 0);
        al_draw_bitmap(betaentry[i].image, betaentry[i].x, betaentry[i].y, 0);
    }

    for (int i = 0; i < EXIT_COUNT; i++)
    {
        al_draw_bitmap(alfaexit[i].image, alfaexit[i].x, alfaexit[i].y, 0);
        al_draw_bitmap(betaexit[i].image, betaexit[i].x, betaexit[i].y, 0);
    }

    for (int i = 0; i < AVENUE_COUNT; i++)
    {
        al_draw_bitmap(northavenueA[i].image, northavenueA[i].x, northavenueA[i].y, 0);
        al_draw_bitmap(southavenueA[i].image, southavenueA[i].x, southavenueA[i].y, 0);
        al_draw_bitmap(northavenueB[i].image, northavenueB[i].x, northavenueB[i].y, 0);
        al_draw_bitmap(southavenueB[i].image, southavenueB[i].x, southavenueB[i].y, 0);
    }
}

void *moveAlien(void *args)
{

    /*
    ctx->map[0] = alfaexit;
    ctx->map[1] = alfaentry;
    ctx->map[2] = northavenueA;
    ctx->map[3] = northavenueB;
    ctx->map[4] = southavenueA;
    ctx->map[5] = southavenueB;
    ctx->map[6] = betaexit;
    ctx->map[7] = betaentry;
    */
    ALIEN *myAlien = (ALIEN *)args;
    PATH **road = malloc(4 * sizeof(PATH *));
    road[0] = myAlien->ctx->map[0];
    road[1] = myAlien->ctx->map[2];
    road[2] = myAlien->ctx->map[4];
    road[3] = myAlien->ctx->map[7];

    int init = 0;
    int LIMIT = 0;
    for (int i = 0; i < 4; i++)
    {
        if (i == 0 || i == 3 || i == 2)
        {
            init = 0;
            LIMIT = EXIT_COUNT;
        }
        if (i == 1)
        {
            init = 2;
            LIMIT = AVENUE_COUNT;
        }

        for (init; init < LIMIT; init++)
        {
            myAlien->x = road[i][init].x;
            myAlien->y = road[i][init].y;
            sleep(1);
        }
    }
    free(road);
}
