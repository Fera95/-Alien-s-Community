#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "lpthread.h"
#include <stdlib.h>
#include <signal.h>
#ifndef LPTHREAD

#endif

void *functionC();
lpthread_mutex_t mutex1;
int  counter = 0;

int main()
{
   int rc1, rc2;
   lpthread_t thread1, thread2;

   /* Create independent threads each of which will execute functionC */

   if( (rc1=lpthread_create( &thread1, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc1);
   }

   if( (rc2=lpthread_create( &thread2, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc2);
   }

   /* Wait till threads are complete before main continues. Unless we  */
   /* wait we run the risk of executing an exit which will terminate   */
   /* the process and all threads before the threads have completed.   */

   lpthread_join( thread1, NULL);
   lpthread_join( thread2, NULL); 

   exit(0);
}

void *functionC()
{
   lpthread_mutex_lock( &mutex1 );
   counter++;
   printf("Counter value: %d\n",counter);
   lpthread_mutex_unlock( &mutex1 );
}