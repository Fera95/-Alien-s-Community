#include "route.h"
#include "gui.h"

ROUTE * create_route(BRIDGE **bridge, PATH **map, enum origin start )
{

    /**
     * ctx->map[0] = alfaexit;
        ctx->map[1] = alfaentry;
        ctx->map[2] = northavenueA;
        ctx->map[3] = northavenueB;
        ctx->map[4] = southavenueA;
        ctx->map[5] = southavenueB;
        ctx->map[6] = betaexit;
        ctx->map[7] = betaentry;
    * 
    */ 
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

    
    if(change_pos){

        int tempPos = alienRoute->pos;
        int tempLimit = alienRoute->limit;
        PATH *nextPath = alienRoute->current;
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
                    nextPath = alienRoute->bridge->queueNorth;
                }
                else if(alienRoute->start == betaPlanet){
                    nextPath = alienRoute->bridge->queueSouth;
                }
                tempLimit = alienRoute->bridge->queueSize;
                tempPos = 0;
                
                // LLAMAR A CALENDARIZADOR AQUI ORDENE LA COLA
            }

            // Cola -> Puente
            else if(alienRoute->current == alienRoute->bridge->queueNorth){
                nextPath = alienRoute->bridge->pass;
                tempLimit = alienRoute->bridge->length;
                tempPos = 0;

            }
            // Cola -> Puente
            else if(alienRoute->current == alienRoute->bridge->queueSouth){
                nextPath = alienRoute->bridge->pass;
                tempLimit = -1;
                tempPos = alienRoute->bridge->length-1;
            }
            // Puente -> salida
            else if(alienRoute->current == alienRoute->bridge->pass){
                tempLimit = alienRoute->bridge->queueSize;
                tempPos = 0;
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
                alienRoute->current[alienRoute->pos].blocked = 0;
            }

        }

        int available = can_move(alien, nextPath, tempPos);

        
        // LLAMAR AL SEMAFORO DEL PUENTE
        

        if(available){
            // if(alienRoute->current == alienRoute->bridge->queueNorth && alienRoute->bridge->position == east){
            //     printf("ALTERANDO A QUEUE\n");
            // }

            // Liberamos el actual
            alienRoute->current[alienRoute->pos].blocked = 0;
            alienRoute->current[alienRoute->pos].alienID = -1;

            // Asignar el nuevo espacio.
            alienRoute->current = nextPath;
            alienRoute->limit = tempLimit;
            alienRoute->pos = tempPos;
            alienRoute->current[alienRoute->pos].alienID = alien->id;
            // printf("NEW POS: %d\n",alienRoute->pos);
            // printf("NEW LIMIT: %d\n",alienRoute->limit);
            alienRoute->current[alienRoute->pos].blocked = 1;
            
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
        // retornar al estado`
        // else{
        //     printf("CAMINO BLOQUEADO\n");
        // }
    }
}

int can_move( ALIEN *alienMoving, PATH *nextPATH, int pos)
{

    
    int result;
    BRIDGE * myBridge = alienMoving->way->bridge;
    // if(myBridge->position == east){
    //     if(alienMoving->way->current != nextPATH)
    //     {
    //         printf("TRATANDO DE CAMBIAR DE ARREGLO\n\n");

    //     }
    // }
    if( nextPATH[pos].blocked ){
        result = 0;
        if(nextPATH == myBridge->queueNorth && myBridge->position == east ){
            // printf("NEXT_PATH[%d] BLOCKED:%d\n",pos, nextPATH[pos].blocked);
            for (int i = 0; i < 5; i++)
            {
                // printf("QUEUE[%d] bloqueado:%d\n",i, myBridge->queueNorth[i].blocked);            
            }
        }
    }
    else{

        if(nextPATH == myBridge->queueNorth && myBridge->position == east ){
            // printf("NEXT_PATH[%d] NOT BLOCKED:%d\n",pos, nextPATH[pos].blocked);
            for (int i = 0; i < 5; i++)
            {
                // printf("QUEUE[%d] bloqueado:%d\n",i, myBridge->queueNorth[i].blocked);            
            }
        }
        if(nextPATH == myBridge->pass && alienMoving->way->current != myBridge->pass)
        {
            if( !myBridge->full){

                if(myBridge->yield && alienMoving->way->start == alfaPlanet){
                    myBridge->full = 1;
                    result = 1;
                    myBridge->countAliens++;
                    // printf("Puente: %d Coutner Alfa:%d\n", myBridge->position, myBridge->countAliens);
                }
                else if (!myBridge->yield && alienMoving->way->start == betaPlanet)
                {
                    myBridge->full = 1;
                    result = 1;
                    myBridge->countAliens++;
                    // printf("Puente: %d Coutner BetA: %d\n", myBridge->position, myBridge->countAliens);   
                }
                else
                {
                    result = 0;
                }
            }
            else
                result = 0;
            
        }
        else if (nextPATH == myBridge->exitNorth || nextPATH == myBridge->exitSouth)
        {
            myBridge->full = 0;
            if(myBridge->countAliens >= 10)
            {
                myBridge->countAliens = 0;
                myBridge->yield = !myBridge->yield;
                // printf("CAMBIO DE Via a BETA en %d,  %d", myBridge->position, myBridge->yield);
            }

        }
        else
        {
            result = 1;
        }
    }
    // if( myBridge->position == east ){
    //     if(alienMoving->way->current != nextPATH || nextPATH == myBridge->queueNorth){
    //         printf("RETURN VALUE NORTH: %d\n",result);
    //     }
    // }
    return result;

}


