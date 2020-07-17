#include "bridge.h"

void create_bridge (BRIDGE **ctxBridge, int length, int strength, enum bridgePosition position, int scheduler )
{
    printf("CREATING \n");
    if(length > 10 || length < 0)
        length = 10;
    BRIDGE * newBridge = malloc(sizeof(BRIDGE));
    int queueSizeTemp = (int) (TOTAL_LENGTH - length)/2;
    newBridge->queueSize = queueSizeTemp;
    newBridge->length = length;
    newBridge->queueNorth = malloc(newBridge->queueSize*sizeof(PATH));
    newBridge->queueSouth = malloc(newBridge->queueSize*sizeof(PATH));
    newBridge->exitNorth = malloc(newBridge->queueSize*sizeof(PATH));
    newBridge->exitSouth = malloc(newBridge->queueSize*sizeof(PATH));
    newBridge->pass = malloc(newBridge->length*sizeof(PATH));   
    newBridge->yield = 1;
    newBridge->countAliens = 0;
    newBridge->full = 0;
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
        newBridge->queueSouth[i].x = init_posx + 40;
        newBridge->exitNorth[i].x = init_posx;
        newBridge->exitSouth[i].x = init_posx  + 40;

        // POSITION Y AXIS

        newBridge->queueNorth[i].y = INIT_POSY + 40*i;
        newBridge->queueSouth[i].y = END_POSY - 40*i;
        newBridge->exitNorth[i].y = (END_POSY-40*(queueSizeTemp-1))  + 40*i;
        newBridge->exitSouth[i].y = (INIT_POSY+40*(queueSizeTemp-1)) - 40*i;

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
    
    // SETTING ONE WAY BRIDGE
    int posxOff = init_posx + 20;
    int posyOff = INIT_POSY + 40 * newBridge->queueSize;
    if (length%2!=0)
        posyOff+=20;
    
    for (int i = 0; i < newBridge->length; i++)
    {
        newBridge->pass[i].x = posxOff;
        newBridge->pass[i].y = posyOff + 40*i;
        // newBridge->pass[i].image = passImage;
        newBridge->pass[i].blocked = 0;    
        newBridge->pass[i].height = 40;
        newBridge->pass[i].width = 40;    
    } 
    
    *ctxBridge = newBridge;
}

// BRIDGE* load_bridge(enum bridgePosition pos)
// {
//     if (pos == west){
// 		return get_config_aux(WEST_BRIDGE_CONF_PATH);
// 	} else if (pos == mid){
// 		return get_config_aux(MID_BRIDGE_CONF_PATH);
// 	} else if (pos == east){
// 		return get_config_aux(EAST_BRIDGE_CONF_PATH);
// 	} else {
// 		exit(EXIT_FAILURE);
// 	}
// }
