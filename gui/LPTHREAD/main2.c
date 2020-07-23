#include "lpthread.h"
#include "lpthread_mutex_posix.h"
#include <stdio.h>


lpthread_mutex test;


void * moveAlien1 (void * alien);
void * moveAlien2 (void * alien);

 int counter = 0;
void * moveAlien1 (void * alien)
{
    int * id = (int*) alien;
    
    
    //Se bloquea mutex
   lpthread_mutex_lock(&test);
 
 
    
    
    printf("CHILD THREAD 1 %d\n",*id);
    for(int i=0; i<10; i++){
        counter = counter + 1;
        printf("Sumando %d \n",counter);
    }
    
    //Se desbloquea mutex
 
    lpthread_mutex_unlock(&test);
    //Se destruye mutex
   //lpthread_mutex_destroy(&test);
}


void * moveAlien2 (void * alien)
{
    int * id = (int*) alien;
    
    
    //Se bloquea mutex
    lpthread_mutex_lock(&test);
 
   
        printf("CHILD THREAD 2 %d\n",*id);
        for(int i=0; i<10; i++){
        counter = counter + 1;
        printf("Sumando %d \n",counter);
        }
    
    //Se desbloquea mutex
 
    lpthread_mutex_unlock(&test);
    //Se destruye mutex
    //lpthread_mutex_destroy(&test);
}




int main () {
    
 //Se crea un mutex 

    lpthread_mutex_init(&test);
  
   
    
    
        lpthread_t t1,t2;
        int a1 = 1;
        int a2 = 1;
        lpthread_create(&t1, NULL, moveAlien1, (void *)&a1);
        lpthread_create(&t2, NULL, moveAlien2, (void *)&a2);
    
       // lpthread_join(t1,NULL);
       // lpthread_join(t2,NULL);
      
    sleep(1);
     printf("%d",counter);
  
    return 0;
}