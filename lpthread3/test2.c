#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "lpthread.h"
#include <stdlib.h>
#include <signal.h>
#ifndef LPTHREAD

#endif


#define NUM_THREADS 4
#define NUM_LOCKS 1
#define STACK_SIZE 16384
#define TIME_SLICE 100000
#define THRESHOLD 1000000
#define CHECK_FREQUENCY 10
#define VECLEN 20000000000
//variables necesarias
static scheduler *sched;
static int sharedVariable = 0;
static int sharedVariable1 = 0;
long beginning;
long finish;
static long int thr_id = 0;
static long int check_flag = 0;
static lpthread_t *thr_list;
static lpthread_mutex_t *mutex1;
lpthread_t callThd[NUM_THREADS];
lpthread_mutex_t mutextest;


//FUNCIONES 

void *thread_function1(void *);
void *thread_function2(void *);
void *thread_function3(void *);
void *thread_function4(void *);




//Se inializan las funciones TESTEAR 

void initialize_threads(){
lpthread_t t_gui;
  if(lpthread_create(&t_gui, NULL, thread_function2, NULL) != 0) printf("\nCould not created Thread GUI\n");
  lpthread_t t_pause;
  if(lpthread_create(&t_pause, NULL, thread_function2, NULL) != 0) printf("\nCould not created Thread Pause\n");
  lpthread_t t_pause_hardware;
  if(lpthread_create(&t_pause_hardware, NULL, thread_function3, NULL) != 0) printf("\nCould not created Thread Pause Hardware\n");
  lpthread_t t_id_0;
  if(lpthread_create(&t_id_0, NULL, thread_function4, NULL) != 0) printf("\nCould not created Thread 0\n");



  lpthread_join(&t_id_0, NULL);
  lpthread_join(&t_pause_hardware, NULL);
  lpthread_join(&t_gui, NULL);
}


void *thread_function1(void *dummyPtr)
{
   lpthread_mutex_lock(&mutextest);
   printf("Soy la funcion 1 lockie el mutex \n");
   lpthread_mutex_unlock(&mutextest);

   lpthread_mutex_destroy(&mutextest);
}
void *thread_function2(void *dummyPtr)
{

   lpthread_mutex_lock(&mutextest);
   printf("Soy la funcion 2 lockie el mutex \n");
   lpthread_mutex_unlock(&mutextest);

}

void *thread_function3(void *dummyPtr)
{
    lpthread_mutex_lock(&mutextest);
   printf("Soy la funcion 3 lockie el mutex \n");
   lpthread_mutex_unlock(&mutextest);

   
}

void *thread_function4(void *dummyPtr)
{
    lpthread_mutex_lock(&mutextest);
   printf("Soy la funcion 4 lockie el mutex \n");
   lpthread_mutex_unlock(&mutextest);

   
}


int main()
{
    thr_list = malloc(NUM_THREADS * sizeof(lpthread_t));
    if (thr_list)
    {
        printf("Se pide espacio de los threads\n");
    }

   // se inicializa Mutex


    schedInit();
    lpthread_attr_t *thread_attr = NULL;
    //Se pide memoria para la lista de threads
   


    lpthread_mutex_init(&mutextest, NULL);

    /*se crean los threads */

    
       
    lpthread_create(&callThd[3], NULL, thread_function4,NULL);
    lpthread_create(&callThd[2], NULL, thread_function3,NULL);
    lpthread_create(&callThd[1], NULL, thread_function1,NULL);
    lpthread_create(&callThd[0], NULL, thread_function2,NULL);

    /* se hace join  */

    lpthread_join(&callThd[3], NULL);
    lpthread_join(&callThd[2], NULL);
    lpthread_join(&callThd[1], NULL);
    lpthread_join(&callThd[0], NULL);
    
  
    //lpthread_mutex_destroy(&mutextest);
    lpthread_exit(NULL);

    return 0;

}