#include "gui.h"

//allegro variable
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *pathImage = NULL;
ALLEGRO_BITMAP *queueImage = NULL;
ALLEGRO_BITMAP *passImage = NULL;
//

bool done = false;
bool render = false;

#define MAPSIZE 8
int countIDs = 0;
// CAMBIAR X ARCHIVO DE CONFIGURACION
float SPEED_NORMAL = 2;

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
    ctx->disp = al_create_display(1366, 678);
    must_init(ctx->disp, "display");

    ctx->font = al_create_builtin_font();
    must_init(ctx->font, "font");

    set_background(ctx);

    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_install_mouse(), "mouse");
    al_register_event_source(ctx->queue, al_get_keyboard_event_source());
    al_register_event_source(ctx->queue, al_get_display_event_source(ctx->disp));
    al_register_event_source(ctx->queue, al_get_timer_event_source(ctx->timer));
    al_register_event_source(ctx->queue, al_get_mouse_event_source());
    ctx->done = false;
    ctx->redraw = true;
    ctx->mouse_released = 0;
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
    al_destroy_bitmap(ctx->pathImage);
}

int set_background(GUI_CONTEXT *ctx)
{
    must_init(al_init_image_addon(), "image addon");
    ctx->background = al_load_bitmap("assets/background.png");
    ctx->pathImage = al_load_bitmap("assets/path.png");
    ctx->passImage = al_load_bitmap("assets/bridge2.png");
    ctx->queueImage = al_load_bitmap("assets/queue.png");
    ctx->alfaCommunity = al_load_bitmap("assets/alfaplanet.png");
    ctx->betaCommunity = al_load_bitmap("assets/betaplanet.png");
    ctx->betaImage = al_load_bitmap("assets/beta25.png");
    ctx->alfaImage = al_load_bitmap("assets/alfa25.png");
    ctx->normalImage = al_load_bitmap("assets/normal25.png");

    ctx->alfaSelected = al_load_bitmap("assets/alfaSelected.png");
    ctx->betaSelected = al_load_bitmap("assets/betaSelected.png");
    ctx->normalSelected = al_load_bitmap("assets/normalSelected.png");

    ctx->a = al_load_bitmap("assets/a.png");
    ctx->ap = al_load_bitmap("assets/ap.png");
    ctx->b = al_load_bitmap("assets/b.png");
    ctx->bp = al_load_bitmap("assets/bp.png");
    ctx->c = al_load_bitmap("assets/c.png");
    ctx->cp = al_load_bitmap("assets/cp.png");

    ctx->sideAlfa = al_load_bitmap("assets/sideAlfa.png");
    ctx->sideBeta = al_load_bitmap("assets/sideBeta.png");

    must_init(ctx->background, "background");
    must_init(ctx->pathImage, "path");
    return 0;
}

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
    int posyInit = 100;
    for (int i = 0; i < ENTRY_COUNT; i++)
    {
        alfaentry[i].x = posxInit;
        alfaentry[i].y = posyInit + 40 * i;
        alfaentry[i].height = 40;
        alfaentry[i].width = 40;
        // alfaentry[i].image = ctx->pathImage;
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
        // alfaexit[i].image = ctx->pathImage;
        alfaexit[i].blocked = 0;
    }
    posxInit = 670;
    posyInit = 20;
    int k = 0;
    // North Avenue
    for (int i = 0; i < AVENUE_COUNT; i++)
    {
        northavenueB[i].x = posxInit - 40 * k;
        northavenueB[i].y = posyInit;
        northavenueB[i].height = 40;
        northavenueB[i].width = 40;
        // northavenueB[i].image = ctx->pathImage;
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
        // northavenueA[i].image = ctx->pathImage;
        northavenueA[i].blocked = 0;
        k++;
    }
    // #########################################
    // Beta Community Exit
    posxInit = 790;
    posyInit = 140;
    for (int i = 0; i < EXIT_COUNT; i++)
    {
        betaexit[i].x = posxInit;
        betaexit[i].y = posyInit + 40 * i;
        betaexit[i].height = 40;
        betaexit[i].width = 40;
        // betaexit[i].image = ctx->pathImage;
        betaexit[i].blocked = 0;
    }

    // Beta Community enter
    posxInit = 870;
    posyInit = 100 + EXIT_COUNT * 40;
    for (int i = 0; i < ENTRY_COUNT; i++)
    {
        betaentry[i].x = posxInit;
        betaentry[i].y = posyInit - 40 * i;
        betaentry[i].height = 40;
        betaentry[i].width = 40;
        // betaentry[i].image = ctx->pathImage;
        betaentry[i].blocked = 0;
    }

    posxInit = 870;
    posyInit = 580;
    k = 0;
    // South Avenue
    for (int i = 0; i < AVENUE_COUNT; i++)
    {
        southavenueB[i].x = posxInit - 40 * k;
        southavenueB[i].y = posyInit;
        southavenueB[i].height = 40;
        southavenueB[i].width = 40;
        // southavenueB[i].image = ctx->pathImage;
        southavenueB[i].blocked = 0;
        k++;
    }
    posxInit = 910 - AVENUE_COUNT * 40;
    posyInit += 40;
    k = 0;
    for (int i = 0; i < AVENUE_COUNT; i++)
    {
        southavenueA[i].x = posxInit + 40 * k;
        southavenueA[i].y = posyInit;
        southavenueA[i].height = 40;
        southavenueA[i].width = 40;
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

/**
 * CREATE A RANDOM ALIEN 
 */
ALIEN *generateAlien(GUI_CONTEXT *ctx)
{
    /**
     *  LLAMAR A FUNCIONES DE ALEATORIEDAD EXPONENCIAL ETC...
     */
    enum alienType type = (enum alienType)(rand() % 3); // beta;//
    enum origin start = (enum origin)(rand() % 2);      //alfaPlanet;//betaPlanet;//
    // RANDOM COMMUNITY
    int emptyqueue = 1;
    if (start == alfaPlanet && (ctx->map[0][2]).blocked)
    {
        emptyqueue = 0;
        printf("SALIDA ALFA NO DISPONIBLE\n");
    }
    else if (start == betaPlanet && (ctx->map[6][2]).blocked)
    {
        emptyqueue = 0;
        printf("SALIDA BETA NO DISPONIBLE\n");
    }

    //  RANDOM BRIDGE
    BRIDGE *tempBridge;
    enum bridgePosition tempPos = (enum bridgePosition)(rand() % 3);
    if (tempPos == east)
    {
        tempBridge = ctx->eastBridge;
        // if(start == alfaPlanet){
        //     if(ctx->eastBridge->queueNorth[0].blocked  && ctx->eastBridge->queueNorth[ctx->eastBridge->queueSize-1].blocked ){
        //         emptyqueue = 0;
        //     }
        // }
        // else if(start == betaPlanet){
        //     if(ctx->eastBridge->queueSouth[0].blocked  && ctx->eastBridge->queueSouth[ctx->eastBridge->queueSize-1].blocked ){
        //         emptyqueue = 0;
        //     }
        // }
    }
    else if (tempPos == west)
    {
        tempBridge = ctx->westBridge;
        // if(start == alfaPlanet){
        //     if(ctx->westBridge->queueNorth[0].blocked  && ctx->westBridge->queueNorth[ctx->westBridge->queueSize-1].blocked ){
        //         emptyqueue = 0;
        //     }
        // }
        // else if(start == betaPlanet){
        //     if(ctx->westBridge->queueSouth[0].blocked  && ctx->westBridge->queueSouth[ctx->westBridge->queueSize-1].blocked ){
        //         emptyqueue = 0;
        //     }
        // }
    }
    else if (tempPos == mid)
    {
        tempBridge = ctx->midBridge;
        // if(start == alfaPlanet){
        //     if(ctx->midBridge->queueNorth[0].blocked  && ctx->midBridge->queueNorth[ctx->midBridge->queueSize-1].blocked ){
        //         emptyqueue = 0;
        //     }
        // }
        // else if(start == betaPlanet){
        //     if(ctx->midBridge->queueSouth[0].blocked  && ctx->midBridge->queueSouth[ctx->midBridge->queueSize-1].blocked ){
        //         emptyqueue = 0;
        //     }
        // }
    }
    ALIEN *newAlien;
    if (emptyqueue)
    {
        float initPosX, initPosY;
        ROUTE *tempRoute = create_route(&tempBridge, ctx->map, start);
        if (start == alfaPlanet)
        {
            // printf("ORIGING ALFA PLANET\n");
            initPosX = COMMUNITY_ALFA_POSX;
            initPosY = COMMUNITY_ALFA_POSY;
        }
        else if (start == betaPlanet)
        {
            // printf("ORIGING BETA PLANET\n");
            initPosX = COMMUNITY_BETA_POSX;
            initPosY = COMMUNITY_BETA_POSY;
        }
        newAlien = create_alien(countIDs, type, &tempRoute, initPosX, initPosY, SPEED_NORMAL);
        countIDs++;
    }

    else
    {
        newAlien = NULL;
    }
    return newAlien;
}

// DIBUJAR Y MOVER

int loop_gui(GUI_CONTEXT *ctx)
{
    /**
     */
    al_start_timer(ctx->timer);

    timer = al_create_timer(1.0 / 60);
    al_start_timer(timer);

    // printf("BEFORE CREATING THE LIST\n");

    create_map(ctx);

    create_bridge(&ctx->eastBridge, 3, 8, east, 0);
    // ctx->eastBridge->queueNorth[4].blocked = 1;
    // for (int i = 0; i < 5; i++)
    // {
    //     printf("QUEUE[%d] bloqueado:%d\n",i, ctx->eastBridge->queueNorth[i].blocked);
    // }
    create_bridge(&ctx->midBridge, 5, 5, mid, 0);
    create_bridge(&ctx->westBridge, 9, 1, west, 0);

    int count = 20, flag = 0;
    // CABEZA DE LA LISTA
    NODE_ALIEN *tempNode;
    ctx->head = (NODE_ALIEN *)malloc(sizeof(NODE_ALIEN *));
    ctx->head->data = generateAlien(ctx);
    ctx->head->next = NULL;
    ctx->head->data->id = -7;
    pthread_t t;
    pthread_create(&t, NULL, moveAlien, (void *)ctx->head->data);
    // ALIEN *first = head->data;

    while (1)
    {
        ctx->mouse_pressed = 0;
        ctx->mouse_released = 0;
        /**
         */
        clickedAlien(ctx, ctx->head);
        al_wait_for_event(ctx->queue, &(ctx->event));
        /**
         */

        switch (ctx->event.type)
        {
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            ctx->mouse_pressed = 1;
            ctx->mouse_released = 0;
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            ctx->mouse_pressed = 0;
            ctx->mouse_released = 1;
            break;
        case ALLEGRO_EVENT_MOUSE_AXES:
            ctx->x = ctx->event.mouse.x;
            ctx->y = ctx->event.mouse.y;
            break;
        case ALLEGRO_EVENT_TIMER:
            /**
             * Logic here 
             */
            ctx->redraw = true;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            ctx->done = true;
            break;
        }
        if (ctx->done)
        {
            printf("Done\n");
            break;
        }
        /**
         */
        handleMenu(ctx);
        if (ctx->redraw && al_is_event_queue_empty(ctx->queue))
        {

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(ctx->background, 0, 0, 1);
            drawmap(ctx->map, ctx);

            if (count <= 0)
            {
                flag = !flag;
                count = rand() % 100;
                // ALIEN *myAlien = generateAlien(ctx);
                ALIEN *myAlien = NULL;
                if (myAlien != NULL)
                {
                    pthread_t t2;
                    pthread_create(&t2, NULL, moveAlien, (void *)myAlien);
                    ADD_Alien((ctx->head), myAlien);
                }
                else
                {
                    printf("QUEUE LLENA\n");
                }
            }
            PATH *primer = ctx->map[0];

            al_draw_bitmap(ctx->alfaCommunity, 5, 470, flag);
            al_draw_bitmap(ctx->betaCommunity, 765, 20, flag);
            char str[30];
            sprintf(str, " x: %d, y: %d", ctx->x, ctx->y);
            al_draw_text(ctx->font, al_map_rgb(255, 255, 255), 1100, 20, 0, str);
            if (ctx->alienSelected != NULL)
            {

                char strAlienStatus[40];
                sprintf(strAlienStatus, " x: %.2f, y: %.2f, type: %d,  id: %d ", ctx->alienSelected->x, ctx->alienSelected->y, ctx->alienSelected->type, ctx->alienSelected->id);
                al_draw_text(ctx->font, al_map_rgb(255, 255, 255), 1000, 658, 0, strAlienStatus);
            }
            else
            {
                al_draw_text(ctx->font, al_map_rgb(255, 255, 255), 1000, 658, 0, "No hay alien seleccionado");
            }

            drawMenu(ctx);
            drawBridge(ctx->eastBridge, ctx);
            drawBridge(ctx->midBridge, ctx);
            drawBridge(ctx->westBridge, ctx);

            tempNode = ctx->head;
            while (tempNode != NULL)
            {
                if (!tempNode->data->way->finished)
                {
                    ALLEGRO_BITMAP *image;
                    if (tempNode->data->type == alfa)
                    {
                        if (tempNode->data->selected == 1)
                        {
                            image = ctx->alfaSelected;
                        }
                        else
                        {
                            image = ctx->alfaImage;
                        }
                    }

                    else if (tempNode->data->type == beta)
                    {
                        if (tempNode->data->selected == 1)
                        {
                            image = ctx->betaSelected;
                        }
                        else
                        {
                            image = ctx->betaImage;
                        }
                    }

                    else if (tempNode->data->type == normal)
                    {
                        if (tempNode->data->selected == 1)
                        {
                            image = ctx->normalSelected;
                        }
                        else
                        {
                            image = ctx->normalImage;
                        }
                    }

                    al_draw_bitmap(image, tempNode->data->x, tempNode->data->y, 0);
                    tempNode = tempNode->next;
                }
                else
                {
                    NODE_ALIEN *temp2 = tempNode->next;
                    REMOVE_Alien(&(ctx->head), tempNode->data->id);
                    tempNode = temp2;
                }
            }

            al_flip_display();
            ctx->redraw = false;
            count--;
        }
    }
    finalize_gui(ctx);
}

void drawmap(PATH **map, GUI_CONTEXT *ctx)
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
        al_draw_bitmap(ctx->pathImage, alfaentry[i].x, alfaentry[i].y, 0);
        al_draw_bitmap(ctx->pathImage, betaentry[i].x, betaentry[i].y, 0);
    }

    for (int i = 0; i < EXIT_COUNT; i++)
    {
        al_draw_bitmap(ctx->pathImage, alfaexit[i].x, alfaexit[i].y, 0);
        al_draw_bitmap(ctx->pathImage, betaexit[i].x, betaexit[i].y, 0);
    }

    for (int i = 0; i < AVENUE_COUNT; i++)
    {
        al_draw_bitmap(ctx->pathImage, northavenueA[i].x, northavenueA[i].y, 0);
        al_draw_bitmap(ctx->pathImage, southavenueA[i].x, southavenueA[i].y, 0);
        al_draw_bitmap(ctx->pathImage, northavenueB[i].x, northavenueB[i].y, 0);
        al_draw_bitmap(ctx->pathImage, southavenueB[i].x, southavenueB[i].y, 0);
    }
}

void drawBridge(BRIDGE *mybridge, GUI_CONTEXT *ctx)
{
    for (int i = 0; i < mybridge->queueSize; i++)
    {
        al_draw_bitmap(ctx->queueImage, mybridge->queueNorth[i].x, mybridge->queueNorth[i].y, 0);
        al_draw_bitmap(ctx->queueImage, mybridge->queueSouth[i].x, mybridge->queueSouth[i].y, 0);
        al_draw_bitmap(ctx->pathImage, mybridge->exitNorth[i].x, mybridge->exitNorth[i].y, 0);
        al_draw_bitmap(ctx->pathImage, mybridge->exitSouth[i].x, mybridge->exitSouth[i].y, 0);
    }

    for (int i = 0; i < mybridge->length; i++)
    {
        al_draw_bitmap(ctx->passImage, mybridge->pass[i].x, mybridge->pass[i].y, 0);
    }
    int linesLeft[] = {
        mybridge->queueNorth[mybridge->queueSize - 1].x,
        mybridge->queueNorth[mybridge->queueSize - 1].y + 40,
        mybridge->pass[0].x,
        mybridge->pass[0].y,
        mybridge->pass[mybridge->length - 1].x,
        mybridge->pass[mybridge->length - 1].y + 40,
        mybridge->exitNorth[0].x,
        mybridge->exitNorth[0].y

    };
    int linesRight[] = {
        mybridge->exitSouth[0].x + 40,
        mybridge->exitSouth[0].y + 40,
        mybridge->pass[0].x + 40,
        mybridge->pass[0].y,
        mybridge->pass[mybridge->length - 1].x + 40,
        mybridge->pass[mybridge->length - 1].y + 40,
        mybridge->queueSouth[mybridge->queueSize - 1].x + 40,
        mybridge->queueSouth[mybridge->queueSize - 1].y};
    ALLEGRO_COLOR purple = al_map_rgb(183, 0, 255);

    for (int i = 0; i < 6; i += 2)
        al_draw_line(linesLeft[i], linesLeft[i + 1], linesLeft[i + 2], linesLeft[i + 3], purple, 3);

    for (int i = 0; i < 6; i += 2)
        al_draw_line(linesRight[i], linesRight[i + 1], linesRight[i + 2], linesRight[i + 3], purple, 3);
}

void *moveAlien(void *args)
{
    ALIEN *myAlien = (ALIEN *)args;
    int onRoad = !myAlien->way->finished;
    while (onRoad == 1)
    {
        if (myAlien != NULL)
        {
            onRoad = !myAlien->way->finished;
            // next_move(&myAlien->x,&myAlien->y,myAlien->way, myAlien->dx, myAlien->dy);
            next_move(myAlien);
            usleep(25000);
        }
        else
            break;
    }
    // printf("ROAD COMPLETED\n");
}

ALIEN *generateManualAlien(GUI_CONTEXT *ctx, enum origin start, enum alienType alien_t)
{
    // RANDOM COMMUNITY
    int emptyqueue = 1;
    if (start == alfaPlanet && (ctx->map[0][2]).blocked)
    {
        emptyqueue = 0;
        printf("SALIDA ALFA NO DISPONIBLE\n");
    }
    else if (start == betaPlanet && (ctx->map[6][2]).blocked)
    {
        emptyqueue = 0;
        printf("SALIDA BETA NO DISPONIBLE\n");
    }
    //  RANDOM BRIDGE
    BRIDGE *tempBridge;
    enum bridgePosition tempPos = (enum bridgePosition)(rand() % 3);
    if (tempPos == east)
    {
        tempBridge = ctx->eastBridge;
        // if(start == alfaPlanet){
        //     if(ctx->eastBridge->queueNorth[0].blocked  && ctx->eastBridge->queueNorth[ctx->eastBridge->queueSize-1].blocked ){
        //         emptyqueue = 0;
        //     }
        // }
        // else if(start == betaPlanet){
        //     if(ctx->eastBridge->queueSouth[0].blocked  && ctx->eastBridge->queueSouth[ctx->eastBridge->queueSize-1].blocked ){
        //         emptyqueue = 0;
        //     }
        // }
    }
    else if (tempPos == west)
    {
        tempBridge = ctx->westBridge;
        // if(start == alfaPlanet){
        //     if(ctx->westBridge->queueNorth[0].blocked  && ctx->westBridge->queueNorth[ctx->westBridge->queueSize-1].blocked ){
        //         emptyqueue = 0;
        //     }
        // }
        // else if(start == betaPlanet){
        //     if(ctx->westBridge->queueSouth[0].blocked  && ctx->westBridge->queueSouth[ctx->westBridge->queueSize-1].blocked ){
        //         emptyqueue = 0;
        //     }
        // }
    }
    else if (tempPos == mid)
    {
        tempBridge = ctx->midBridge;
        // if(start == alfaPlanet){
        //     if(ctx->midBridge->queueNorth[0].blocked  && ctx->midBridge->queueNorth[ctx->midBridge->queueSize-1].blocked ){
        //         emptyqueue = 0;
        //     }
        // }
        // else if(start == betaPlanet){
        //     if(ctx->midBridge->queueSouth[0].blocked  && ctx->midBridge->queueSouth[ctx->midBridge->queueSize-1].blocked ){
        //         emptyqueue = 0;
        //     }
        // }
    }
    ALIEN *newAlien;
    if (emptyqueue)
    {
        float initPosX, initPosY;
        ROUTE *tempRoute = create_route(&tempBridge, ctx->map, start);
        if (start == alfaPlanet)
        {
            // printf("ORIGING ALFA PLANET\n");
            initPosX = COMMUNITY_ALFA_POSX;
            initPosY = COMMUNITY_ALFA_POSY;
        }
        else if (start == betaPlanet)
        {
            // printf("ORIGING BETA PLANET\n");
            initPosX = COMMUNITY_BETA_POSX;
            initPosY = COMMUNITY_BETA_POSY;
        }
        newAlien = create_alien(countIDs, alien_t, &tempRoute, initPosX, initPosY, SPEED_NORMAL);
        countIDs++;
    }

    else
    {

        newAlien = NULL;
    }
    return newAlien;
}

void handleMenu(GUI_CONTEXT *ctx)
{
    ALIEN *temp = NULL;
    char str[30];
    char *planet = ctx->sideSelected == alfaPlanet ? "Alfa" : "Beta";
    if (ctx->mouse_released && ctx->x >= (1100) && ctx->x <= (1100 + 79) && ctx->y <= (70 + 79) && ctx->y >= (70))
    {
        printf("Creando normal en planeta: %s\n", planet);
        temp = generateManualAlien(ctx, ctx->sideSelected, normal);
        ctx->mouse_released = 0;
    }
    /*** Second**/
    if (ctx->mouse_released && ctx->x >= (1100) && ctx->x <= (1100 + 79) && ctx->y <= (160 + 79) && ctx->y >= (160))
    {
        printf("Creando alfa en planeta: %s \n", planet);
        temp = generateManualAlien(ctx, ctx->sideSelected, alfa);
        ctx->mouse_released = 0;
    }
    /*** Third**/
    if (ctx->mouse_released && ctx->x >= (1190) && ctx->x <= (1190 + 79) && ctx->y <= (160 + 79) && ctx->y >= (160))
    {
        printf("Creando beta en planeta: %s\n", planet);
        temp = generateManualAlien(ctx, ctx->sideSelected, beta);
        ctx->mouse_released = 0;
    }

    if (temp != NULL)
    {
        ADD_Alien((ctx->head), temp);
        pthread_t t;
        pthread_create(&t, NULL, moveAlien, (void *)temp);
    }
}

void drawMenu(GUI_CONTEXT *ctx)
{
    if (ctx->mouse_pressed && ctx->x >= (1100) && ctx->x <= (1100 + 79) && ctx->y <= (70 + 79) && ctx->y >= (70))
    {
        al_draw_bitmap(ctx->ap, 1100, 70, 0);
    }
    else
    {
        al_draw_bitmap(ctx->a, 1100, 70, 0);
    }
    /*** Second**/
    if (ctx->mouse_pressed && ctx->x >= (1100) && ctx->x <= (1100 + 79) && ctx->y <= (160 + 79) && ctx->y >= (160))
    {
        al_draw_bitmap(ctx->bp, 1100, 160, 0);
    }
    else
    {
        al_draw_bitmap(ctx->b, 1100, 160, 0);
    }
    /*** Third**/
    if (ctx->mouse_pressed && ctx->x >= (1190) && ctx->x <= (1190 + 79) && ctx->y <= (160 + 79) && ctx->y >= (160))
    {
        al_draw_bitmap(ctx->cp, 1190, 160, 0);
    }
    else
    {
        al_draw_bitmap(ctx->c, 1190, 160, 0);
    }
    /*** Switch**/
    if (ctx->mouse_pressed && ctx->x >= (1190) && ctx->x <= (1190 + 157 / 2) && ctx->y <= (70 + 79) && ctx->y >= (70))
    {
        ctx->sideSelected = alfaPlanet;
    }
    if (ctx->mouse_pressed && ctx->x >= (1190 + 157 / 2) && ctx->x <= (1190 + 157) && ctx->y <= (70 + 79) && ctx->y >= (70))
    {
        ctx->sideSelected = betaPlanet;
    }

    if (ctx->sideSelected == 0)
    {
        al_draw_bitmap(ctx->sideAlfa, 1190, 70, 0);
    }
    else
    {
        al_draw_bitmap(ctx->sideBeta, 1190, 70, 0);
    }
}

void clickedAlien(GUI_CONTEXT *ctx, NODE_ALIEN *head)
{

    NODE_ALIEN *temp = NULL;
    if (ctx->mouse_pressed == 1)
    {
        temp = head;

        int count = 0;
        while (temp != NULL)
        {
            if (temp->data != NULL)
            {
                count = count + 1;
                float dx = abs(temp->data->x - ctx->x);
                float dy = abs(temp->data->y - ctx->y);
                if (dx <= 20 && dy <= 20)
                {
                    printf("Index of alien selected %d\n", count);
                    // printf("dx: %f dy : %f\n", dx, dy);
                    if (ctx->alienSelected != NULL)
                    {
                        ctx->alienSelected->selected = 0;
                    }
                    ctx->alienSelected = temp->data;
                    ctx->alienSelected->selected = 1;
                    // printf("x: %f y : %f\n", alien->x, alien->y);
                    printf("x: %f y : %f %d\n", ctx->alienSelected->x, ctx->alienSelected->y, ctx->alienSelected->id);
                    break;
                }
            }
            temp = temp->next;
        }
    }
    else
    {
    }
}