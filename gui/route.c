#include "route.h"
#include "gui.h"




ROUTE * create_route(BRIDGE **bridge, PATH **map, enum origin start )
{ 
    ROUTE *newRoute = malloc(sizeof(ROUTE));
    if(start == alfaPlanet)
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



void next_move(ALIEN *alien)//)
{
    float *next_x = &alien->x;
    float *next_y = &alien->y;
    ROUTE *alienRoute = alien->way;
    float dx = alien->dx;
    float dy = alien->dy;
    int change_pos = 1;
    float tempx,tempy;

    if (alienRoute->diry == down){
        tempy = *next_y + dy; 
        if(tempy < alienRoute->current[alienRoute->pos].y){
            *next_y = tempy;
            change_pos = 0;
        }
    }
    if (alienRoute->diry == up){
        tempy = *next_y - dy; 
        if(tempy > alienRoute->current[alienRoute->pos].y){
            *next_y = tempy;
            change_pos = 0;
        }
    }
    
    if (alienRoute->dirx == left){
        tempx = *next_x - dx; 
        if(tempx > alienRoute->current[alienRoute->pos].x){
            *next_x = tempx;
            change_pos = 0;
        }
    }
    if (alienRoute->dirx == right){
        tempx = *next_x + dx; 
        if(tempx < alienRoute->current[alienRoute->pos].x){
            *next_x = tempx;
            change_pos = 0;
        }
    }

    
    if(change_pos)
    {
        enum state nextStatus = alien->status; 
        int tempPos = alienRoute->pos;
        int tempLimit = alienRoute->limit;
        PATH *nextPath = alienRoute->current;
        int enqueue = 0;
        int dequeue = 0;
        // CASO PARTICULAR
        if(alienRoute->current == alienRoute->bridge->pass && alienRoute->start==betaPlanet)
            tempPos--;
        else
            tempPos++;

        if (tempPos == tempLimit)
        {
            // Salida -> Avenida
            if(alienRoute->current == alienRoute->exit){
                nextPath = alienRoute->avenue1;
                tempPos = 2;
                if(alienRoute->start == alfaPlanet)
                {
                    if(alienRoute->bridge->position == east)
                        tempLimit = 6;
                    else if(alienRoute->bridge->position == mid)
                        tempLimit = 10;
                    else if(alienRoute->bridge->position == west)
                        tempLimit = 15;
                }

                else if(alienRoute->start == betaPlanet)
                {
                    if(alienRoute->bridge->position == east)
                        tempLimit = 15;
                    else if(alienRoute->bridge->position == mid)
                        tempLimit = 11;
                    else if(alienRoute->bridge->position == west)
                        tempLimit = 6;
                }
            }
            // Avenida -> Cola
            else if(alienRoute->current == alienRoute->avenue1){
                if(alienRoute->start == alfaPlanet){
                    enqueue = 1;
                    nextPath = alienRoute->bridge->queueNorth;
                }
                else if(alienRoute->start == betaPlanet){
                    nextPath = alienRoute->bridge->queueSouth;
                }
                nextStatus = ready;
                tempLimit = alienRoute->bridge->queueSize;
                tempPos = 0;
                
                // LLAMAR A CALENDARIZADOR AQUI ORDENE LA COLA
            }

            // Cola -> Puente
            else if(alienRoute->current == alienRoute->bridge->queueNorth){
                nextPath = alienRoute->bridge->pass;
                tempLimit = alienRoute->bridge->length;
                tempPos = 0;
                dequeue = 1;
                enqueue = 0;
                nextStatus = running;


            }
            // Cola -> Puente
            else if(alienRoute->current == alienRoute->bridge->queueSouth){
                nextPath = alienRoute->bridge->pass;
                tempLimit = -1;
                tempPos = alienRoute->bridge->length-1;
                nextStatus = running;

            }
            // Puente -> salida
            else if(alienRoute->current == alienRoute->bridge->pass){
                tempLimit = alienRoute->bridge->queueSize;
                tempPos = 0;
                nextStatus = terminated;

                if(alienRoute->start == alfaPlanet){
                    nextPath = alienRoute->bridge->exitNorth;
                }
                else if(alienRoute->start == betaPlanet){
                    nextPath = alienRoute->bridge->exitSouth;
                }

            }
            // Salida -> Avenida (Cruzar avenida)
            else if(alienRoute->current == alienRoute->bridge->exitNorth){
                nextPath = alienRoute->cross2avenue;
                if(alienRoute->bridge->position == east){
                    tempLimit = AVENUE_COUNT;
                    tempPos = AVENUE_COUNT-1;
                }
                else if(alienRoute->bridge->position == mid){
                    tempLimit = 12;
                    tempPos = 11;
                }
                else if(alienRoute->bridge->position == west){
                    tempLimit = 7;
                    tempPos = 6;
                }
                
            }
            // Salida -> Avenida (Cruzar avenida)
            else if(alienRoute->current == alienRoute->bridge->exitSouth){
                nextPath = alienRoute->cross2avenue;
                if(alienRoute->bridge->position == east){
                    tempLimit = 7;
                    tempPos = 6;
                }
                else if(alienRoute->bridge->position == mid){
                    tempLimit = 11;
                    tempPos = 10;
                }
                else if(alienRoute->bridge->position == west){
                    tempLimit = AVENUE_COUNT;
                    tempPos = AVENUE_COUNT-1;
                }
                
            }
            // Avenida -> Avenida (Cruzar avenida)
            else if(alienRoute->current==alienRoute->cross2avenue&& tempPos!=1){
                nextPath = alienRoute->avenue2;
                tempPos = AVENUE_COUNT - alienRoute->limit;
                tempLimit = AVENUE_COUNT;
            }
            // Avenida -> Entrada
            else if(alienRoute->current == alienRoute->avenue2 ){
                nextPath = alienRoute->cross2entry;
                tempLimit = 1;
                tempPos= 0;
            }
            // 
            else if(alienRoute->current == alienRoute->cross2entry){
                nextPath = alienRoute->entry;
                tempLimit = ENTRY_COUNT;
                tempPos= 0;
            }
            // Final de trayecto, llegada a comunidad.
            else if(alienRoute->current == alienRoute->entry){
                alienRoute->finished = 1;
            }

        }

        int available = can_move(alien, nextPath, tempPos);

        
        // LLAMAR AL SEMAFORO DEL PUENTE
        

        if(available){
            // Liberamos el actual
            nextPath[tempPos].blocked = 1;
            nextPath[tempPos].alienID = alien->id;
            alien->status = nextStatus;
            PATH *queuePath = alienRoute->bridge->queueNorth;
            PATH *past = alienRoute->current;
            int pastPos = alienRoute->pos;
            int sorted = 0;
            
            alienRoute->current = nextPath;
            alienRoute->limit = tempLimit;
            alienRoute->pos = tempPos;
             char* bridgeName;
                switch (alienRoute->bridge->position)
                {
                case 0:
                    bridgeName = "east";
                    break;
                case 1:
                    bridgeName = "mid";
                    break;
                case 2:
                    bridgeName = "west";
                    break;
                
                default:
                    break;
                }

            // printf("PUENTE: %s enqueue: %d dequeue: %d\n", bridgeName, enqueue, dequeue);
            if( enqueue && !dequeue )
            {
                NODE_ALIEN *head = (NODE_ALIEN*) alienRoute->bridge->northHead;
                if(head == NULL){
                    head = malloc(sizeof(NODE_ALIEN));
                    head->data = alien;
                    head->next = NULL;
                }
                else
                {
                    ADD_ALIEN( &head, alien);
                    schedule_list(&head, alienRoute->bridge->scheduler);               
                    set_sorted_path(head, queuePath, alienRoute->bridge->queueSize);
                    sorted = 1;             
                }
               
                printf("PUENTE: %s\n", bridgeName);
                print_list2(head, 0);
                alienRoute->bridge->northHead = (void *) head; 
            }
            else if ( dequeue )
            {
                NODE_ALIEN *head = (NODE_ALIEN*) alienRoute->bridge->northHead;
                REMOVE_ALIEN(&head, alien->id);
                print_list2(head, 0);
                alienRoute->bridge->northHead = (void *) head; 


            }
            // printf("DELETING ALIEN iD:%d FROm: %s ",alien->id, bridgeName);
            past[pastPos].blocked = 0;
            past[pastPos].alienID = -1;         
    
            /**
             * Direction of speed
            */ 
            // OCUPO IR A LA DERECHA
            if( *next_x < alienRoute->current[tempPos].x ){
                // printf("Derecha\n");
                alienRoute->dirx=right;

            }
            // OCUPO IR A LA IZQUIERDA
            else if(*next_x > alienRoute->current[tempPos].x){
                // printf("Izquierda\n");
                alienRoute->dirx=left;
            }

            // OCUPO BAJAR
            if (*next_y < alienRoute->current[tempPos].y ){
                alienRoute->diry=down;
                // printf("Bajando\n");
            }
            // OCUPO SUBIR
            else if (*next_y > alienRoute->current[tempPos].y ){
                alienRoute->diry=up;
                // printf("Subiendo\n");
            }
        }
    }
}

int can_move( ALIEN *alienMoving, PATH *nextPATH, int pos)
{ 
    
    int result = 0;
    if(alienMoving->way->finished){
        if(alienMoving->way->current[alienMoving->way->pos].alienID == alienMoving->id){
            alienMoving->way->current[alienMoving->way->pos].blocked = 0;
            alienMoving->way->current[alienMoving->way->pos].alienID = -1;
            // printf("CURRENT[%d] : %p -> BLOCKED:%d -> ALIEN ID KILLED IN FINISH:%d\n", alienMoving->way->pos, (void*) &(alienMoving->way->current[alienMoving->way->pos]), alienMoving->way->current[alienMoving->way->pos].blocked, alienMoving->way->current[alienMoving->way->pos].alienID);
        }
        result = 0;
    }  
    else if( nextPATH[pos].blocked ){
        result = 0;
    }
    else
    {   
        BRIDGE * myBridge = alienMoving->way->bridge;
        bool debug = 0;
        if(nextPATH == myBridge->queueNorth )
        {
            if(get_length( myBridge->northHead ) == myBridge->queueSize)
            {
                result = 0;
            }
            else
            {
                result = 1;
            }
            

        }
        else if(nextPATH == myBridge->pass && alienMoving->way->current != myBridge->pass)
        {      
            int nextholdup = alienMoving->weight + myBridge->holdup;
            if( nextholdup <= myBridge->strength){
                if(myBridge->yield && alienMoving->way->start == alfaPlanet){
                    
                    myBridge->holdup = nextholdup;
                    result = 1;
                    
                }
                else if (!myBridge->yield && alienMoving->way->start == betaPlanet)
                {
                    myBridge->holdup = nextholdup;
                    result = 1;
                    // printf("Puente: %d Coutner BetA: %d\n", myBridge->position, myBridge->countAliens);   
                }
                else
                {
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
            if(pos == 0){
                myBridge->holdup -= alienMoving->weight;
                myBridge->countAliens++;
                if(myBridge->countAliens >= 3)
                {
                    if(debug){
                        printf("CAMBIO DE LA VIA \n");
                    }  
                    myBridge->countAliens = 0;
                    myBridge->yield = !myBridge->yield;
                }
            } 
            result = 1;
        }
        else
        {
            result = 1;
        }
    } 
    return result;
}

void set_sorted_path(NODE_ALIEN *sorted_list, PATH *queuePATH, int sizequeue )
{
    int index = sizequeue - 1;
    NODE_ALIEN *temp = sorted_list;
    ROUTE *tempRoute = temp->data->way;
    for (index; index >= 0; index--){
        if(temp != NULL){
            if(!tempRoute->finished )
            {
                queuePATH[index].blocked = 1;
                queuePATH[index].alienID = temp->data->id;
                temp->data->way->current;
                temp->data->x = queuePATH[index].x;
                temp->data->y = queuePATH[index].y;
                temp->data->way->pos = index;
                temp = temp->next;
            }
        }
        else
        {
            queuePATH[index].alienID = -1;
            queuePATH[index].blocked = 0;
        }
    }
}
