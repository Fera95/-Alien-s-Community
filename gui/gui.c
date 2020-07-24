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

int cuenta1=0;
int cuenta2=0;

lpthread_t lista1[maxThread];
lpthread_t lista2[maxThread];


int countIDs = 0;


void must_init(bool test, const char *description)
{
    if (test)
        return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

// EXPONENCIAL FUNCTION
double ran_expo(double lambda)
{
    long idum = rand();
    double u;
    u = rand() / (RAND_MAX + 1.0);
    //   return -log(1 - u) / lambda;
    return 2;
}
/**
 * 
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

    /**
     * LOADING FILES
     */ 
    bool ERROR_LOADING = 0;
    create_map(ctx);
    config_bridge temp = load_bridge(east);
    create_bridge(&ctx->eastBridge, temp, east);
    
    temp = load_bridge(west);
    create_bridge(&ctx->westBridge, temp, west);
    
    temp = load_bridge(mid);
    create_bridge(&ctx->midBridge, temp, mid);

    if(ctx->eastBridge == NULL ||ctx->midBridge == NULL || ctx->westBridge == NULL )
    {
        ERROR_LOADING = 1;
    }
    
    ctx->config = load_alien();
    if(ctx->config.base_speed > 10 || ctx->config.base_speed < 0 )
    {
        ERROR_LOADING = 1;
        printf("ERROR: INVALID BASE SPEED FOR NORMAL ALIENS\n");

    }
    int total_percent = ctx->config.normal_percent + ctx->config.alfa_percent + ctx->config.beta_percent;
    if(total_percent != 100){
        ERROR_LOADING = 1;
        printf("ERROR: INVALID PERCENTS FOR ALIENS GENERATIONS, ADD NOT EQUAL TO 100\n");
        
    }
    if(ctx->config.medium_time_creation < 0 || ctx->config.medium_time_creation > 60)
    {
           ERROR_LOADING = 1;
           printf("ERROR: INVALID MEDIUM TIME FOR CREATION\n");
    }
    else
    {
        ctx->generate = 0;
        printf("Wait time: %d\n",ctx->config.medium_time_creation);
        ctx->waitTime = ran_expo((double) ctx->config.medium_time_creation);
        printf("EXPO Wait time: %d\n",ctx->waitTime);
    }
    
    if(ERROR_LOADING){
        printf("CLOSING PROGRAM\n");
        kill(getpid(),SIGINT);
    }
    ctx->alienSelected = NULL;
    ctx->mouse_pressed = 0;
    ctx->done = false;
    ctx->head = NULL;
    ctx->redraw = true;
    ctx->mouse_released = 0;
    ctx->sideSelected = alfa;
    ctx->run = true;
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
   
    if(al_load_bitmap("assets/background.png")!=NULL){
        ctx->background = al_load_bitmap("assets/background.png");
    }    else{
        printf("Error imagen");
   }
   if( al_load_bitmap("assets/path.png")!=NULL){
       ctx->pathImage = al_load_bitmap("assets/path.png");
   }    else{
        printf("Error imagen");
   }
   
    
    
      if(al_load_bitmap("assets/bridge2.png")!=NULL){
        ctx->passImage = al_load_bitmap("assets/bridge2.png");
     }    
       else{
        printf("Error imagen");
   }
   
      if( al_load_bitmap("assets/queue.png")!=NULL){
        ctx->queueImage = al_load_bitmap("assets/queue.png");
     }
        else{
        printf("Error imagen");
   }
   
      if(al_load_bitmap("assets/alfaplanet.png")!=NULL){
       ctx->alfaCommunity = al_load_bitmap("assets/alfaplanet.png");
     }
      else{
        printf("Error imagen");
   }
    
      if( al_load_bitmap("assets/betaplanet.png")!=NULL){
       ctx->betaCommunity = al_load_bitmap("assets/betaplanet.png");
    }
     else{
       printf("Error imagen");
   }
    
      if(al_load_bitmap("assets/beta25.png")!=NULL){
        ctx->betaImage = al_load_bitmap("assets/beta25.png");
   }
    else{
       printf("Error imagen");
   }
   
      if(al_load_bitmap("assets/alfa25.png")!=NULL){
       ctx->alfaImage = al_load_bitmap("assets/alfa25.png");
   }
    else{
       printf("Error imagen");
   }
    
      if( al_load_bitmap("assets/normal25.png")!=NULL){
       ctx->normalImage = al_load_bitmap("assets/normal25.png");
    }
     else{
       printf("Error imagen");
   }
    
      if(  al_load_bitmap("assets/alfaSelected.png")!=NULL){
         ctx->alfaSelected = al_load_bitmap("assets/alfaSelected.png");
     }
      else{
       printf("Error imagen");
   }

  
      if(al_load_bitmap("assets/betaSelected.png")!=NULL){
        ctx->betaSelected = al_load_bitmap("assets/betaSelected.png");
    }
     else{
       printf("Error imagen");
   }
   
      if(al_load_bitmap("assets/normalSelected.png")!=NULL){
         ctx->normalSelected = al_load_bitmap("assets/normalSelected.png");
    }
     else{
       printf("Error imagen");
   }
  
      if(al_load_bitmap("assets/a.png")!=NULL){
        ctx->a = al_load_bitmap("assets/a.png");
    }
     else{
       printf("Error imagen");
   }

   
      if(al_load_bitmap("assets/ap.png")!=NULL){
         ctx->ap = al_load_bitmap("assets/ap.png");
   }
    else{
       printf("Error imagen");
   }
  
      if(al_load_bitmap("assets/b.png")!=NULL){
        ctx->b = al_load_bitmap("assets/b.png");
   }
    else{
       printf("Error imagen");
   }
   
      if(al_load_bitmap("assets/bp.png")!=NULL){
       ctx->bp = al_load_bitmap("assets/bp.png");
   }
    else{
       printf("Error imagen");
   }
    
      if(al_load_bitmap("assets/c.png")!=NULL){
       ctx->c = al_load_bitmap("assets/c.png");
   }
    else{
       printf("Error imagen");
   }
    
      if(al_load_bitmap("assets/cp.png")!=NULL){
         ctx->cp = al_load_bitmap("assets/cp.png");
   }
    else{
       printf("Error imagen");
   }
  
      if(al_load_bitmap("assets/i.png")!=NULL){
        ctx->i = al_load_bitmap("assets/i.png");
   }
    else{
       printf("Error imagen");
   }
   
      if(al_load_bitmap("assets/ip.png")!=NULL){
        ctx->ip = al_load_bitmap("assets/ip.png");
   }
    else{
       printf("Error imagen");
   }
   
      if( al_load_bitmap("assets/sideAlfa.png")!=NULL){
          ctx->sideAlfa = al_load_bitmap("assets/sideAlfa.png");
   }
    else{
       printf("Error imagen");
   }
 
      if(al_load_bitmap("assets/sideBeta.png")!=NULL){
        ctx->sideBeta = al_load_bitmap("assets/sideBeta.png");
   }
   else{
       printf("Error imagen");
   }
   
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
    int x, y;
    for (int i = 0; i < ENTRY_COUNT; i++)
    {
        x = posxInit;
        y = posyInit + 40 * i;
        create_path(&alfaentry[i],x,y,40,40);
    }

    // Alfa Community Exit
    posxInit = 150;
    posyInit = 500;
    for (int i = 0; i < EXIT_COUNT; i++)
    {
        x = posxInit;
        y = posyInit - 40 * i;
        create_path(&alfaexit[i],x,y,40,40);
    }
    posxInit = 670;
    posyInit = 20;
    int k = 0;
    // North Avenue
    for (int i = 0; i < AVENUE_COUNT; i++)
    {
        x = posxInit - 40 * k;
        y = posyInit;
        create_path(&northavenueB[i],x,y,40,40);
        k++;   
    }
    posxInit = 70;
    posyInit += 40;
    k = 0;
    for (int i = 0; i < AVENUE_COUNT; i++)
    {
        x = posxInit + 40 * k;
        y = posyInit;
        create_path(&northavenueA[i],x,y,40,40);
        k++;
    }
    // #########################################
    // Beta Community Exit
    posxInit = 790;
    posyInit = 140;
    for (int i = 0; i < EXIT_COUNT; i++)
    {
        x = posxInit;
        y = posyInit + 40 * i;
        create_path(&betaexit[i],x,y,40,40);
    }

    // Beta Community enter
    posxInit = 870;
    posyInit = 100 + EXIT_COUNT * 40;
    for (int i = 0; i < ENTRY_COUNT; i++)
    {
        x = posxInit;
        y = posyInit - 40 * i;
        create_path(&betaentry[i],x,y,40,40);
    }

    posxInit = 870;
    posyInit = 580;
    k = 0;
    // South Avenue
    for (int i = 0; i < AVENUE_COUNT; i++)
    {
        x = posxInit - 40 * k;
        y = posyInit;
        create_path(&southavenueB[i],x,y,40,40);
        k++;
    }
    posxInit = 910 - AVENUE_COUNT * 40;
    posyInit += 40;
    k = 0;
    for (int i = 0; i < AVENUE_COUNT; i++)
    {
        x = posxInit + 40 * k;
        y = posyInit;
        create_path(&southavenueA[i],x,y,40,40);
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

int get_type(GUI_CONTEXT *ctx)
{
    int percentAlfa = ctx->config.alfa_percent;
    int percentBeta = ctx->config.beta_percent;
    int percentNormal = ctx->config.normal_percent;
    int arrayPercent[100];
    int index = 0;
    for (int i = 0; i < percentAlfa; i++)
    {
        arrayPercent[index] = alfa;
        index++;
    }
    for (int i = 0; i < percentBeta; i++)
    {
        arrayPercent[index] = beta;
        index++;
    }
    for (int i = 0; i < percentNormal; i++)
    {
        arrayPercent[index] = normal;
        index++;
    }
    int select = rand()%100;
    // printf("array[%d] = %d\n",select, arrayPercent[select]);
    return arrayPercent[select];

}
/**
 * CREATE A RANDOM ALIEN 
 */
ALIEN *generateAlien(GUI_CONTEXT *ctx)
{
 
    enum alienType type = (enum alienType) get_type(ctx); // RANDOM TYPE
    enum origin start = (enum origin)(rand() % 2);      // RANDOM COMMUNITY
    int emptyqueue = 1;
    if (start == alfaPlanet && (ctx->map[0][2]).blocked)
    {
        emptyqueue = 0;
    }
    else if (start == betaPlanet && (ctx->map[6][2]).blocked)
    {
        emptyqueue = 0;
    }

    BRIDGE *tempBridge;                                 // RANDOM BRIDGE
    enum bridgePosition tempPos = (enum bridgePosition)(rand() % 3);
    if (tempPos == east)
    {
        tempBridge = ctx->eastBridge;
    }
    else if (tempPos == west)
    {
        tempBridge = ctx->westBridge;
    }
    else if (tempPos == mid)
    {
        tempBridge = ctx->midBridge;
    }
    ALIEN *newAlien;
    if (emptyqueue)
    {
        float initPosX, initPosY;
        ROUTE *tempRoute = create_route(&tempBridge, ctx->map, start);
        if (start == alfaPlanet)
        {
            initPosX = COMMUNITY_ALFA_POSX;
            initPosY = COMMUNITY_ALFA_POSY;
        }
        else if (start == betaPlanet)
        {
            initPosX = COMMUNITY_BETA_POSX;
            initPosY = COMMUNITY_BETA_POSY;
        }
        newAlien = create_alien(countIDs, type, &tempRoute, initPosX, initPosY, 1); //ctx->config.base_speed
        countIDs++;
    }

    else
    {
        newAlien = NULL;
    }
    return newAlien;
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
    }
    else if (tempPos == west)
    {
        tempBridge = ctx->westBridge;
    }
    else if (tempPos == mid)
    {
        tempBridge = ctx->midBridge;
    }
    ALIEN *newAlien = NULL;
    if (emptyqueue)
    {
        float initPosX, initPosY;
        ROUTE *tempRoute = create_route(&tempBridge, ctx->map, start);
        if (start == alfaPlanet)
        {
            initPosX = COMMUNITY_ALFA_POSX;
            initPosY = COMMUNITY_ALFA_POSY;
        }
        else if (start == betaPlanet)
        {
            // printf("ORIGING BETA PLANET\n");
            initPosX = COMMUNITY_BETA_POSX;
            initPosY = COMMUNITY_BETA_POSY;
        }
        // printf("Creando (MANUAL) nuevo alien %d\n",countIDs);
        newAlien = create_alien(countIDs, alien_t, &tempRoute, initPosX, initPosY, ctx->config.base_speed);
        countIDs++;
    }

    else
    {

        newAlien = NULL;
    }
    return newAlien;
}

/**
 * DRAW AND MOVING FUNCTIONS
 */
// WAIT FOR A PERIOD OF TIME
void * wait_generation(void *arg )
{
    GUI_CONTEXT *ctx = (GUI_CONTEXT *)arg;
    while (1)
    {
        if (ctx->done)
        {
            break;
        }
        ctx->generate = 1;
        sleep(ctx->waitTime);
    }
}

int loop_gui(GUI_CONTEXT *ctx)
{
    al_start_timer(ctx->timer);

    timer = al_create_timer(1.0 / 60);
    al_start_timer(timer);
    lpthread_t t;
    lpthread_create(&t, NULL, wait_generation, (void *)ctx);
    int flag = 0;
    while (1)
    {
        clickedAlien(ctx, ctx->head);
        al_wait_for_event(ctx->queue, &(ctx->event));

        switch (ctx->event.type)
        {
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                ctx->mouse_pressed = 1;
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                ctx->mouse_released = 1;
                handleMenu(ctx);
                ctx->mouse_released = 0;
                ctx->mouse_pressed = 0;
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
        
        if (ctx->redraw && al_is_event_queue_empty(ctx->queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(ctx->background, 0, 0, 1);
            drawmap(ctx->map, ctx);

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

            NODE_ALIEN *tempNode = ctx->head;
            while (tempNode != NULL)
            {
                if (!tempNode->data->deleted)
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
                    DELETE_ALIEN(&(ctx->head), tempNode->data->id);
                    tempNode = temp2;
                    
                }
            }

            al_flip_display();
            ctx->redraw = false;
            
            /**
             * ALIEN GENERATOR IN A RANDOM NUMBER OF FRAMES
             */
            if (ctx->generate && ctx->run )
            {
                ctx->generate = 0;
                flag = !flag;
                ALIEN *myAlien = generateAlien(ctx);
                if (myAlien != NULL)
                {
                    lpthread_t t2;
                    t2 = lista2[cuenta2];
                    lpthread_create(&t2, NULL, moveAlien, (void *)myAlien);
                    ADD_ALIEN(&(ctx->head), myAlien);
                    printf("NEW ALIEN ID: %d\n",myAlien->id);
                    printf("Cuenta2 %d",cuenta2);
                    cuenta2++;
                }
            }
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
            next_move(myAlien);
            usleep(25000);
        }
        else
        {
            break;
        }
    }
    myAlien->deleted = 1; 
}



/** 
 * MENU RELATED FUNCTIONS
 * 
 */ 
void handleMenu(GUI_CONTEXT *ctx)
{
    ALIEN *temp = NULL;
    char str[30];
    char *planet = ctx->sideSelected == alfaPlanet ? "Alfa" : "Beta";
    if (ctx->mouse_released && ctx->x >= (1100) && ctx->x <= (1100 + 79) && ctx->y <= (70 + 79) && ctx->y >= (70))
    {
        // printf("Creando normal en planeta: %s\n", planet);
        temp = generateManualAlien(ctx, ctx->sideSelected, normal);
        ctx->mouse_released = 0;
    }
    /*** Second**/
    if (ctx->mouse_released && ctx->x >= (1100) && ctx->x <= (1100 + 79) && ctx->y <= (160 + 79) && ctx->y >= (160))
    {
        // printf("Creando alfa en planeta: %s \n", planet);
        temp = generateManualAlien(ctx, ctx->sideSelected, alfa);
        ctx->mouse_released = 0;
    }
    /*** Third**/
    if (ctx->mouse_released && ctx->x >= (1190) && ctx->x <= (1190 + 79) && ctx->y <= (160 + 79) && ctx->y >= (160))
    {
        // printf("Creando beta en planeta: %s\n", planet);
        temp = generateManualAlien(ctx, ctx->sideSelected, beta);
        ctx->mouse_released = 0;
    }

    if (temp != NULL)
    {
        ADD_ALIEN(&(ctx->head), temp);
        lpthread_t t;
        t=lista1[cuenta1];
        lpthread_create(&t, NULL, moveAlien, (void *)temp);
            printf("Cuenta1 %d",cuenta1);
        cuenta1++;
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

    /**     * Fourth **/
    if (ctx->mouse_pressed && ctx->x >= (1010) && ctx->x <= (1010 + 79) && ctx->y <= (70 + 79) && ctx->y >= (70))
    {
        al_draw_bitmap(ctx->ip, 1010, 70, 0);
    }
    else
    {
        al_draw_bitmap(ctx->i, 1010, 70, 0);
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
    if(head!=NULL)
    {
        temp = head;
        while (temp != NULL )
        {
            if (temp->data != NULL)
            {
                float dx = abs(ctx->x - temp->data->x - 20 );
                float dy = abs(ctx->y - temp->data->y - 20 );
                if (dx <= 10 && dy <= 10)
                {
                    if (ctx->alienSelected != NULL)
                    {
                        ctx->alienSelected->selected = 0;
                    }
                    ctx->alienSelected = temp->data;
                    ctx->alienSelected->selected = 1;
                    if (ctx->mouse_pressed == 1 && temp->data->selected)
                    {
                        KILL_ALIEN( temp->data );
                    } 
                    break;
                }
            }
            temp = temp->next;
        }
    }
}