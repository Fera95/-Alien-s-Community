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

lpthread_mutex_t mutex1;
int  counter = 0;



void *thread_function(void *dummyPtr)
{
   lpthread_mutex_lock( &mutex1 );
   printf("Soy la funcion  lockie el mutex \n");
   lpthread_mutex_unlock(&mutex1 );
   lpthread_mutex_destroy(&mutex1);
}


int main()
{
  lpthread_t thread1, thread2;

   lpthread_mutex_init(&mutex1,NULL);
   lpthread_create( &thread1, NULL, thread_function, NULL );
   lpthread_create( &thread2, NULL, thread_function, NULL );

//Join
   //lpthread_join( thread_id[0], NULL); 
   //lpthread_join( thread_id[1], NULL); 

   
  
                                          
}

