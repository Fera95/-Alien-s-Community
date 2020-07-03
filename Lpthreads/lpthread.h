#ifndef H_LPTHREAD
#define H_LPTHREAD

#ifndef _XOPEN_SOURCE		// address compilation issues on Mac OS
#define _XOPEN_SOURCE 600
#endif

#define MEM 64000

#include <stdlib.h>
#include <ucontext.h>
#include <stdio.h>
#include <errno.h>

static int tid = 0;			// incrementa por cada nuevo thread

// para trackea thread actuvo. 

static int currActive = -1;		


typedef enum {
	untouched,	// thread no se ha creado
	ready,		// thread se ha creado
	running,	// thread está activo (solo un thread está activo el mismo tiempo)
	blocked,	// thread está bloqueado esperando. . .
	zombie,		// thread se terminó de ejecutar
} state;

typedef struct {
	//  tipos
	int pid;	// id único
} lpthread_t;

typedef struct {
	// atributos
} lpthread_attr_t;

typedef struct {
	int pid;				// id unico
	state status;			// estado del thread
	ucontext_t lcontext;	// contecto asociado

	// so that if a thread is joinable, its calling thread is unblocked
	int wait_pid;			// waiting (if any) thread : -1 OR pid
	void * retval;			// puntero al valor de retorno

} lpthread_info_t;

// Funciones de los threads

/*

La función lpthread_create () creará un nuevo thread, con
atributos especificados por attr, dentro de un proceso.

*/


int lpthread_create(lpthread_t *thread, const lpthread_attr_t *attr,
			void *(*start_routine) (void *), void *arg);



/*
La función lpthread_exit () termina el thread de llamada y devuelve
un valor a través de retval que (si el hilo es joinable) está disponible para
otro hilo en el mismo proceso que llama a lpthread_join ()
*/

void lpthread_exit(void *retval);

/*

*/

int lpthread_yield(void);


/*
La función lpthread_join () suspenderá la ejecución de la llamada
hilo hasta que el hilo objetivo termine, a menos que el hilo objetivo tenga ya terminado.
*/
int lpthread_join(lpthread_t thread, void **retval);



/*
La función pthread_detach () indicará a la implementación que
       El almacenamiento para el subproceso de subproceso se puede recuperar cuando ese subproceso termina.
*/

int lpthread_detach(lpthread_t thread, void **retval);

//defición de los mutex


typedef int lpthread_mutex_t;
typedef int lpthread_mutexattr_t;

static inline int lpthread_mutex_init(lpthread_mutex_t *mutex,
			const lpthread_mutexattr_t *attr) { return 0; }

static inline int lpthread_mutex_destroy(lpthread_mutex_t *mutex) { return 0; }

static inline int lpthread_mutex_lock(lpthread_mutex_t *mutex) { return 0; }

static inline int lpthread_mutex_trylock(lpthread_mutex_t *mutex) { return 0; }

static inline int lpthread_mutex_unlock(lpthread_mutex_t *mutex) { return 0; }

#endif