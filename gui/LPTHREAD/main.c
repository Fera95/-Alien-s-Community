#include "lpthread.h"
#include <stdio.h>

void * moveAlien (void * alien);

void * moveAlien (void * alien)
{
    while (1)
    {
        printf("CHILD THREAD\n");
        sleep(3);
    }
}


int main () {
    lpthread_t t1;
    lpthread_create(&t1, NULL, moveAlien, NULL);
    while (1)
    {
        printf("MAIN THREAD\n");
        sleep(1);
    }
    
    return 0;
}