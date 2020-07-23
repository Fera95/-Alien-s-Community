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
    newAlien->tickets = 20;
    newAlien->alienPriority = (rand()%10) + 1;
    newAlien->weight = (rand()%5)+1;

    return newAlien;
}

 

void ADD_ALIEN (struct NODE_ALIEN **head, struct ALIEN *newData)
{
    NODE_ALIEN *newNode = (NODE_ALIEN *) malloc(sizeof(NODE_ALIEN*));
    newNode->data = newData;
    newNode->next = NULL;
    if(*head != NULL)
    {   
        NODE_ALIEN *temp = *head;
        while ( temp->next != NULL )
            temp = temp ->next;
        temp->next = newNode;
    
    }
    else{
        *head = newNode;
    }
    
}

void KILL_ALIEN(struct ALIEN *deadAlien)
{
  if(deadAlien != NULL)
  {
    ROUTE *deadWay = deadAlien->way;
    if(deadWay->current != deadWay->bridge->pass)
    {
      if(deadWay->start == alfaPlanet){
         REMOVE_ALIEN(deadWay->bridge->northHead, deadAlien->id);
      }
      else if(deadWay->start == betaPlanet)
      {
         REMOVE_ALIEN(deadWay->bridge->southHead, deadAlien->id);
      }
      deadWay->current[deadWay->pos].blocked = 0;
      deadWay->current[deadWay->pos].alienID = -1;
      deadWay->finished = 1;    
    }
    else
    {
      printf("ERROR :THE ALIEN ID: %d CAN NOT BE KILLED DURING HIS TIME IN THE PROCESSOR\n", deadAlien->id);
    }
  }
}


/**
 * DELETES AN ALIEN FROM MEMORY
 * CAN CAUSE SEGMENTATION FAULT 
 */
void DELETE_ALIEN ( struct NODE_ALIEN ** head, int idRemove)
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


/**
 * SAME AS DELETE BUT NOT FREEING THE MEMORY 
 * KEEP EXISTING FOR FURTHER REFERENCES
 */
void REMOVE_ALIEN ( struct NODE_ALIEN ** head, int idRemove)
{
  if(head != NULL && *head != NULL){
    NODE_ALIEN * temp = (*head);
    if((*head)->data->id == idRemove)
    {
      *head =  temp->next;
    }
    else
    {
      while (temp->next != NULL && temp->next->data->id != idRemove)
      {
        temp = temp->next;
      }
      if(temp->next != NULL)
      {
        NODE_ALIEN *temp2 = temp->next;
        temp->next = temp2->next;
      }
    }
  }
}


void PRINT_LIST(NODE_ALIEN *head)
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
    else if(property == 2)
      printf("%d ", current->data->alienPriority);
        
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



ALIEN *remove_by_id(NODE_ALIEN *head, int id) {
  int length = get_length(head);
  NODE_ALIEN * current = head;

 //temporales
  ALIEN * retval = NULL;
  NODE_ALIEN * temp = NULL;


  for(int i = 0; i < length; ++i){
    if(current->data->id == id) {
       temp = current->next;
       retval = temp->data;
       current->next = temp->next;
       free(temp);
       return retval;
    
    }
    else{
      current = current->next;
    }
   
  }
  printf("This id is not in the list\n");
  return NULL;
}


void swap_one_by_one(struct NODE_ALIEN *a, struct NODE_ALIEN *b) 
{ 
    struct ALIEN * temp = a->data; 
    a->data = b->data; 
    b->data = temp; 
} 


void order_list_by_priority(NODE_ALIEN *head){
  int swapped, i; 
  struct NODE_ALIEN *ptr1; 
  struct NODE_ALIEN *lptr = NULL; 
  
    /* Se verifica si la lista no es nula */
    if (head == NULL) 
        return; 
    do
    { 
        swapped = 0; 
        ptr1 = head; 
  
        while (ptr1->next != lptr) 
        { 
            //se ordena por propiedad 
            if (ptr1->data->alienPriority < ptr1->next->data->alienPriority) 
            {  
                swap_one_by_one(ptr1, ptr1->next); 
                swapped = 1; 
            } 
            ptr1 = ptr1->next; 
        } 
        lptr = ptr1; 
    } 
    while (swapped); 

}

NODE_ALIEN *order_list_by_lotery(NODE_ALIEN *head)
{

  //Se crea un nodo alien  para returno
  struct NODE_ALIEN * result = NULL; 

  int length = get_length(head);
  int iterations= 0;

  while(iterations != length)
  {

    //Se obtiene el largo de la lista original
    int length_temporal = get_length(head);
    int total_tickets = 0;


    //Se usar el largo para obtener tiquetes
    printf("length temp %d \n",length_temporal);

    for (int i = 0; i < length_temporal; i++)
    {
      struct ALIEN * temp = get_at(head, i);
      printf("ID temp %d \n",temp->id);

      int new_tickets= temp->tickets;
      total_tickets = total_tickets + new_tickets;
    }

    //se crea un array de largo de los tiquetes y un indice

    int array_lot[total_tickets];
    int lot_index = 0;

    //Se meten los ids al sorteo
    for (int i = 0; i < length_temporal; i++)
    {
      
      struct ALIEN * temp2 = get_at(head, i);
      int current_tickets = temp2->tickets;
      
      for ( int j = 0; j < current_tickets; j++)
      {
        //se agrega el id actual dependiendo de mi cantidad de tiquetes
        array_lot[lot_index] = temp2->id;
        lot_index ++;
      }
      

    }

    // una vez se tiene ya todo los tiquetes en la tombola 

    //random de 0 a total de tiquetes
    printf("random sin %d\n", (int) rand()%10);
    printf("total tickets %d\n",total_tickets);
    // se obtiene el id usando array_lot[resultado del random]
    int random = rand() % total_tickets;
    printf("random %d \n", random);

    int id_winner = array_lot[random];
    // se busca el alien ganador por 

    //Se crea un temporal para el alien ganador actual
    
    struct ALIEN * winner = get_by_id(head, id_winner);
    printf("WINNER ID: %d \n", winner->id);
    printf("TOTAL TICKETS: %d \n",total_tickets);


    //Se hace pushback del ganador en la lista de retorno

    push_back(&result, winner);
    printf("DESPUES DE PUSH BACK\n");
    // se elimina de la lista actual

    // remove_by_id(head,id_winner);
    DELETE_ALIEN(&head,winner->id);
    printf("DESPUES DE PUSH BACK\n");


    iterations ++;

  }

return result;


}