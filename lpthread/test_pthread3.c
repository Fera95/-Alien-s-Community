#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


/*

void *print_message_function( void *ptr );

int main()
{
     pthread_t thread1, thread2,thread3, thread4;
     char *message1 = "Hilo 1";
     char *message2 = "Hilo 2";
     char *message3 = "Hilo 3";
     char *message4 = "Hilo 4";

     int  iret1, iret2, iret3, iret4;

    /* Create independent threads each of which will execute function 

     iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);
     iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);
     iret3 = pthread_create( &thread3, NULL, print_message_function, (void*) message3);
     iret4 = pthread_create( &thread4, NULL, print_message_function, (void*) message4);

     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   

     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL); 
     pthread_join( thread3, NULL);
     pthread_join( thread4, NULL);

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
*/



#define NTHREADS  2
int *thread_function1(void *);
int *thread_function2(void *);
//pthread_mutex_t mutex1;
int  counter = 0;



int *thread_function1(void *dummyPtr)
{
   printf("antes de mutex 1 \n");
   pthread_mutex_lock( NULL );
   printf("Soy la funcion 1 lockie el mutex \n");
   pthread_mutex_unlock( NULL );
}


int *thread_function2(void *dummyPtr)
{
   printf("antes de mutex 2\n");
   pthread_mutex_lock( NULL );
   printf("Soy la funcion 2 lockie el mutex \n");
   pthread_mutex_unlock( NULL );
}


int main()
{
   pthread_t thread_id[NTHREADS];

   //lpthread_mutex_init(&mutex1,NULL);
 
   pthread_create( &thread_id[0], NULL, (void *)thread_function1, NULL );
   pthread_create( &thread_id[1], NULL, (void *)thread_function2, NULL );

//Join
   pthread_join( thread_id[0], NULL); 
   pthread_join( thread_id[1], NULL); 

  
}

