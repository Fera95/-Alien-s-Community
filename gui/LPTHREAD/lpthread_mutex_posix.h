
#ifndef LPTHREAD_MUTEX_POSIX_H_
#define LPTHREAD_MUTEX_POSIX_H_

#include "lpthread_mutex.h"
#include "lpthread_atomic.h"
			/*	Mutex	*/
void            lpthread_mutex_init                    (lpthread_mutex *mutex);

void            lpthread_mutex_destroy                   (lpthread_mutex *mutex);

void            lpthread_mutex_lock                    (lpthread_mutex *mutex);

ebool           lpthread_mutex_trylock                 (lpthread_mutex *mutex);

void            lpthread_mutex_unlock                  (lpthread_mutex*mutex);





#endif /* EASY_MUTEX_POSIX_H_ */