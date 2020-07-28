#include "route.h"
#include "gui.h"

ROUTE *create_route(BRIDGE **bridge, PATH **map, enum origin start)
{
    ROUTE *newRoute = malloc(sizeof(ROUTE));
    if (start == alfaPlanet)
    {
        newRoute->exit = map[0];
        newRoute->avenue1 = map[2];
        //
        newRoute->cross2avenue = map[5];
        newRoute->avenue2 = map[4];
        newRoute->cross2entry = map[5];
        newRoute->entry = map[7];
        newRoute->diry = up;
        newRoute->dirx = right;
    }
    else if (start == betaPlanet)
    {
        newRoute->exit = map[6];
        newRoute->avenue1 = map[5];

        newRoute->cross2avenue = map[2];
        newRoute->avenue2 = map[3];
        newRoute->cross2entry = map[2];
        newRoute->entry = map[1];
        newRoute->diry = down;
        newRoute->dirx = left;
    }
    newRoute->bridge = *bridge;
    newRoute->current = newRoute->exit;
    newRoute->pos = 0;
    // printf("FIRST POSITION of CURRENT type %d: X = %f, Y = %f\n", newRoute->start, newRoute->current->x, newRoute->current->y);
    newRoute->limit = EXIT_COUNT;
    newRoute->start = start;
    newRoute->finished = 0;
    return newRoute;
}


void set_waiting(ALIEN * alien){
    float *next_x = &alien->x;
    float *next_y = &alien->y;

    alien->RR_pos = alien->way->pos;
    alien->status = waiting;
    PATH* pastPath = alien->way->current;
    int pastPos = alien->way->pos;


    
    if(alien->way->start ==  alfaPlanet){
        alien->leftPixels -= 40*(alien->way->pos);        
    }
    else {
        alien->leftPixels -= 40*(alien->way->bridge->length - alien->way->pos) *alien->dy;
    }

    alien->sleep = 1;

    NODE_ALIEN * queue;
    PATH * queuePath;
    alien->way->pos = 0;

    if(alien->way->start ==  alfaPlanet){
        *next_x = alien->way->bridge->queueNorth[0].x;
        *next_x -= 40;
        *next_y = alien->way->bridge->queueNorth[0].y;
        queue = (NODE_ALIEN* ) alien->way->bridge->northHead;
        alien->way->current = alien->way->bridge->queueNorth;
        queuePath = alien->way->bridge->queueNorth;
    
    }
    else if(alien->way->start ==  betaPlanet){
        *next_x = alien->way->bridge->queueSouth[0].x + 40;
        *next_y = alien->way->bridge->queueSouth[0].y;
        queue = (NODE_ALIEN* )alien->way->bridge->southHead;
        queuePath = alien->way->bridge->queueSouth;



    }
    NODE_ALIEN * cross = (NODE_ALIEN* )alien->way->bridge->crossing;
    ADD_ALIEN(&queue, alien);
    int cardinal;
    if(alien->way->start ==  alfaPlanet){
        alien->way->bridge->northHead = (void* )queue;
        cardinal=1;
    }
    else if (alien->way->start ==  betaPlanet) {
        alien->way->bridge->southHead = (void* )queue;
        cardinal=0;
    }
    

    REMOVE_ALIEN (&cross, alien->id);
    alien->way->bridge->holdup = alien->way->bridge->holdup - (alien->weight);
    alien->way->bridge->crossing= (void* )cross;
    if(get_length(cross)==0){
        alien->way->bridge->waiting = 0;
    }
    
    int len =alien->way->bridge->queueSize-1;
    for (int i = len; i >= 0 ; i--)
    {
        if(!queuePath[i].blocked ){
            queuePath[i].blocked = 1;
            queuePath[i].alienID = alien->id;
            if(cardinal){
                alien->way->bridge->IdsNorth[i]=alien->id;
            }
            else
            {
                alien->way->bridge->IdsSouth[i]=alien->id;
                
            }
            
            alien->way->current = queuePath;
            alien->way->pos = i;
            alien->way->limit = alien->way->bridge->queueSize;
            alien->x = queuePath[i].x;
            alien->y = queuePath[i].y;
            alien->sleep = 0;
            break;
        }    
    }

    if(pastPath[pastPos].alienID == alien->id){
        pastPath[pastPos].blocked = 0;
        pastPath[pastPos].alienID = -1;
    }
    if(alien->way->bridge->scheduler == ShortestFirst){
        if(cardinal){
            printf("SHORTEST FIRST %d, POS %d\n", cardinal, alien->way->bridge->position);
            print_list2((NODE_ALIEN*) (alien->way->bridge->northHead),0);
            schedule_list((NODE_ALIEN**) ( &(alien->way->bridge->northHead)),alien->way->bridge->scheduler);
        }
        else{
            printf("SHORTEST FIRST %d, POS %d\n", cardinal, alien->way->bridge->position);
            print_list2((NODE_ALIEN*) (alien->way->bridge->northHead),0);
            schedule_list((NODE_ALIEN**) ( &(alien->way->bridge->northHead)),alien->way->bridge->scheduler);
        }
    }     
}

int check_RoundRobin(ALIEN *alienMoving, PATH *nextPATH, int pos) {

}

void next_move(ALIEN *alien) //)
{
    float *next_x = &alien->x;
    float *next_y = &alien->y;
    ROUTE *alienRoute = alien->way;
    float dx = alien->dx;
    float dy = alien->dy;
    int change_pos = 1;
    float tempx, tempy;
    int move = 1;
    if(alien->way->bridge->scheduler == RoundRobin || alien->way->bridge->scheduler == ShortestFirst){
        // printf("%p\n",(void *)alien->way->bridge, );
        if(alien->status == running && alien->way->bridge->waiting){
            change_pos = 0;
            set_waiting(alien);
        }
        else if (alien->status == waiting) {
            if(alien->sleep){
                change_pos = 0;
                move=0;
            }
            else{
                move = 1;
            }
        }
    }

    if (alienRoute->diry == down && move)
    {
        tempy = *next_y + dy;
        if (tempy < alienRoute->current[alienRoute->pos].y)
        {
            *next_y = tempy;
            change_pos = 0;
        }
    }
    
    if (alienRoute->diry == up && move)
    {
        tempy = *next_y - dy;
        if (tempy > alienRoute->current[alienRoute->pos].y)
        {
            *next_y = tempy;
            change_pos = 0;
        }
    }

    if (alienRoute->dirx == left && move)
    {
        tempx = *next_x - dx;
        if (tempx > alienRoute->current[alienRoute->pos].x)
        {
            *next_x = tempx;
            change_pos = 0;
        }
    }
    
    if (alienRoute->dirx == right && move)
    {
        tempx = *next_x + dx;
        if (tempx < alienRoute->current[alienRoute->pos].x)
        {
            *next_x = tempx;
            change_pos = 0;
        }
    }

    if (change_pos){
        enum state nextStatus = -1;
        PATH *nextPath = alienRoute->current;
        int tempPos = alienRoute->pos;
        int tempLimit = alienRoute->limit;
        int enqueue = 0;
        int dequeue = 0;
        int crossed = 0;
        // CASO PARTICULAR
        if (alienRoute->current == alienRoute->bridge->pass && alienRoute->start == betaPlanet)
            tempPos--;
        else
            tempPos++;

        if (tempPos == tempLimit)
        {
            // Salida -> Avenida
            if (alienRoute->current == alienRoute->exit)
            {
                nextPath = alienRoute->avenue1;
                tempPos = 2;
                if (alienRoute->start == alfaPlanet)
                {
                    if (alienRoute->bridge->position == east)
                        tempLimit = 6;
                    else if (alienRoute->bridge->position == mid)
                        tempLimit = 10;
                    else if (alienRoute->bridge->position == west)
                        tempLimit = 15;
                }

                else if (alienRoute->start == betaPlanet)
                {
                    if (alienRoute->bridge->position == east)
                        tempLimit = 15;
                    else if (alienRoute->bridge->position == mid)
                        tempLimit = 11;
                    else if (alienRoute->bridge->position == west)
                        tempLimit = 6;
                }
            }
            // Avenida -> Cola
            else if (alienRoute->current == alienRoute->avenue1)
            {
                if (alienRoute->start == alfaPlanet)
                {
                    nextPath = alienRoute->bridge->queueNorth;
                }
                else if (alienRoute->start == betaPlanet)
                {
                    nextPath = alienRoute->bridge->queueSouth;
                }
                nextStatus = ready;
                tempLimit = alienRoute->bridge->queueSize;
                tempPos = 0;
                enqueue = 1;
            }

            // Cola -> Puente
            else if (alienRoute->current == alienRoute->bridge->queueNorth)
            {
                nextStatus = running;
                if(alien->status == waiting) {
                    // printf("SET TO ROUND ROBIN\n");
                    nextPath = alienRoute->bridge->pass;
                    tempPos = alien->RR_pos;
                }
                else if (alien->status == ready) {
                    nextPath = alienRoute->bridge->pass;
                    tempPos = 0;
                }
                
                tempLimit = alienRoute->bridge->length;
                dequeue = 1;

            }
            // Cola -> Puente
            else if (alienRoute->current == alienRoute->bridge->queueSouth)
            {
                if(alien->status == waiting) {
                    nextPath = alienRoute->bridge->pass;
                    tempPos = alien->RR_pos;
                }
                else if (alien->status == ready) {
                    nextPath = alienRoute->bridge->pass;
                    tempPos = alienRoute->bridge->length - 1;
                }
                nextStatus = running;                
                tempLimit = -1;
                dequeue = 1;
            }
            // Puente -> salida
            else if (alienRoute->current == alienRoute->bridge->pass)
            {
                tempLimit = alienRoute->bridge->queueSize;
                tempPos = 0;
                crossed = 1;
                nextStatus = terminated;
                if (alienRoute->start == alfaPlanet)
                {
                    nextPath = alienRoute->bridge->exitNorth;
                }
                else if (alienRoute->start == betaPlanet)
                {
                    nextPath = alienRoute->bridge->exitSouth;
                }
            }
            // Salida -> Avenida (Cruzar avenida)
            else if (alienRoute->current == alienRoute->bridge->exitNorth)
            {
                nextPath = alienRoute->cross2avenue;
                if (alienRoute->bridge->position == east)
                {
                    tempLimit = AVENUE_COUNT;
                    tempPos = AVENUE_COUNT - 1;
                }
                else if (alienRoute->bridge->position == mid)
                {
                    tempLimit = 12;
                    tempPos = 11;
                }
                else if (alienRoute->bridge->position == west)
                {
                    tempLimit = 7;
                    tempPos = 6;
                }
            }
            // Salida -> Avenida (Cruzar avenida)
            else if (alienRoute->current == alienRoute->bridge->exitSouth)
            {
                nextPath = alienRoute->cross2avenue;
                if (alienRoute->bridge->position == east)
                {
                    tempLimit = 7;
                    tempPos = 6;
                }
                else if (alienRoute->bridge->position == mid)
                {
                    tempLimit = 11;
                    tempPos = 10;
                }
                else if (alienRoute->bridge->position == west)
                {
                    tempLimit = AVENUE_COUNT;
                    tempPos = AVENUE_COUNT - 1;
                }
            }
            // Avenida -> Avenida (Cruzar avenida)
            else if (alienRoute->current == alienRoute->cross2avenue && tempPos != 1)
            {
                nextPath = alienRoute->avenue2;
                tempPos = AVENUE_COUNT - alienRoute->limit;
                tempLimit = AVENUE_COUNT;
            }
            // Avenida -> Entrada
            else if (alienRoute->current == alienRoute->avenue2)
            {
                nextPath = alienRoute->cross2entry;
                tempLimit = 1;
                tempPos = 0;
            }
            //
            else if (alienRoute->current == alienRoute->cross2entry)
            {
                nextPath = alienRoute->entry;
                tempLimit = ENTRY_COUNT;
                tempPos = 0;
            }
            // Final de trayecto, llegada a comunidad.
            else if (alienRoute->current == alienRoute->entry)
            {
                alienRoute->finished = 1;
            }
            else if (alien->status == waiting)
            {
                
            }
            
        }

        int available = can_move(alien, nextPath, tempPos);
        // LLAMAR AL SEMAFORO DEL PUENTE

        if (available)
        {
            nextPath[tempPos].blocked = 1;
            nextPath[tempPos].alienID = alien->id;


            PATH *past = alienRoute->current;
            PATH *queuePath;
            int pastPos = alienRoute->pos;

            if (alienRoute->start == alfaPlanet)
            {
                queuePath = alienRoute->bridge->queueNorth;
            }
            else if (alienRoute->start == betaPlanet)
            {
                queuePath = alienRoute->bridge->queueSouth;
            }

            alienRoute->current = nextPath;
            alienRoute->limit = tempLimit;
            alienRoute->pos = tempPos;

            if(alien->status == waiting){
                alien->x = nextPath[tempPos].x;
                alien->y = nextPath[tempPos].y;
            }
            if (nextStatus != -1)
            {
                alien->status = nextStatus;
            }

            int sorted = 0;
            if (enqueue)
            {

                NODE_ALIEN *head;
                if (alienRoute->start == alfaPlanet)
                {
                    head = (NODE_ALIEN *)alienRoute->bridge->northHead;
                }
                else if (alienRoute->start == betaPlanet)
                {
                    head = (NODE_ALIEN *)alienRoute->bridge->southHead;
                }

                if (head == NULL)
                {
                    head = malloc(sizeof(NODE_ALIEN));
                    head->data = alien;
                    head->next = NULL;
                }
                else
                {
                    ADD_ALIEN(&head, alien);
                    schedule_list(&head, alienRoute->bridge->scheduler);
                }
                sorted = 1;
                set_sorted_path(head, queuePath, alienRoute->bridge->queueSize);
                if (alienRoute->start == alfaPlanet)
                {
                    alienRoute->bridge->northHead = (void *)head;
                }
                else if (alienRoute->start == betaPlanet)
                {
                    alienRoute->bridge->southHead = (void *)head;
                }
            }
            else if (dequeue)
            {
                if (alienRoute->start == alfaPlanet)
                {
                    NODE_ALIEN *head = (NODE_ALIEN *)alienRoute->bridge->northHead;
                    REMOVE_ALIEN(&head, alien->id);
                    alienRoute->bridge->northHead = (void *)head;
                    int size = alienRoute->bridge->queueSize;
                    if(get_length(head) > size ){
                        ALIEN* sleepy = get_at(head, size);
                        sleepy->sleep=0;
                    }
                }
                else if (alienRoute->start == betaPlanet)
                {
                    NODE_ALIEN *head = (NODE_ALIEN *)alienRoute->bridge->southHead;
                    REMOVE_ALIEN(&head, alien->id);
                    alienRoute->bridge->southHead = (void *)head;
                    int size = alienRoute->bridge->queueSize;
                    if(get_length(head) > size ){
                        ALIEN* sleepy = get_at(head, size);
                        sleepy->sleep=0;
                    }
                }
                NODE_ALIEN *crossList = (NODE_ALIEN *)alienRoute->bridge->crossing;
                ADD_ALIEN(&crossList, alien);
                alienRoute->bridge->crossing = (void*) crossList;
                (alienRoute->bridge)->holdup = (alienRoute->bridge)->holdup + alien->weight;
                if (alienRoute->bridge->planner == Count){
                    (alienRoute->bridge->tempCount)++;
                }
            }
            else if (crossed)
            {
                NODE_ALIEN *crossList = (NODE_ALIEN *)alienRoute->bridge->crossing;
                REMOVE_ALIEN(&crossList, alien->id);
                (alienRoute->bridge)->holdup = (alienRoute->bridge)->holdup - alien->weight;
                alienRoute->bridge->crossing = (void *)crossList;
                (alienRoute->bridge->countAliens)++;
                if(alien->status == terminated && get_length(crossList) == 0){
                    alienRoute->bridge->waiting = 0;
                }
            }

          
            past[pastPos].blocked = 0;
            past[pastPos].alienID = -1;
            
            NODE_ALIEN * queue;
            int cardinal = 0;
            if(alien->way->start == alfaPlanet){
                cardinal=1;
                queue = (NODE_ALIEN*) alien->way->bridge->northHead;
            }
            else
            {
                queue = (NODE_ALIEN*) alien->way->bridge->southHead;

            }
            
            
            // int len =  alien->way->bridge->queueSize;
            // for (int i = 0; i < len; i++)
            // {
            //     if(cardinal){
            //         int pathID =  alien->way->bridge->queueNorth[i].alienID;
            //         ALIEN* temp = get_at(queue, i);
            //         if (temp!=NULL )
            //         {
            //             if(temp->id != pathID){
            //                 alien->way->bridge->queueNorth[i].blocked=0;
            //                 alien->way->bridge->queueNorth[i].alienID=-1;
            //             }
            //         }
            //         else
            //         {
            //             alien->way->bridge->queueNorth[i].blocked=0;
            //             alien->way->bridge->queueNorth[i].alienID=-1;
            //         }
            //     }
            // }
            
            
            /**
             * Direction of speed
            */
            // OCUPO IR A LA DERECHA
            if (*next_x < alienRoute->current[tempPos].x)
            {
                // printf("Derecha\n");
                alienRoute->dirx = right;
            }
            // OCUPO IR A LA IZQUIERDA
            else if (*next_x > alienRoute->current[tempPos].x)
            {
                // printf("Izquierda\n");
                alienRoute->dirx = left;
            }

            // OCUPO BAJAR
            if (*next_y < alienRoute->current[tempPos].y)
            {
                alienRoute->diry = down;
                // printf("Bajando\n");
            }
            // OCUPO SUBIR
            else if (*next_y > alienRoute->current[tempPos].y)
            {
                alienRoute->diry = up;
                // printf("Subiendo\n");
            }
        }
    }
}

int can_move(ALIEN *alienMoving, PATH *nextPATH, int pos)
{


    if(alienMoving->way->bridge->position == mid){
        int len = alienMoving->way->bridge->queueSize-1;
        NODE_ALIEN * headN = (NODE_ALIEN *) alienMoving->way->bridge->northHead;
        NODE_ALIEN * headS = (NODE_ALIEN *) alienMoving->way->bridge->southHead;
        printf("\nPATH LEN %d   N: %d\n",len, alienMoving->way->bridge->queueNorth[len].alienID);
        if(headN!=NULL){
        printf("ID HEAD     N: %d\n",headN->data->id);
        }
        else {
        printf("PATH LEN-1  N: NULL\n");
        }
        printf("\nPATH LEN %d   S: %d\n",len, alienMoving->way->bridge->queueNorth[len].alienID);
        if(headS!=NULL){
        printf("ID HEAD     S: %d\n",headS->data->id);
        }
        else {
        printf("PATH LEN-1  S: NULL\n");
        }
        
    }
    int result = 0;
    if (alienMoving->way->finished)
    {
        if (alienMoving->way->current[alienMoving->way->pos].alienID == alienMoving->id)
        {
            alienMoving->way->current[alienMoving->way->pos].blocked = 0;
            alienMoving->way->current[alienMoving->way->pos].alienID = -1;
        }
        result = 0;
    }
    else if (nextPATH[pos].blocked)
    {
        result = 0;
    }
    else
    {
        BRIDGE *myBridge = alienMoving->way->bridge;
        if (nextPATH == myBridge->queueNorth)
        {
            if (get_length(myBridge->northHead) >= myBridge->queueSize)
            {
                result = 0;
            }
            else if (myBridge->scheduler == RoundRobin || myBridge->scheduler == ShortestFirst) {
                int lenCola = get_length(myBridge->northHead);
                int lenPuente  = get_length(myBridge->crossing);                
                if ( lenCola+lenPuente >= myBridge->queueSize){
                    result = 0;
                }
                else
                {
                    result = 1;
                }
            }
            
            else
            {
                result = 1;
            }
        }
        else if (nextPATH == myBridge->queueSouth)
        {
            if (get_length(myBridge->southHead) >= myBridge->queueSize)
            {
                result = 0;
            }
            else if (myBridge->scheduler == RoundRobin || myBridge->scheduler == ShortestFirst) {
                if( alienMoving->status == new ){
                    int lenCola = get_length(myBridge->southHead);
                    int lenPuente  = get_length(myBridge->crossing);                
                    if ( lenCola+lenPuente >= myBridge->queueSize){
                        result = 0;
                    }
                    else
                    {
                        result = 1;
                    }
                }
                else
                {
                    result =1;
                }
                
            }
            else
            {
                result = 1;
            }
        }
        else if (nextPATH == myBridge->pass){
            if (alienMoving->status == running ){
                result = 1;
            }
            else if ((alienMoving->status == ready || alienMoving->status == waiting) && !myBridge->waiting) {

                if( (alienMoving->way->start == alfaPlanet && myBridge->yield == northYield) || (alienMoving->way->start == betaPlanet && myBridge->yield == southYield ))
                {
                    int nextHoldup = alienMoving->weight + myBridge->holdup;
                    if(nextHoldup <= myBridge->strength){

                        if (myBridge->planner == Count){
                            int nextCount = myBridge->tempCount + 1;
                            if(nextCount <= myBridge->planner_count){
                                result = 1;
                            }
                            else {
                                result = 0;
                            }
                        }
                        else if (myBridge->planner == Semaphore)
                        {
                            // double myTimeCross = myBridge->crossTime / alienMoving->dy;
                            if(0 < myBridge->tempTime){
                                result = 1;
                            }
                            else {
                                result = 0;
                            }
                        }
                        else if (myBridge->planner == Survive){
                            result = 1;
                        }
                    }
                    else {
                        result = 0;
                    }                    
                }
                else {
                    result = 0;
                }                
            }
            else
            {

                result = 0;
            }
        }
        else if (nextPATH == myBridge->exitNorth || nextPATH == myBridge->exitSouth)
        {
            result = 1;
        }
        else
        {
            result = 1;
        }
    }
    return result;
}

void set_sorted_path(NODE_ALIEN *sorted_list, PATH *queuePATH, int sizequeue)
{
    int index = sizequeue - 1;
    NODE_ALIEN *temp = sorted_list;
    ROUTE *tempRoute = temp->data->way;
    for (index; index >= 0; index--)
    {
        if (temp != NULL)
        {
            if (!tempRoute->finished || temp->data->status != running)
            {
                queuePATH[index].blocked = 1;
                queuePATH[index].alienID = temp->data->id;
                temp->data->way->current;
                temp->data->x = queuePATH[index].x;
                temp->data->y = queuePATH[index].y;
                temp->data->way->pos = index;
                temp = temp->next;
                // tempRoute->bridge->IdsNorth[index]./
            }
        }
        else
        {
            queuePATH[index].alienID = -1;
            queuePATH[index].blocked = 0;
        }
    }
}
