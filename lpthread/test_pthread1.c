#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
//#include <pthread.h>
#include <unistd.h>



#include "lpthread.h"
#ifndef LPTHREAD

#endif



void *print_message_function( void *ptr );

int main()
{
     pthread_t thread1, thread2,thread3, thread4;
     char *message1 = "Hilo 1";
     char *message2 = "Hilo 2";
     char *message3 = "Hilo 3";
     char *message4 = "Hilo 4";

     int  iret1, iret2, iret3, iret4;

    /* Create independent threads each of which will execute function */
     iret2 = lpthread_create( &thread2, NULL, print_message_function, (void*) message2);
     iret1 = lpthread_create( &thread1, NULL, print_message_function, (void*) message1);
     iret4 = lpthread_create( &thread4, NULL, print_message_function, (void*) message4);

     iret3 = lpthread_create( &thread3, NULL, print_message_function, (void*) message3);
     
     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */

     //lpthread_join( thread1, NULL);
     //lpthread_join( thread4, NULL);
     //lpthread_join( thread3, NULL);
     //lpthread_join( thread2, NULL);
  

     printf("Thread 1 returns: %d\n",iret1);
     printf("Thread 2 returns: %d\n",iret2);
     printf("Thread 3 returns: %d\n",iret3);
     printf("Thread 4 returns: %d\n",iret4);
     exit(0);
}

void *print_message_function( void *ptr )
{
     char *message;
     message = (char *) ptr;
     printf("%s \n", message);
}