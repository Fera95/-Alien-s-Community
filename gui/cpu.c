#include "cpu.h"

void *cpu_north_ready_queue(void * arg)
{
    BRIDGE ** bridge =(BRIDGE**) arg;
    while ( bridge!=NULL )
    {
        NODE_ALIEN *head = (NODE_ALIEN*) (*bridge)->northHead;
        if (head != NULL)
        {       
            NODE_ALIEN * temp = head;
            if((*bridge)->yield == northYield)
            {
                if( temp!=NULL)
                {
                    int nextHoldup = temp->data->weight + (*bridge)->holdup;
                    if(temp->data->weight > (*bridge)->strength){
                        REMOVE_ALIEN(&head, temp->data->id);
                        push_back(&head, temp->data);
                    }
                    else if(nextHoldup <= (*bridge)->strength && (*bridge)->yield == northYield){
                        if (temp->data->status == ready)
                        {
                            temp->data->status = running;
                        }
                    }
                    temp = temp->next;
                }
                else{
                    temp = head;
                }
            }
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
            NODE_ALIEN * temp = head;
            if((*bridge)->yield == southYield)
            {
                if( temp!=NULL)
                {
                    int nextHoldup = temp->data->weight + (*bridge)->holdup;
                    if(temp->data->weight > (*bridge)->strength){
                        REMOVE_ALIEN(&head, temp->data->id);
                        push_back(&head, temp->data);
                    }
                    else if(nextHoldup <= (*bridge)->strength && (*bridge)->yield == southYield){
                        if (temp->data->status == ready)
                        {
                            temp->data->status = running;
                        }
                    }
                    temp = temp->next;
                }
                else{
                    temp = head;
                }
            }
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
    lpthread_create(&t04, NULL, cpu_runnig, (void *)bridge);

}



/**
 * 
 */
void * rutineSurvive(void *arg){
    BRIDGE ** bridge = (BRIDGE**) arg;
    (*bridge)->yield = waitYield;
    while ( bridge != NULL )
    {
        if(get_length((*bridge)->crossing) == 0 && (*bridge)->holdup == 0){
            if(get_length((*bridge)->northHead) > 0 && get_length((*bridge)->southHead) == 0){
                (*bridge)->yield = northYield;
            }
            else if(get_length((*bridge)->southHead) > 0 && get_length((*bridge)->northHead) == 0){
                (*bridge)->yield = southYield;
            }
            else
            {
                (*bridge)->yield = northYield;
            }
        }
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