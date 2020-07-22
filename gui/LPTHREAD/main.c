#include "lpthread.h"
#include <stdio.h>

void * moveAlien (void * alien);

void * moveAlien (void * alien)
{
    int * id = (int*) alien;
    while (1)
    {
        printf("CHILD THREAD %d\n",*id);
        sleep(3);
    }
}


int main () {
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