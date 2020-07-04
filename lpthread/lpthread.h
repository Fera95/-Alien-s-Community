#ifndef LPTHREAD
#define LPTHREAD


/* include lib header files that you need here: */
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <malloc.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include <time.h>
#include "tcb.h"
#include "list.h"
#include "lottery.h"
#include "selfishrr.h"
#include "edf.h"


//Status
#define READY 0
#define YIELD 1
#define WAIT 2
#define EXIT 3
#define JOIN 4
#define MUTEX_WAIT 5
#define DETACH 6


//Scheduling codes
#define RR 0
#define SELFISH_RR 1
#define LOTTERY 2
#define RT_EDF 3

//Quantum

#define MICROSEC_PER_MILISEC 1000
#define MICROSEC_PER_SEC 1000000
#define INTERVAL_SEC 0
#define INTERVAL_MILISEC 1
#define INTERVAL_MICROSEC  INTERVAL_MILISEC * 1000
#define INTERVAL (INTERVAL_SEC * MICROSEC_PER_SEC) + INTERVAL_MICROSEC // HALF second
#define TICK INTERVAL
#define QUANTUM 1

typedef struct sigaction mySig;

// TOOLS ----------------
void garbage_collection();
void initializeMainContext();
void initializeGarbageContext();

// SCHEDULING
void scheduler(int signum);
void getNextThread();
void lpthread_setsched(int);

// DEBUG ----------------
void lpthread_print_queues();
pid_t  lpthread_self();


// THREAD ----------------
int lpthread_create(lpthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg);
// Initialize pthread attr
int lpthread_attr_init (lpthread_attr_t* attr);
// Set Lottery tickets
int lpthread_attr_set_tickets(lpthread_attr_t* attr, unsigned int tickets);
int lpthread_attr_set_rt_params(lpthread_attr_t* attr, unsigned int period, unsigned int duration);
/* give CPU pocession to other user level threads voluntarily */
int lpthread_yield();
/* terminate a thread */
void lpthread_exit(void *value_ptr);
/* detach a thread */
int pthread_detach(lpthread_t thread);
/* wait for thread termination */
int lpthread_join(lpthread_t thread, void **value_ptr);



// MUTEX  ---------------------


/* initial the mutex lock */
int lpthread_mutex_init(lpthread_mutex_t *mutex, const lpthread_mutexattr_t *mutexattr);
/* aquire the mutex lock */
int lpthread_mutex_lock(lpthread_mutex_t *mutex);
/* try aquire the mutex lock */
int lpthread_mutex_trylock(lpthread_mutex_t *mutex);
/* release the mutex lock */
int lpthread_mutex_unlock(lpthread_mutex_t *mutex);
/* destroy the mutex */
int lpthread_mutex_destroy(lpthread_mutex_t *mutex);

void reset_timer();

// FIX sleep
void my_sleep(unsigned long time);



#define pthread_create(thread, attr, function, arg) lpthread_create(thread, attr, function, arg)
#define pthread_attr_init(attr) lpthread_attr_init(attr)
#define pthread_yield() lpthread_yield()
#define pthread_exit(value_ptr) lpthread_exit(value_ptr)
#define pthread_join(thread, value_ptr) lpthread_join(thread, value_ptr)
#define pthread_detach(thread) lpthread_detach(thread)
#define pthread_mutex_init(mutex, mutexattr) lpthread_mutex_init(mutex, mutexattr)
#define pthread_mutex_lock(mutex) lpthread_mutex_lock(mutex)
#define pthread_mutex_trylock(mutex) lpthread_mutex_trylock(mutex)
#define pthread_mutex_unlock(mutex) lpthread_mutex_unlock(mutex)
#define pthread_mutex_destroy(mutex) lpthread_mutex_destroy(mutex)



// Variables
long long ticks;
tcb *currentThread, *prevThread;
head_t runningQueue;
head_t newQueue; // SelfishRR
head_t allThreads;
head_t tickets;
ucontext_t cleanup;
sigset_t signal_set;
mySig sig;

struct itimerval timer, currentTime;

int mainContextInitialized;
int timeElapsed;
int threadCount;
int operationInProgress;

int schedulingAlgorithm;

#endif
