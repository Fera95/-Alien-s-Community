#ifndef ALIEN_HEADER_FILE
#define ALIEN_HEADER_FILE

#include "constants.h"
#include <stdio.h>
#include "time.h"

#define ALIEN_CONF_PATH  "./config/alien.conf"


alien_config load_alien();


ALIEN * create_alien (int ID, enum alienType type, ROUTE ** myWay, float firstX, float firstY, float baseSpeed, double rto_time);

void KILL_ALIEN(struct ALIEN *deadAlien);
void ADD_ALIEN (struct NODE_ALIEN **head, struct ALIEN *newData);
void REMOVE_ALIEN (struct NODE_ALIEN ** head, int id);
void DELETE_ALIEN (struct NODE_ALIEN ** head, int id);
void PRINT_LIST(NODE_ALIEN *head);


void print_list2(NODE_ALIEN * head, int property);
void push_back(NODE_ALIEN ** head, ALIEN *value);
void push_front(NODE_ALIEN ** head, ALIEN *value);

//returns the id of the popped element
ALIEN * pop_front(NODE_ALIEN **head, int delete);
ALIEN * pop_back(NODE_ALIEN * head);
ALIEN * remove_at(NODE_ALIEN **head, int index);

//returns the package at a position
ALIEN * get_at(NODE_ALIEN *head, int index);

//returns the position of a package
ALIEN * get_by_id(NODE_ALIEN *head, int id);


//sets the package at a position
ALIEN * set_at(NODE_ALIEN *head, int index, ALIEN * value);

//swaps the content of two nodes
void swap(NODE_ALIEN *head, int index1, int index2);

//returns the length of the list
int get_length(NODE_ALIEN * head);

//removes alien by id
ALIEN *remove_by_id(NODE_ALIEN *head, int id);

//swaps the content of two nodes one by one
void swap_one_by_one(struct NODE_ALIEN *a, struct NODE_ALIEN *b);

#endif