#ifndef SCHEDULER_HEADER_FILE
#define SCHEDULER_HEADER_FILE

#include "constants.h"
#include "alien.h"

void schedule_list(NODE_ALIEN ** list, int option);


NODE_ALIEN * order_list_by_lotery(NODE_ALIEN *head);    //gets a loterry winner
void order_list_by_priority(NODE_ALIEN *head);          //orders list by priority 
void order_list_by_shortest(NODE_ALIEN *head);
void order_list_by_rr(NODE_ALIEN *head);
#endif