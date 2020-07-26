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
            if((*bridge)->yield == northYield)
            {
                // printf("PUENTE %d YIELD: %d\n", (*bridge)->position, (*bridge)->yield);
                if( temp!=NULL)
                {
                    // printf("PUENTE %d LEN NORTE: %d\n", (*bridge)->position, get_length((*bridge)->northHead));
                    int nextHoldup = temp->data->weight + (*bridge)->holdup;
                    if(nextHoldup <= (*bridge)->strength && (*bridge)->yield == northYield)
                    {
                        // printf("PUENTE %d NEXT HOLD: %d YIELD: %d\n", (*bridge)->position, nextHoldup, (*bridge)->yield);
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
            if((*bridge)->yield == southYield)
            {
                if( temp!=NULL)
                {
                    int nextHoldup = temp->data->weight + (*bridge)->holdup;
                    if(nextHoldup <= (*bridge)->strength && (*bridge)->yield == southYield){
                        if (temp->data->status == ready)
                        {
                            temp->data->status = running;
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
    (*bridge)->yield = waitYield;
    while ( bridge != NULL )
    {
        lpthread_mutex_lock(&(*bridge)->yield_semaphore); 
        if(get_length((*bridge)->crossing) == 0 && (*bridge)->holdup == 0){
            if(get_length((*bridge)->northHead) > 0 && get_length((*bridge)->southHead) == 0){
                (*bridge)->yield = northYield;
            }
            else if(get_length((*bridge)->southHead) > 0 && get_length((*bridge)->northHead) == 0){
                (*bridge)->yield = southYield;
                
            }
            else if(get_length((*bridge)->southHead) > 0 && get_length((*bridge)->northHead) > 0){
                NODE_ALIEN* northQueue = (NODE_ALIEN*) (*bridge)->northHead;
                NODE_ALIEN* southQueue = (NODE_ALIEN*) (*bridge)->southHead;
                
                if( northQueue->data->weight > (*bridge)->strength )
                {
                    (*bridge)->yield = northYield;
                }
                else if (southQueue->data->weight > (*bridge)->strength)
                {
                    (*bridge)->yield = southYield;
                }
                else
                {
                    (*bridge)->yield = northYield;
                }
            }
            else
            {
                (*bridge)->yield = northYield;
            }
        }
        lpthread_mutex_unlock(&(*bridge)->yield_semaphore); 
    }
}

void rutineCount(void *bridge){

}
void rutineSemaphore(void *bridge){

}
void planning ( BRIDGE **bridge)
{
    enum algorithm plan = (*bridge)->planner;
    lpthread_t t02;
    lpthread_create(&t02, NULL, rutineSurvive, (void *)bridge);

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