#include "bridge.h"

BRIDGE* create_bridge (int length, int weight, enum bridgePosition position, ALLEGRO_BITMAP *queueImage, ALLEGRO_BITMAP *passImage, ALLEGRO_BITMAP *pathImage )
{
    BRIDGE * newBridge = malloc(sizeof(BRIDGE));
    int queueSizeTemp = (int) (TOTAL_LENGTH - length)/2;
    newBridge->queueSize = queueSizeTemp;
    newBridge->passSize = length;
    newBridge->queueNorth = malloc(newBridge->queueSize*sizeof(PATH));
    newBridge->queueSouth = malloc(newBridge->queueSize*sizeof(PATH));
    newBridge->exitNorth = malloc(newBridge->queueSize*sizeof(PATH));
    newBridge->exitSouth = malloc(newBridge->queueSize*sizeof(PATH));
    newBridge->pass = malloc(newBridge->passSize*sizeof(PATH));   
    
    newBridge->position = position;
    // CAMBIAR POR UN ENUMERATE
    int init_posx;
    if (position == east)
        init_posx = EAST_POSX; 

    else if (position == mid)
        init_posx = MID_POSX;

    else if(position == west)
        init_posx = WEST_POSX; 
    
    
    for (int i = 0; i < queueSizeTemp; i++)
    {
        /**
         *  SETTING VALUES
         */
        // POSITION X AXIS
        newBridge->queueNorth[i].x = init_posx ;
        newBridge->queueSouth[i].x = init_posx ;
        newBridge->exitNorth[i].x = init_posx  + 40;
        newBridge->exitSouth[i].x = init_posx  + 40;

        // POSITION Y AXIS
        newBridge->queueNorth[i].y = INIT_POSY + 40*i;
        newBridge->queueSouth[i].y = END_POSY + 40*i- 40*queueSizeTemp;
        newBridge->exitNorth[i].y = INIT_POSY + 40*i;
        newBridge->exitSouth[i].y = END_POSY + 40*i- 40*queueSizeTemp;

        // IMAGE
        newBridge->queueNorth[i].image = queueImage;
        newBridge->queueSouth[i].image = queueImage;
        newBridge->exitNorth[i].image = pathImage;
        newBridge->exitSouth[i].image = pathImage;

        // BLOCKED 
        newBridge->queueNorth[i].blocked = 0;
        newBridge->queueSouth[i].blocked = 0;
        newBridge->exitNorth[i].blocked = 0;
        newBridge->exitSouth[i].blocked = 0;

        // METERING
        newBridge->queueNorth[i].height = 40;
        newBridge->queueSouth[i].height = 40;
        newBridge->exitNorth[i].height = 40;
        newBridge->exitSouth[i].height = 40;
        newBridge->queueNorth[i].width = 40;
        newBridge->queueSouth[i].width = 40;
        newBridge->exitNorth[i].width = 40;
        newBridge->exitSouth[i].width = 40;
    }
    
    // SETTING THE 
    int posxOff = init_posx + 20;
    int posyOff = INIT_POSY + 5 + 40 * newBridge->queueSize;
    if (length%2!=0)
        posyOff+=20;
    
    for (int i = 0; i < newBridge->passSize; i++)
    {
        newBridge->pass[i].x = posxOff;
        newBridge->pass[i].y = posyOff + 40*i;
        newBridge->pass[i].image = passImage;
        newBridge->pass[i].blocked = 0;    
        newBridge->pass[i].height = 40;
        newBridge->pass[i].width = 40;    
    } 
    
    return  newBridge;
}