
#ifndef lpthread_mutex_H_
#define lpthread_mutex_H_

#include "easy_utils.h"

typedef union {
	/*	private	*/
	eptr p;
	euint i[2];
    euint magic;
} lpthread_mutex;

typedef struct {
	eptr p;
	euint i[2];
} eRwLock_t;

typedef struct {
	eptr p;
	euint i[2];
} eCond_t;

typedef struct {
	eptr p;
	euint i[2];
} eRecMutex_t;

#define E_PRIV_INIT(notify) { NULL, (notify), { NULL, NULL } }
typedef struct {
	eptr p;
	eDestroyNotify notif;
	eptr future[2];
} ePriv_t;

typedef enum {
	E_ONCE_NOTCALLED,
	E_ONCE_PROGRESS,
	E_ONCE_READY
} kEOnceStat;

#define E_ONCE_INIT { E_ONCE_NOTCALLED, NULL }
typedef struct {
	volatile kEOnceStat status;
	volatile eptr retval;
} eOnce_t;

/*	Using Kernel Hacks like macro pasting and indirect callables	*/
#define E_LOCK_NAME(name)				e__ ## name ## _lock
#define E_LOCK_DEFINE_STATIC(name)		static E_LOCK_DEFINE (name)
#define E_LOCK_DEFINE(name)				lpthread_mutex E_LOCK_NAME (name)
#define E_LOCK_EXTERN(name)				extern lpthread_mutex G_LOCK_NAME (name)

#define E_LOCK(name) 		lpthread_mutex_lock 		(&E_LOCK_NAME (name))
#define E_UNLOCK(name)		lpthread_mutex_unlock		(&E_LOCK_NAME (name))
#define E_TRYLOCK(name)		lpthread_mutex_trylock		(&E_LOCK_NAME (name))

#endif /* EASY_MUTEX_H_ */