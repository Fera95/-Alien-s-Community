#include "lpthread_mutex_posix.h"
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
#include <sys/syscall.h> // For call to gettid
#include <sys/queue.h>
#include <ucontext.h>
#include <sys/time.h>
#include <errno.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
							/*	Mutex	*/

void lpthread_mutex_init(lpthread_mutex *mutex) {

	mutex->i[0] = 0;
    mutex->i[2] =0; //se instancia el magic

}

void lpthread_mutex_destroy(lpthread_mutex *mutex) {


        mutex->i[2] =1;

    
}

static void __attribute__((noinline))
lpthread_mutex_lock_slowpath(lpthread_mutex *mutex) {

	while (exchange_acquire (&mutex->i[0], 2) != 0) {
		syscall(__NR_futex, &mutex->i[0], (esize) FUTEX_WAIT_PRIVATE, (esize) 2,
		NULL);
	}
}

static void __attribute__((noinline))
lpthread_mutex_unlock_slowpath(lpthread_mutex *mutex,
		euint prev) {

	if E_UNLIKELY (prev == 0) {
		fprintf (stderr, "Attempt to unlock mutex that was not locked\n");
		abort ();
	}

	syscall (__NR_futex, &mutex->i[0], (esize) FUTEX_WAKE_PRIVATE, (esize) 1, NULL);
}

void lpthread_mutex_lock(lpthread_mutex *mutex) {
    if (mutex->i[2] == 1){
     
       printf("Lock: Mutex no existente: Violación de segmento (`core' generado) \n");
       kill(getpid(),SIGINT);
    }

	/* 0 -> 1 and we're done.  Anything else, and we need to wait... */
	if E_UNLIKELY (e_atomic_int_add (&mutex->i[0], 1) != 0)
			lpthread_mutex_lock_slowpath (mutex);
}

void lpthread_mutex_unlock(lpthread_mutex *mutex) {
      if (mutex->i[2] == 1){
        
       printf("Unlock: Mutex no existente: Violación de segmento (`core' generado)  \n");
        kill(getpid(),SIGINT);
    }


	euint prev;
	prev = exchange_release(&mutex->i[0], 0);

	/* 1-> 0 and we're done.  Anything else and we need to signal... */
	if E_UNLIKELY (prev != 1)
		lpthread_mutex_unlock_slowpath (mutex, prev);
}

ebool lpthread_mutex_trylock(lpthread_mutex *mutex) {
    if (mutex->i[2] == 1){
        
       printf("Trylock: Mutex no existente: Violación de segmento (`core' generado)  \n");
       kill(getpid(),SIGINT);
    }


	euint zero = 0;
	/* We don't want to touch the value at all unless we can move it from
	 * exactly 0 to 1.
	 */
	return compare_exchange_acquire (&mutex->i[0], &zero, 1);
}