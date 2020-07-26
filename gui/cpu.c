#include "cpu.h"

void *cpu_north_ready_queue(void * arg)
{
    BRIDGE ** bridge =(BRIDGE**) arg;
    while ( bridge!=NULL )
    {
        NODE_ALIEN *head = (NODE_ALIEN*) (*bridge)->northHead;
        if (head != NULL)
        {       
            lpthread_mutex_lock(&(*bridge)->yield_semaphore);
            NODE_ALIEN * temp = head;
            if((*bridge)->yield == northYield ) //&& !(*bridge)->waiting)
            {
                // printf("PUENTE %d YIELD: %d\n", (*bridge)->position, (*bridge)->yield);
                if( temp!=NULL)
                {
                    // printf("PUENTE %d LEN NORTE: %d\n", (*bridge)->position, get_length((*bridge)->northHead));
                    int nextHoldup = temp->data->weight + (*bridge)->holdup;
                    if(nextHoldup <= (*bridge)->strength && (*bridge)->yield == northYield)
                    {
                        if (temp->data->status == ready)
                        {
                            temp->data->status = running;
                        } 
                    }
                }
                else {
                    temp = head;
                }
            }
            lpthread_mutex_unlock(&(*bridge)->yield_semaphore); 
        }
    }   
}

void *cpu_south_ready_queue(void * arg)
{
    BRIDGE ** bridge =(BRIDGE**) arg;
    while ( bridge!=NULL )
    {       
        NODE_ALIEN *head = (NODE_ALIEN*) (*bridge)->southHead;
        if (head != NULL)
        {   
            lpthread_mutex_lock(&(*bridge)->yield_semaphore); 
            NODE_ALIEN * temp = head;
            if((*bridge)->yield == southYield )//&& !(*bridge)->waiting)
            {
                if( temp!=NULL){
                    int nextHoldup = temp->data->weight + (*bridge)->holdup;
                    if(nextHoldup <= (*bridge)->strength && (*bridge)->yield == southYield){
                        if (temp->data->status == ready){
                            if((*bridge)->planner == Count){
                                int nextCout = (*bridge)->tempCount + 1;
                                if (nextCout <= (*bridge)->planner_count){
                                    if(!(*bridge)->waiting){
                                        temp->data->status = running;
                                    }
                                }
                                else
                                {
                                    printf("count al limite\n");
                                }
                                
                            }
                            else{
                                temp->data->status = running;
                            }
                        }
                    }
                }
                else{
                    temp = head;
                }
            }
            lpthread_mutex_unlock(&(*bridge)->yield_semaphore); 
        }
    }   
}


void * cpu_runnig (void *arg)
{
    BRIDGE** bridge =  (BRIDGE**) arg;
    while (bridge != NULL)
    {
        if( (*bridge)->crossing != NULL) {
            NODE_ALIEN * temp = (*bridge)->crossing;
            int newHoldUp = 0;
            while (temp != NULL)
            {
                newHoldUp += (temp->data)->weight;
                temp = temp->next;
            }
            (*bridge)->holdup = newHoldUp;
            printf("BRIDGE %d NEW HOLDUP %d SET %d\n",(*bridge)->position, newHoldUp, (*bridge)->holdup);
        }
        else {
            (*bridge)->holdup = 0;
        }
        
    }
    

}

void cpu ( BRIDGE ** bridge, int cardinal )
{
    lpthread_t t03, t04;
    if(cardinal){
        lpthread_create(&t03, NULL, cpu_north_ready_queue, (void *)bridge);
    }
    else
    {
        lpthread_create(&t03, NULL, cpu_south_ready_queue, (void *)bridge);
    }
    // lpthread_create(&t04, NULL, cpu_runnig, (void *)bridge);

}



/**
 * 
 */
void * rutineSurvive(void *arg){
    BRIDGE ** bridge = (BRIDGE**) arg;
    (*bridge)->yield = northYield;

    while ( bridge != NULL )
    {
        if(get_length((*bridge)->crossing) == 0 && (*bridge)->holdup == 0){
            if(get_length((*bridge)->northHead) > 0 && get_length((*bridge)->southHead) == 0){
                if((*bridge)->yield != northYield){
                    (*bridge)->yield = northYield;
                    // (*bridge)->waiting = 1;
                }
            }
            else if(get_length((*bridge)->southHead) > 0 && get_length((*bridge)->northHead) == 0){
                if((*bridge)->yield != southYield){
                    (*bridge)->yield = southYield;
                    // (*bridge)->waiting = 1;
                }
            }
            else if (get_length((*bridge)->southHead) > 0 && get_length((*bridge)->northHead) > 0) {
                if((*bridge)->yield != northYield){
                    (*bridge)->yield = northYield;
                    // (*bridge)->waiting = 1;
                }
            }
        }
    }
}



void *rutineCount(void * arg)
{ 
    BRIDGE ** bridge = (BRIDGE**) arg;  
    while ((*bridge)!=NULL)
    {
        int change_yield = 0;
        NODE_ALIEN *queueNorth = (NODE_ALIEN *) (*bridge)->northHead;
        NODE_ALIEN *queueSouth = (NODE_ALIEN *) (*bridge)->southHead;
        if((*bridge)->tempCount >= (*bridge)->planner_count)
        {
            (*bridge)->tempCount = 0;
            if((*bridge)->yield == northYield){
                (*bridge)->yield = southYield;
                change_yield = 1;
            }
            else if((*bridge)->yield == southYield){
                (*bridge)->yield = northYield;
                change_yield = 1;
            }
        }
        else
        {
            int lenNorth = get_length(queueNorth);
            int lenSouth = get_length(queueSouth);
            if (lenNorth == 0 && lenSouth != 0){
                if((*bridge)->yield != southYield){
                    change_yield = 1;
                    (*bridge)->yield = southYield;
                }
            }
            else if (lenSouth == 0 && lenNorth != 0){
                if((*bridge)->yield != northYield){
                    change_yield = 1;
                    (*bridge)->yield = northYield;
                }
            }
            else {
                if( lenNorth < (*bridge)->planner_count && lenSouth < (*bridge)->planner_count){
                    if(lenNorth < lenSouth) {
                        if((*bridge)->yield != southYield){
                            change_yield = 1;
                            (*bridge)->yield = southYield;
                        }
                    }
                    else if(lenNorth > lenSouth) {
                        if((*bridge)->yield != northYield){
                            change_yield = 1;
                            (*bridge)->yield = northYield;
                        }
                    }
                }
            }
        }
        if(change_yield){
            if(get_length((*bridge)->crossing) != 0){
                (*bridge)->waiting = 1;
            }
        }
        usleep(12500);
    }  
}

void rutineSemaphore(void *bridge){

}
void planning ( BRIDGE **bridge)
{
    enum algorithm plan = (*bridge)->planner;
    lpthread_t t02;
    // lpthread_create(&t02, NULL, rutineSurvive, (void *)bridge);
    lpthread_create(&t02, NULL, rutineCount, (void *)bridge);

    // switch (plan)
    // {
    // case Count:
    //     rutineCount((void *)bridge);
    //     break;
    // case Semaphore:
    //     rutineSemaphore((void *)bridge);
    //     break;
    // case Survive:
    //     lpthread_create(&t02, NULL, rutineSurvive, (void *)bridge);
    //     break;
    // default:
    //     break;
    // }
}