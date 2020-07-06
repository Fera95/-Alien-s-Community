#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "lpthread/lpthread.h"
#include <stdlib.h>
#include <signal.h>
#ifndef LPTHREAD

#endif

#define NTHREADS 9
void *thread_function(void *);
lpthread_mutex_t mutex1;
int  counter = 0;

struct args {
   lpthread_t idB;
   int numero;
};

void *thread_function(void *numero)
{  
   int a = *((int *) numero);
   printf("Numero %d\n", a);
   free(numero);
}


void *crearhilo1(void *input){
 
 lpthread_t x = ((struct args*)input)->idB ;
 int num = ((struct args*)input)->numero;

 int *arg = malloc(sizeof(*arg));
        if ( arg == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }

        *arg = num;
 printf("Creador1 \n");
 lpthread_create( &x, NULL, thread_function, arg);
}


void *crearhilo2(void *input){
 
 lpthread_t x = ((struct args*)input)->idB ;
 int num = ((struct args*)input)->numero;

 int *arg = malloc(sizeof(*arg));
        if ( arg == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }

        *arg = num;
  printf("Creador2 \n");
 lpthread_create( &x, NULL, thread_function, arg);
}



void *crearhilo3(void *input){
 
 lpthread_t x = ((struct args*)input)->idB ;
 int num = ((struct args*)input)->numero;

 int *arg = malloc(sizeof(*arg));
        if ( arg == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }

        *arg = num;
  printf("Creador3 \n");
 lpthread_create( &x, NULL, thread_function, arg);
}



int main()
{
   
   int i, j;

   lpthread_t thread_id[NTHREADS];
   lpthread_t thread_id2[NTHREADS];


   for(i=0; i < NTHREADS; i++)
   {

       struct args *datos = (struct args *)malloc(sizeof(struct args));
   
   
         datos->idB =  thread_id2[i];
       datos->numero = i;
      

      

         if(i<=3){
            lpthread_create( &thread_id[i], NULL, crearhilo1, (void *)datos);
         }
         if(i>3 && i<=6){
            lpthread_create( &thread_id[i], NULL, crearhilo2, (void *)datos);
         }

         if(i>6){
            lpthread_create( &thread_id[i], NULL, crearhilo3, (void *)datos);
         }
         
      
   }

  
  
                                    

}

