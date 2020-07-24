#include "path.h"

void create_path(PATH * newPath, float x, float y, int width, int height)
{
    newPath->alienID = -1;
    newPath->x = x;
    newPath->y = y;
    newPath->width = width;
    newPath->height = height;
    newPath->blocked = 0;
    // if (sem_init(&(newPath->con_carrera), 0, 1) == -1)
    // {
    //     printf("ERROR creando el semáforo\n");
    // }
}


int lock_path(PATH *nextPath, int id)
{
    int lockResult = 0;
    // lpthread_mutex_lock(locktemp);
    // if(lpmutex_trylock(nextPath->lock))
    // if (sem_wait(&(nextPath->con_carrera)) == -1)
    // {
    //     printf("ERROR TRATANDO DE UTILIZAR EL SEMAFORO \n");
    // }
    nextPath->blocked = 1;
    nextPath->alienID = id;
    return lockResult;
}

void unlock_path(PATH * nextPath){
    nextPath->blocked = 0;
    nextPath->alienID = -1;
    // lpthread_mutex_unlock(locktemp);
    // lpthread_mutex_unlock(nextPath->lock);
    // sem_post(&(nextPath->con_carrera));
}