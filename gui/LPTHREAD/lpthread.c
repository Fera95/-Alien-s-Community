
#define _GNU_SOURCE

#include <unistd.h>
#include <sys/types.h>
#include "lpthread.h"

#ifndef gettid
// equivalent to:  pid_t  gettid(void)
#define gettid() syscall(SYS_gettid)
#endif

// List of threads
static lpthread_t lpthreadList[maxThread];
// Required pids
static pid_t parent_pid;
static pid_t group_pid;
// Counter for threads
static int numLPthreads = 0;

static char started_ = 0;

void init_threads(){
	// Initialices
	for (int i = 0; i < maxThread; ++ i){
		lpthreadList[i].pid = 0;
		lpthreadList[i].stack = 0;
	}
	// Signal from terminal
	signal(SIGINT, lpthread_end);
	atexit(lpthread_end); // When parent ends
	// Sets the required pids
	group_pid = getpgrp();
	parent_pid = getpid();

}


int lpthread_create(lpthread_t* thread, const lpthread_attr_t *attr, void *(*start_routine)(void*), void* arg)
{
    if(started_ == 0){ // Singleton for the system
		init_threads();
		started_=1;
	}
    
    /*Allocate the stack*/
	void * child_stack = malloc(threadStack);
	posix_memalign(&child_stack,8,threadStack);
	thread->stack = child_stack;
	if(thread->stack==0){
		printf("Error: Could not allocate stack.\n");
		// return LF_MALLOCERROR;
	}
    thread->pid = clone((int (*)(void *))start_routine, (char*) (thread->stack + threadStack),
						SIGCHLD|
						// CLONE_FS|
						// CLONE_FILES|
						CLONE_SIGHAND|
						CLONE_VM|
						CLONE_PARENT_SETTID|
						CLONE_CHILD_CLEARTID|
						CLONE_PTRACE, arg);
	if(thread->pid==-1){ // Error in clone
		free(thread->stack);
		printf("Error: clone system call failed.\n");
		return thread_cloneError;
	}
	// Copies thread to list
	thread->detached=0;
	memcpy((void*)&lpthreadList[numLPthreads++], (void*)thread, sizeof(lpthread_t));
    // printf("Contador de ids %d\n",numLPthreads);
    return thread_noError;
}


void lpthread_end(){
	// Kills the thread
	killpg(getpgrp(), SIGKILL);
}


void lpthread_exit(lpthread_t thread){
	kill(thread.pid, SIGKILL);
	// return 0;
}

int lpthread_yield(){
	/* Call the sched_yield system call which moves the current process to the
	end of the process queue. */
	sched_yield();
	return 0;
}

int map_pid_index(pid_t id){
	// Search for that pid
	for(int i = 0; i < maxThread; ++i){
		if(lpthreadList[i].pid == id){
			return i;
		}
	}
	return -1;
}

int lpthread_join(lpthread_t thread, void **retval){
	int index = map_pid_index(thread.pid);

	if(lpthreadList[index].detached==0){
		waitpid(thread.pid, 0, 0); // Key is here, wait for it to end
		printf("%s\n", "done join");

		return 0;
	}
	else{
		return 1;
	}
}


//Mutex functions


