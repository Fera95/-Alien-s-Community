#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "lpthread.h"
#include <stdlib.h>
#include <signal.h>
#ifndef LPTHREAD

#endif


#define NTHREADS 10
void *thread_function(void *);
lpthread_mutex_t mutex1;
int  counter = 0;

int main()
{
   lpthread_t thread_id[NTHREADS];
   int i, j;

   for(i=0; i < NTHREADS; i++)
   {
      lpthread_create( &thread_id[i], NULL, thread_function, NULL );
   }

   for(j=0; j < NTHREADS; j++)
   {
      lpthread_join( thread_id[j], NULL); 
   }
  
                                                 

   printf("Final counter value: %d\n", counter);
}

void *thread_function(void *dummyPtr)
{
 
   lpthread_mutex_lock( &mutex1 );
   lpthread_self();
   counter++;
   lpthread_mutex_unlock( &mutex1 );
}