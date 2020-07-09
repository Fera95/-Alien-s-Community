#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef LISTA_H
#define LISTA_H


typedef struct configespera {
    int esperaID;
    int esperaLargo;
    int esperaCalendarizador;
    int esperaQuantum;
} configespera_t;

typedef struct configpuente {
    int puenteID;
    int puenteTotalAliens;
    int puenteTotalAliens_turno;
    int puentePesoPermitido;
    int puenteLargo;
    int puenteTiempo_para_cruzar;
    int puenteAlgoritmo;
} configpuente_t;

typedef struct alien {
  int id;
  int tipo;
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
} alien_t;


typedef struct Node{
  alien_t * value;
  struct Node * next;
} Node_t;

//Imprime la info de las listas de aliens
void print_list(Node_t * head);

void push_back(Node_t ** head, alien_t *value);
void push_front(Node_t ** head, alien_t *value);

//devuelve el id del elemento emergente
alien_t * pop_front(Node_t **head);
alien_t * pop_back(Node_t * head);
alien_t * remove_at(Node_t **head, int index);

//devuelve el paquete en una posición
alien_t * get_at(Node_t *head, int index);

//returns the position of a package
alien_t * get_by_id(Node_t *head, int id);


//establece el paquete en una posición
alien_t * set_at(Node_t *head, int index, alien_t * value);

//intercambia el contenido de dos nodos
void swap(Node_t *head, int index1, int index2);

//devuelve la longitud de la lista
int get_length(Node_t * head);



#endif 