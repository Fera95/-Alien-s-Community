#ifndef LPTHREAD 
#define LPTHREAD 

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include <sched.h> /* For clone */
#include <signal.h> /* For SIGCHLD */
#include <sys/types.h> /* For pid_t */
#include <sys/wait.h> /* For wait */
#include <unistd.h> /* For getpid */
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/syscall.h> // For call to gettid

#include <sys/queue.h>
#include <ucontext.h>
#include <sys/time.h>
#include <errno.h>



#define maxThread 1000
#define threadStack (1024*1024)
#define thread_noError 0
#define thread_maxThreads 1
#define thread_mallocError 2
#define thread_cloneError 3
#define thread_inThread 4
#define thread_signalError 5
/**
 * Estructura para la info individual del LPTHREAD
 */ 

typedef struct
{
    pid_t pid;       //ID retornado por el clone
    void* stack;
    char detached;
} lpthread_t;


typedef struct{
	int id;
} lpthread_attr_t;


/**
 * 
 */ 
int lpthread_create(lpthread_t* thread, const lpthread_attr_t *attr, void *(*start_routine)(void*), void* arg);

/**
 * 
 */ 
int lpthread_exit(lpthread_t thread);


void init_threads();

void lpthread_end();

#endif