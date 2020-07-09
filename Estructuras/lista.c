#include "lista.h"

//Imprime los id's de los aliens

/*
typedef struct alien {
  int id;
  int tipo;  0 normal, 1 alfa, 2 beta , 3 intruso
  float peso;
  short prioridad;
  double velocidad;
  short espera;
  bool en_espera;
  double suma_tiempo_ejecucion;
  double tiempo_actual_ejecucion;
  double tiempo_tota_ejecucion;
  double tiempo_restante; //tiempo total de ejecucion - tiempo actual de ejecucion
  clock_t usage_time_start;
} 
*/

void print_list(Node_t *head) {
  Node_t * current = head;
  printf("[");
  while(current != NULL){
    printf("%d ,", current->value->id);
    printf("%d ,", current->value->tipo);
    printf("%f ,", current->value->peso);
    printf("%d ,", current->value->prioridad);
    printf("%d ,", current->value->velocidad);
    printf("%d ,", current->value->espera);
    printf("%d ,", current->value->en_espera);
    printf("%d ,", current->value->suma_tiempo_ejecucion);
    printf("%d ,", current->value->tiempo_actual_ejecucion);
    printf("%d ,", current->value->tiempo_tota_ejecucion);
    printf("%d ,", current->value->tiempo_restante);
  }
  printf("]\n");
}

void push_back(Node_t **head, alien_t * value) {
  //Si la lista está vacia
  Node_t * current = *head;
  if (current == NULL){
    push_front(head, value);
    return;
  }
  //go to end of list
  while(current->next != NULL){
    current = current->next;
  }
  //add new element
  current->next = malloc(sizeof(Node_t));
  current->next->value =  value;
  current->next->next = NULL;
}

void push_front(Node_t **head, alien_t * value) {
  Node_t * new_node;
  //allocates memory for new node
  new_node = malloc(sizeof(Node_t));
  new_node->value =  value;
  new_node->next = *head;
  //assigns new head
  *head = new_node;
}

alien_t * pop_front(Node_t **head) {
  alien_t * retval = NULL;
  Node_t * new_head = NULL;

  //Si la lista está vacia
  if(*head == NULL){
    return retval;
  }

  new_head = (*head)->next;
  retval = (*head)->value;
  free(*head);
  *head = new_head;

  return retval;
}

alien_t* pop_back(Node_t *head) {
  alien_t * retval = NULL;

  if(get_length(head) == 0){
    printf("La lista ya está vacía no se puede hacer pop.\n");
    return retval;
  }

  //Si solo hay un item en la lista.
  if(head->next == NULL){
    retval = head->value;
    free(head);
    return retval;
  }

  //Ir al penultimo nodo
  Node_t * current = head;
  while(current->next->next != NULL){
    current = current->next;
  }

  retval = current->next->value;
  free(current->next);
  current->next = NULL;
  return retval;
}

alien_t *remove_at(Node_t **head, int index) {
  if(get_length(*head) <= index || index < 0){
    printf("No se puede elimintar , index fuera de rango \n");
    return NULL;
  }
  int i = 0;
  alien_t * retval = NULL;

  Node_t * current = *head;
  Node_t * temp = NULL;

  //Primer elemento
  if(index == 0){
    return pop_front(head);
  }

  for(i = 0; i < index - 1; ++i){
    //index fuera del rango
    if(current->next == NULL){
      printf("index fuera de rango");
      return NULL;
    }
    current = current->next;
  }

  temp = current->next;
  retval = temp->value;
  current->next = temp->next;
  free(temp);

  return retval;
}

//devuelve el tamaño de las lista
int get_length(Node_t *head) {
  Node_t * current = head;
  int retval = 0;
  while(current != NULL){
    current = current->next;
    retval ++;
  }
  return retval;
}

alien_t* get_at(Node_t *head, int index) {
  alien_t * retval = NULL;

  if(get_length(head) <= index || index < 0){
    printf("Index out of range. Cannot get at %d.\n", index);
    return retval;
  }

  Node_t * current = head;

  for(int i = 0; i < index; ++i){
    current = current->next;
  }
  retval = current->value;

  return retval;
}

alien_t *set_at(Node_t *head,  int index, alien_t *value) {
  alien_t * retval = NULL;

  if(get_length(head) <= index || index < 0){
    printf("Index fuera de rango. No se puede setear %d.\n", index);
    return retval;
  }

  Node_t * current = head;

  for(int i = 0; i < index; ++i){
    current = current->next;
  }
  current->value = value;
  retval = current->value;

  return retval;
}

void swap(Node_t *head, int index1, int index2) {
  int length = get_length(head);
  if(index1 >= length || index1 < 0 || index2 >= length || index2 < 0){
    printf("Index fuera de rango. Se de puede hacer swap de %d y %d.\n", index1, index2);
    return;
  }

  alien_t * temp;
  temp = get_at(head, index1);
  set_at(head, index1, get_at(head,index2));
  set_at(head,index2, temp);

}

alien_t *get_by_id(Node_t *head, int id) {
  int length = get_length(head);
  Node_t * current = head;

  for(int i = 0; i < length; ++i){
    if(current->value->id == id) return current->value;
    current = current->next;
  }
  printf("El id no está en la lista\n");
  return NULL;
}