#include "bridge.h"
#include "alien.h"

void create_bridge (BRIDGE **ctxBridge, config_bridge bridgeConf,  enum bridgePosition position )
{
    int length = bridgeConf.length;
    int queueSize = bridgeConf.queueSize;
    int strength = bridgeConf.strength;
    int scheduler = bridgeConf.scheduler;
    BRIDGE * newBridge = NULL;
    if(length > 10 )
    {
        printf("ERROR: Bridge %d length is out of limits, please enter a value between 1 and 10\n",position);   
    }
    else if (length<0)
    {
        printf("ERROR: Bridge %d length is out of limits, please enter a value between 1 and 10\n",position);   
    }
    else if (strength < 1)
    {
        printf("ERROR: Bridge %d strength not allowed, please enter a value between 1 and %d\n",position,length);   
    }
    else if (scheduler < 0 || scheduler > 4)
    {
        printf("ERROR: Bridge %d scheduler not allowed, please enter a value between 0 and 4\n",position);   
    }
    
    else
    {
        int maxQueSize = (int) (TOTAL_LENGTH - length)/2;
        if(queueSize > maxQueSize)
        {
            printf("WARNING: Bridge %d  Queue Size is out of limits, value set to: %d\n",position,maxQueSize);   
            queueSize=maxQueSize;
        }
        else if (queueSize<1)
        {
            printf("WARNING: Bridge %d  Queue Size is out of limits, value set to: %d\n",position,1);   
            queueSize = 1;
        }
        newBridge = malloc(sizeof(BRIDGE));
        newBridge->queueSize = queueSize;
        newBridge->length = length;
        newBridge->position = position;
        newBridge->strength = strength;
        newBridge->scheduler = (enum scheduler_method) scheduler;
        newBridge->yield = 1;
        newBridge->countAliens = 0;
        newBridge->holdup = 0;
        newBridge->crossTime = length * 40 * 0.025;

        newBridge->queueNorth = malloc(newBridge->queueSize*sizeof(PATH));
        newBridge->queueSouth = malloc(newBridge->queueSize*sizeof(PATH));
        newBridge->exitNorth = malloc(newBridge->queueSize*sizeof(PATH));
        newBridge->exitSouth = malloc(newBridge->queueSize*sizeof(PATH));
        newBridge->pass = malloc(newBridge->length*sizeof(PATH));   
        newBridge->southHead = NULL;
        newBridge->northHead = NULL;

        // CAMBIAR POR UN ENUMERATE
        int init_posx;
        if (position == east)
            init_posx = EAST_POSX; 

        else if (position == mid)
            init_posx = MID_POSX;

        else if(position == west)
            init_posx = WEST_POSX; 
        
            
        for (int i = 0; i < queueSize; i++)
        {
            create_path(&(newBridge->queueNorth[i]), init_posx, INIT_POSY + 40*i, 40, 40);
            create_path(&(newBridge->queueSouth[i]), init_posx + 40, END_POSY - 40*i, 40, 40);
            create_path(&(newBridge->exitNorth[i]), init_posx, (END_POSY-40*(queueSize-1))  + 40*i, 40, 40);
            create_path(&(newBridge->exitSouth[i]), init_posx  + 40, (INIT_POSY+40*(queueSize-1)) - 40*i, 40, 40);
        }
        
        // SETTING ONE WAY BRIDGE
        int posxOff = init_posx + 20;
        int space2bridge = 12 - (newBridge->queueSize * 2);
        int blankSpace = (space2bridge - length)/2;
        int posyOff = INIT_POSY +(newBridge->queueSize * 40) + (40 * blankSpace);
        if (length%2!=0)
            posyOff+=20;
        
        for (int i = 0; i < newBridge->length; i++)
        {
            newBridge->pass[i].x = posxOff;
            newBridge->pass[i].y = posyOff + 40*i;
            newBridge->pass[i].blocked = 0;    
            newBridge->pass[i].height = 40;
            newBridge->pass[i].width = 40;    
        } 
        
    }
    *ctxBridge = newBridge;
}


void print_bridge(BRIDGE * bridge2print)
{
    
    int lenPass = bridge2print->length;
    int lenQueue = bridge2print->queueSize;
    printf("POS: %d\nYIELD: %d\nFULL: %d\nBRIDGE SIZE: %d\nQUEUE LEN: %d\nSTRENGH: %d",bridge2print->position,bridge2print->yield,bridge2print->holdup, lenPass, lenQueue, bridge2print->strength);
    // POSICIon, largo
    printf("NORTH PATH:\n");
    for (int i = 0; i < lenQueue; i++)
    {
        printf("QUEUENORTH[%d] BLOCKED: %d IDALIEN: %d\n",i, bridge2print->queueNorth[i].blocked, bridge2print->queueNorth[i].alienID);        
    }
    
    
    printf("BRIDGE PATH:\n");
    for (int i = 0; i < lenPass; i++)
    {
        printf("BRIDGE[%d] BLOCKED: %d IDALIEN: %d\n",i, bridge2print->pass[i].blocked, bridge2print->pass[i].alienID);        
    }
    
    
    printf("SOUTH PATH:\n");
    for (int i = 0; i < lenQueue; i++)
    {
        printf("QUEUESOUTH[%d] BLOCKED: %d IDALIEN: %d\n",i, bridge2print->queueSouth[i].blocked, bridge2print->queueSouth[i].alienID);        
    }
}


config_bridge get_config_aux(const char* conf_path){
    config_bridge conf;
    FILE* file = fopen(conf_path, "r");
    if(file==NULL){
        printf ("Path to config not avaliable. \n");
        exit(EXIT_FAILURE); 
    }
    char line[256];
    char prev[256];
    while (fgets(line, sizeof(line), file)) {
        char* current = strtok (line, "=:");
        //prev={0};
        while (current) {
            if(!strcmp(prev,"length")){
                conf.length = atoi(current);
            }
            else if(!strcmp(prev,"strength")){
                conf.strength = atoi(current);
            }
            else if(!strcmp(prev,"queueSize")){
                conf.queueSize = atoi(current);
            }            
			else if(!strcmp(prev,"scheduler")){
                conf.scheduler = atoi(current);
            }
            else if(!strcmp(prev,"planner")){
                conf.planner = atoi(current);
            }
            else if(!strcmp(prev,"planner_north_count")){
                conf.planner_north_count = atoi(current);
            }
            else if(!strcmp(prev,"planner_south_count")){
                conf.planner_south_count = atoi(current);
            }
            else if(!strcmp(prev,"planner_time")){
                conf.planner_time = atoi(current);
            }
            strcpy(prev, current);
            current = strtok (NULL, "=:");
        }
    }
    fclose(file);
    return conf;
}

config_bridge load_bridge(enum bridgePosition pos)
{
    if (pos == west){
		return get_config_aux(WEST_BRIDGE_CONF_PATH);
	} else if (pos == mid){
		return get_config_aux(MID_BRIDGE_CONF_PATH);
	} else if (pos == east){
		return get_config_aux(EAST_BRIDGE_CONF_PATH);
	} else {
		exit(EXIT_FAILURE);
	}
}


