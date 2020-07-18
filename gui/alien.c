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
            printf("data ID:%d",temp->data->id);
            countID++;
        }
        else {
            printf("data NULL");
        }
            
        countNode++;
    }

    
}



//Funciones de prueba

//Revibe la cabeza de la lista e imprime todas las propiedades de los aliens en la lista
void print_list2(NODE_ALIEN *head, int property) {
  NODE_ALIEN * current = head;
  printf("[");
  while(current != NULL){
    if(property == 0)
      printf("%d ", current->data->id);
    else if(property == 1)
      printf("%d ", current->data->type);
    current = current->next;
  }
  printf("]\n");
}


//La idea es agregar in alien al final de la lista

void push_back(NODE_ALIEN **head, ALIEN * data) {
  //si la lista que se paso esta vacia
  NODE_ALIEN * current = *head;
  if (current == NULL){
    push_front(head, data);
    return;
  }
  //go to end of list
  while(current->next != NULL){
    current = current->next;
  }
  //add new element
  current->next = malloc(sizeof(NODE_ALIEN));
  current->next->data =  data;
  current->next->next = NULL;
}

//agregar en la cabeza

void push_front(NODE_ALIEN **head, ALIEN * data) {
  NODE_ALIEN * new_node;
  //allocates memory for new node
  new_node = malloc(sizeof(NODE_ALIEN));
  new_node->data =  data;
  new_node->next = *head;
  //assigns new head
  *head = new_node;
}

// quitar de la cabeza 
ALIEN * pop_front(NODE_ALIEN **head, int delete) {
  ALIEN * retval = NULL;
  NODE_ALIEN * new_head = NULL;

  //if list is empty
  if(*head == NULL){
    return retval;
  }

  new_head = (*head)->next;
  retval = (*head)->data;
  if(delete){
    free(*head);
  }
  *head = new_head;

  return retval;
}


//remover de la cola
ALIEN* pop_back(NODE_ALIEN *head) {
  ALIEN * retval = NULL;

  if(get_length(head) == 0){
    printf("List is already empty. Cannot pop-back.\n");
    return retval;
  }

  //Si solo hay un elemento en la lista 
  if(head->next == NULL){
    retval = head->data;
    free(head);
    return retval;
  }

  //Ir al penultimo nodo
  NODE_ALIEN * current = head;
  while(current->next->next != NULL){
    current = current->next;
  }

  retval = current->next->data;
  free(current->next);
  current->next = NULL;
  return retval;
}

//elimnar en un index especifico
ALIEN *remove_at(NODE_ALIEN **head, int index) {
  if(get_length(*head) <= index || index < 0){
    printf("No se puede eliminar , indice fuera del rango \n");
    return NULL;
  }
  int i = 0;
  ALIEN * retval = NULL;

  NODE_ALIEN * current = *head;
  NODE_ALIEN * temp = NULL;

  //primer elemento
  if(index == 0){
    return pop_front(head,1);
  }

  for(i = 0; i < index - 1; ++i){
    //the index is out of range
    if(current->next == NULL){
      printf("Indice fuera del rango ");
      return NULL;
    }
    current = current->next;
  }

  temp = current->next;
  retval = temp->data;
  current->next = temp->next;
  free(temp);

  return retval;
}

int get_length(NODE_ALIEN *head) {
  NODE_ALIEN * current = head;
  int retval = 0;
  while(current != NULL){
    current = current->next;
    retval ++;
  }
  return retval;
}

ALIEN* get_at(NODE_ALIEN *head, int index) {
  ALIEN * retval = NULL;

  if(get_length(head) <= index || index < 0){
    printf("Index out of range. Cannot get at %d.\n", index);
    return retval;
  }

  NODE_ALIEN * current = head;

  for(int i = 0; i < index; ++i){
    current = current->next;
  }
  retval = current->data;

  return retval;
}

ALIEN *set_at(NODE_ALIEN *head,  int index, ALIEN *data) {
  ALIEN * retval = NULL;

  if(get_length(head) <= index || index < 0){
    printf("Index out of range. Cannot set at %d.\n", index);
    return retval;
  }

  NODE_ALIEN * current = head;

  for(int i = 0; i < index; ++i){
    current = current->next;
  }
  current->data = data;
  retval = current->data;

  return retval;
}

void swap(NODE_ALIEN *head, int index1, int index2) {
  int length = get_length(head);
  if(index1 >= length || index1 < 0 || index2 >= length || index2 < 0){
    printf("Index out of range. Cannot swap %d and %d.\n", index1, index2);
    return;
  }

  ALIEN * temp;
  temp = get_at(head, index1);
  set_at(head, index1, get_at(head,index2));
  set_at(head,index2, temp);

}

ALIEN *get_by_id(NODE_ALIEN *head, int id) {
  int length = get_length(head);
  NODE_ALIEN * current = head;

  for(int i = 0; i < length; ++i){
    if(current->data->id == id) return current->data;
    current = current->next;
  }
  printf("This id is not in the list\n");
  return NULL;
}
