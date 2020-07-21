#include "lpthread.h"
#include <ucontext.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <signal.h>
#include <ucontext.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define NUM_THREADS 4
#define NUM_LOCKS 1
#define STACK_SIZE 16384
#define TIME_SLICE 100000
#define THRESHOLD 1000000
#define CHECK_FREQUENCY 10
#define VECLEN 20000000


typedef struct
{
    double      *a;
    double      *b;
    double     sum;
    int     veclen;
} DOTDATA;


static scheduler *sched;
static int sharedVariable = 0;
static int sharedVariable1 = 0;
long beginning;
long finish;
static long int thr_id = 0;
static long int check_flag = 0;
static lpthread_t *thr_list;
static lpthread_mutex_t *mutex1;
DOTDATA dotstr;
lpthread_t callThd[NUM_THREADS];
lpthread_mutex_t mutexsum;

void *dotprod(void *arg)
{

    /* Define and use local variables for convenience */
    int i, start, end, len;
    long offset;
    double mysum, *x, *y;
    offset = (long)arg;
    len = dotstr.veclen;
    start = offset * len;
    end = start + len;
    x = dotstr.a;
    y = dotstr.b;

    /*
    Perform the dot product and assign result
    to the appropriate variable in the structure.
    */
    mysum = 0;
    for (i = start; i<end; i++)
    {
        mysum += (x[i] * y[i]);
    }

    /*
    Lock a mutex prior to updating the value in the shared
    structure, and unlock it upon updating.
    */
    lpthread_mutex_lock(&mutexsum);
    dotstr.sum += mysum;
    printf("Thread %ld did %d to %d:  mysum=%f global sum=%f\n", offset, start, end, mysum, dotstr.sum);
    lpthread_mutex_unlock(&mutexsum);
    lpthread_exit((void*)0);
}



int main()
{
    thr_list = malloc(NUM_THREADS * sizeof(lpthread_t));
    if (thr_list)
    {
        printf("Threads' space allocated\n");
    }

    mutex1 = malloc(sizeof(lpthread_mutex_t));
    if (mutex1)
    {
        lpthread_mutex_init(mutex1, NULL);
        printf("Mutex initialized\n");
    }

    long random[NUM_THREADS];
    long random_sec[NUM_THREADS];
    schedInit();
    lpthread_attr_t *thread_attr = NULL;
    thr_list = malloc(NUM_THREADS * sizeof(lpthread_t));


    long i;
    double *a, *b;
    void *status;
    lpthread_attr_t attr;


    a = (double*)malloc(NUM_THREADS*VECLEN * sizeof(double));
    b = (double*)malloc(NUM_THREADS*VECLEN * sizeof(double));
    beginning = timeStamp();
    for (i = 0; i<VECLEN*NUM_THREADS; i++)
    {
        a[i] = 1;
        b[i] = a[i];
    }

    dotstr.veclen = VECLEN;
    dotstr.a = a;
    dotstr.b = b;
    dotstr.sum = 0;

    lpthread_mutex_init(&mutexsum, NULL);

    /* Create threads to perform the dotproduct  */

    for (i = 0; i<NUM_THREADS; i++)
    {
        /* Each thread works on a different set of data.
        *  The offset is specified by 'i'. The size of
        *  the data for each thread is indicated by VECLEN.*/
        lpthread_create(&callThd[i], &attr, dotprod, (void *)i);
    }
    /* Wait on the other threads */
    for (i = 0; i<NUM_THREADS; i++)
    {
        lpthread_join(&callThd[i], &status);
    }
    /* After joining, print out the results and cleanup */

    printf("Sum =  %f \n", dotstr.sum);
    finish = timeStamp();
    printf("time elapsed: %ld\n", finish - beginning);

    free(a);
    free(b);
    lpthread_mutex_destroy(&mutexsum);
    lpthread_exit(NULL);

    return 0;
}

