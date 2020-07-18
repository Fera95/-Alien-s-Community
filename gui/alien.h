#ifndef ALIEN_HEADER_FILE
#define ALIEN_HEADER_FILE

#include "constants.h"
#include <stdio.h>


ALIEN * create_alien (int ID, enum alienType type, ROUTE ** myWay, float firstX, float firstY, float baseSpeed);

void ADD_Alien (struct NODE_ALIEN * head, struct ALIEN *newData);
void REMOVE_Alien (struct NODE_ALIEN ** head, int id);
void printList(NODE_ALIEN *head);


void print_list2(NODE_ALIEN * head, int property);
void push_back(NODE_ALIEN ** head, ALIEN *value);
void push_front(NODE_ALIEN ** head, ALIEN *value);

//returns the id of the popped element
ALIEN * pop_front(NODE_ALIEN **head);
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


#endif