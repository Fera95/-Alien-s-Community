
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


			/*	Recursive-Mutex		*/
void            e_rec_mutex_init                (eRecMutex_t *rec_mutex);

void            e_rec_mutex_clear               (eRecMutex_t *rec_mutex);

void            e_rec_mutex_lock                (eRecMutex_t *rec_mutex);

ebool           e_rec_mutex_trylock             (eRecMutex_t *rec_mutex);

void            e_rec_mutex_unlock              (eRecMutex_t *rec_mutex);

			/*	ConditionVar	*/
void            e_cond_init                     (eCond_t *cond);

void            e_cond_clear                    (eCond_t *cond);

void            e_cond_wait                     (eCond_t *cond, lpthread_mutex *mutex);

void            e_cond_signal                   (eCond_t *cond);

void            e_cond_broadcast                (eCond_t *cond);

ebool           e_cond_wait_until               (eCond_t *cond, lpthread_mutex *mutex, eint64 end_time);


#endif /* EASY_MUTEX_POSIX_H_ */