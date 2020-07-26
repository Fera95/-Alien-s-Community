#include "scheduler.h"

/**
 * LIST SORT OPERATIONS
 * SCHEDULER
 */

void schedule_list (NODE_ALIEN **head, int option)
{
    switch (option)
    {
    case Priority:
        order_list_by_priority(*head);
        break;
    case Lottery:
        *head = order_list_by_lotery(*head);
        break;
    case ShortestFirst:
        order_list_by_shortest(*head);
        break;
    case RoundRobin:
        order_list_by_rr(*head);
        break;
    case FIFO:
        break;
    default:
        break;
    }
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

void order_list_by_shortest(NODE_ALIEN *head)
{
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
            if (ptr1->data->dy < ptr1->next->data->dy) 
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

void order_list_by_rr(NODE_ALIEN *head){}

