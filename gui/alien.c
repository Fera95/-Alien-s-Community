#include "alien.h"

ALIEN * create_alien(int ID, enum alienType type, ROUTE ** myWay, float firstX, float firstY, float baseSpeed)
{
    // printf("Creating ALIEN\n");
    // printf("TIPO RECIBIDO: %d\n", type);
    ALIEN * newAlien = (ALIEN*) malloc(sizeof(ALIEN));
    newAlien->type = type;
    newAlien->id = ID;
    if(type == normal){
        // printf("TIPO: Normal\n");

        newAlien->dx = baseSpeed;
        newAlien->dy = baseSpeed;
        // newAlien->image = al_load_bitmap("assets/normal25.png");
    }
    else if(type == alfa){
        // printf("TIPO: Alfa\n");

        float speed = baseSpeed;
        speed += baseSpeed * 0.2;
        newAlien->dx = speed;
        newAlien->dy = speed;
        // newAlien->image = al_load_bitmap("assets/alfa25.png");
    }
    else if(type == beta){
        // printf("TIPO: Beta\n");

        // CAMBIAR A VALOR RANDOM
        float speed = baseSpeed + baseSpeed * 2;
        newAlien->dx = speed;
        newAlien->dy = speed;
        // newAlien->image = al_load_bitmap("assets/beta25.png");
    }
    // printf("Asigning route\n");
    newAlien->way = *myWay;
    newAlien->x = firstX;
    newAlien->y = firstY;

    return newAlien;
}

 

void ADD_Alien (struct NODE_ALIEN * head, struct ALIEN *newData)
{
    NODE_ALIEN *newNode = (NODE_ALIEN *) malloc(sizeof(NODE_ALIEN*));
    newNode->data = newData;
    newNode->next = NULL;
    if(head != NULL)
    {   
        NODE_ALIEN *temp = head;
        while ( temp->next != NULL )
            temp = temp ->next;
        temp->next = newNode;
    
    }
    else
        head = newNode;
    
}

void REMOVE_Alien ( struct NODE_ALIEN ** head, int idRemove)
{
    NODE_ALIEN * temp = (*head);

    if((*head)->data->id == idRemove)
    {
        *head =  temp->next;
        free(temp);
    }
    else
    {
        while (temp->next != NULL && temp->next->data->id != idRemove)
            temp = temp->next;
        if(temp->next != NULL)
        {
            NODE_ALIEN *temp2 = temp->next;
            temp->next = temp2->next;
            free(temp2);
        }
        
    }
}


void printList(NODE_ALIEN *head)
{
    NODE_ALIEN *temp = head;
    int countNode = 0;
    int countID = 0;
    while (temp != NULL)
    {
        if(head->data != NULL){
            // printf("data ID:%d",temp->data->id);
            countID++;
        }
        else {
            printf("data NULL");
        }
            
        countNode++;
    }

    
}
