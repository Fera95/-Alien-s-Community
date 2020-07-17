#ifndef ALIEN_HEADER_FILE
#define ALIEN_HEADER_FILE

#include "constants.h"
#include <stdio.h>


ALIEN * create_alien (int ID, enum alienType type, ROUTE ** myWay, float firstX, float firstY, float baseSpeed);

void ADD_Alien (struct NODE_ALIEN * head, struct ALIEN *newData);
void REMOVE_Alien (struct NODE_ALIEN ** head, int id);
void printList(NODE_ALIEN *head);

#endif