#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "lpthread.h"
#include <stdlib.h>
#include <signal.h>
#ifndef LPTHREAD

#endif


#define NTHREADS  2
void *thread_function1(void *);
void *thread_function2(void *);
void *thread_function3(void *);
void *thread_function4(void *);


lpthread_mutex_t mutex1;
int  counter = 0;
int INITIALIZED = 0;


//Se inializan las funciones
void initialize_threads(){

  lpthread_t t_gui;
  if(lpthread_create(&t_gui, NULL, thread_function2, NULL) != 0) printf("\nCould not created Thread GUI\n");
  lpthread_t t_pause;
  if(lpthread_create(&t_pause, NULL, thread_function2, NULL) != 0) printf("\nCould not created Thread Pause\n");
  lpthread_t t_pause_hardware;
  if(lpthread_create(&t_pause_hardware, NULL, thread_function3, NULL) != 0) printf("\nCould not created Thread Pause Hardware\n");
  lpthread_t t_id_0;
  if(lpthread_create(&t_id_0, NULL, thread_function4, NULL) != 0) printf("\nCould not created Thread 0\n");



  lpthread_join(t_id_0, NULL);
  lpthread_join(t_pause_hardware, NULL);
  lpthread_join(t_gui, NULL);
}


void *thread_function1(void *dummyPtr)
{
  
   printf("Soy la funcion 1 lockie el mutex \n");

   //lpmutex_destroy(&mutex1);
}
void *thread_function2(void *dummyPtr)
{

   printf("Soy la funcion 2 lockie el mutex \n");
}

void *thread_function3(void *dummyPtr)
{
   
   printf("Soy la funcion 3 lockie el mutex \n");
   
}

void *thread_function4(void *dummyPtr)
{
   
   printf("Soy la funcion 4 lockie el mutex \n");
   
}


int main()
{
   lpthread_t thread1;
   lpthread_mutex_t   mutex1;
    
    lpmutex_init(&mutex1,NULL);
   //initialize_system(); //donde se leeria todos los configs
   if(lpthread_create(&thread1, NULL, thread_function1, NULL))
    printf("\nCould not created Thread Package Generation\n");
  //Wait for initial package generation

  initialize_threads();
  lpthread_join(thread1, NULL);
  return 0;

   
  
   

   
  
                                          
}