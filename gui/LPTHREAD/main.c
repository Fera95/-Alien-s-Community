#include "lpthread.h"
#include "lpthread_mutex_posix.h"
#include <stdio.h>


lpthread_mutex test;


void * moveAlien (void * alien);

void * moveAlien (void * alien)
{
    int * id = (int*) alien;
    
    
    //Se bloquea mutex
    //lpthread_mutex_lock(&test);
 
    while (1)
    {
        printf("CHILD THREAD %d\n",*id);
        sleep(3);
    }
    //Se desbloquea mutex
 
    //lpthread_mutex_lock(&test);
    //Se destruye mutex
    //lpthread_mutex_destroy(&mut);
}


int main () {
    
    //Se crea un mutex 

    //lpthread_mutex_init(&test);
  
    
    
    for (int i = 0; i < maxThread+1; i++)
    {
        lpthread_t t1;
        int a = i;
        lpthread_create(&t1, NULL, moveAlien, (void *)&a);
    }
    
    while (1)
    {
        printf("MAIN THREAD\n");
        sleep(1);
    }
    
    return 0;
}