#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "lpthread/lpthread.h"
#include <stdlib.h>
#include <signal.h>
#ifndef LPTHREAD

#endif

#define NUM_THREADS	5

void *PrintHello(void *threadid)
{
   long tid;
   tid = (long)threadid;
   printf("Hello World! It's me, thread #%ld!\n", tid);
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
   lpthread_t threads[NUM_THREADS];
   int rc;
   long t;
   for(t=0;t<NUM_THREADS;t++){
     printf("In main: creating thread %ld\n", t);
     rc = lpthread_create(&threads[t], NULL, PrintHello, (void *)t);
     if (rc){
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
       }
     }

   /* Last thing that main() should do */
   lpthread_exit(NULL);
}
