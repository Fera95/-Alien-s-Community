#include "lpthread.h"


#define NUM_THREADS 4
#define NUM_LOCKS 1
#define STACK_SIZE 16384
#define TIME_SLICE 100000
#define THRESHOLD 1000000
#define CHECK_FREQUENCY 10
#define VECLEN 20000000

static scheduler *sched;
static int sharedVariable = 0;
static int sharedVariable1 = 0;
long beginning;
long finish;
static long int thr_id = 0;
static long int check_flag = 0;
static lpthread_t *thr_list;
static lpthread_mutex_t *mutex1;

typedef struct
{
    double      *a;
    double      *b;
    double     sum;
    int     veclen;
} DOTDATA;

DOTDATA dotstr;
lpthread_t callThd[NUM_THREADS];
lpthread_mutex_t mutexsum;





// methods of queue
void queueInit(queue *first)
{
    first->tail = NULL;
    first->size = 0;
    first->head = NULL;
}

lpthread_t *peek(queue *first)
{
    return first->head;
}

char queue_isEmpty(queue *first)
{
    return first->size == 0;
}

void enqueue(queue *first, lpthread_t *thr_node)
{
    if (first->size == 0)
    {
        first->head = thr_node;
        first->tail = thr_node;
        first->size++;
    }
    else
    {
        first->tail->next_thr = thr_node;
        first->tail = thr_node;
        first->size++;
    }
}
lpthread_t *dequeue(queue *first)
{
    if (first->size == 0)
    {
        printf("Empty Queue\n");
        return NULL;
    }
    lpthread_t *tmp;
    if (first->size == 1)
    {
        tmp = first->head;
        first->head = NULL;
        first->tail = NULL;
    }
    else
    {
        tmp = first->head;
        first->head = first->head->next_thr;
    }
    tmp->next_thr = NULL;
    first->size--;
    return tmp;
}




// methods of scheduler
void scheduler_handler()
{
    struct itimerval tick;
    ucontext_t sched_ctx;

    if (check_flag++ >= CHECK_FREQUENCY)
    {
        int i;
        check_flag = 0;
        long int current_time = timeStamp();
        for (i = 1; i < NUM_LEVELS; i++)
        {
            if (sched->running_queue[i].head != NULL)
            {
                lpthread_t *tmp = sched->running_queue[i].head;
                lpthread_t *parent = NULL;
                while (!tmp)
                {
                    if (current_time - tmp->last_exe_tt >= THRESHOLD)
                    {
                        if (parent == NULL)
                        {
                            sched->running_queue[i].head = tmp->next_thr;
                        }
                        else
                        {
                            parent->next_thr = tmp->next_thr;
                        }
                        schedAddThread(tmp, 0);
                    }
                    else
                    {
                        parent = tmp;
                    }
                    tmp = tmp->next_thr;
                }
            }
        }
    }
    lpthread_t* tmp = sched->thr_cur;
    if (tmp != NULL) {
        int old_priority = tmp->priority;
        tmp->time_runs += TIME_SLICE;
        if (tmp->time_runs >= sched->prior_list[old_priority] || tmp->thr_state == YIELD || tmp->thr_state == TERMINATED || tmp->thr_state == WAITING)
        {
            if (tmp->thr_state == YIELD) {
                schedAddThread(tmp, tmp->priority);
            }
            else {
                //put the thread back into the queue with the lower priority
                int new_priority = (tmp->priority + 1) > (NUM_LEVELS - 1) ? (NUM_LEVELS - 1) : (tmp->priority + 1);
                schedAddThread(tmp, new_priority);
            }
            //pick another thread out and run
            if ((sched->thr_cur = schedPickThread()) != NULL) {
                sched->thr_cur->thr_state = RUNNING;
            }
        }
    }
    else {
        //pick another thread out and run
        if ((sched->thr_cur = schedPickThread()) != NULL)
        {
            sched->thr_cur->thr_state = RUNNING;
        }
    }

    //set timer
    tick.it_value.tv_sec = 0;
    tick.it_value.tv_usec = 50000;
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = 0;

            setitimer(ITIMER_REAL, &tick, NULL);


        if (sched->thr_cur != NULL) {
            if (sched->thr_cur->first_exe_tt == 0) {
                sched->thr_cur->first_exe_tt = timeStamp();
            }
            sched->thr_cur->last_exe_tt = timeStamp();
            if (tmp != NULL)
        {
            swapcontext(&(tmp->ucp), &(sched->thr_cur->ucp));
        }
        else
        {
            swapcontext(&sched_ctx, &(sched->thr_cur->ucp));
        }

    }
}


long timeStamp()
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    return current_time.tv_usec + current_time.tv_sec * 1000000;
}
void schedInit()
{
    sched = malloc(sizeof(scheduler));
    sched->running_queue = malloc(NUM_LEVELS * sizeof(queue));
    sched->waiting_queue = malloc(NUM_LOCKS * sizeof(queue));
    sched->thr_main = (lpthread_t *)calloc(1, sizeof(lpthread_t));
    int a = 0;
    int b = 0;
    int c = 0;
    for (a = 0; a < NUM_LEVELS; a++)
    {
        queueInit((a + sched->running_queue));
    }
    for (b = 0; b < NUM_LOCKS; b++)
    {
        queueInit((b + sched->waiting_queue));
    }
    for (c = 0; c < NUM_LEVELS; c++)
    {
        sched->prior_list[c] = TIME_SLICE * (c + 1);
    }

    sched->thr_main->next_thr = sched->thr_main;
    sched->thr_cur = NULL;
    sched->num_sched = 0;
    sched->thr_main->thr_id = 0;
    sched->thr_main->thr_state = NEW;

    signal(SIGALRM, scheduler_handler); // when timer expired, transfer control to scheduler_handler
}

void schedAddThread(lpthread_t *thr_node, int priority)
{
    if (priority < 0 || priority >= NUM_LEVELS)
    {
        printf("Corrupted priority.\n");
    }
    else
    {
        thr_node->thr_state = READY;
        thr_node->priority = priority;
        thr_node->time_runs = 0;
        enqueue(&(sched->running_queue[priority]), thr_node);
        sched->num_sched++;
    }
}
lpthread_t *schedPickThread()
{
    int i;
    for (i = 0; i < NUM_LEVELS; i++) {
        if (sched->running_queue[i].head != NULL) {
            lpthread_t *chosen = dequeue(&(sched->running_queue[i]));
            printf("Scheduled a level %d thread: %ld\n", i, chosen->thr_id);
            sched->num_sched--;
            return chosen;
        }
    }

    return NULL;
}
void runThread(lpthread_t *thr_node, void *(*f)(void *), void *arg)
{
    finish = timeStamp();
    printf("time elapsed: %ld\n", finish - beginning);
    thr_node->thr_state = RUNNING;

    sched->thr_cur = thr_node;
    thr_node->retval = f(arg);
    if (thr_node->thr_state != TERMINATED) {
        thr_node->thr_state = TERMINATED;
    }
    if (sched->thr_cur != NULL) {
        sched->thr_cur->end_tt = timeStamp();
    }
    scheduler_handler();
}



// Follwings are methoeds of thread

// Initializes a lpthread_mutex_t created by the calling thread. Attributes are ignored.
int lpthread_mutex_init(lpthread_mutex_t *mutex, const pthread_attr_t *mutexattr)
{
    if (mutex == NULL)
    {
        return EINVAL;
    }
    mutex->flag = 0;
    mutex->guard = 0;
    mutex->wait = malloc(sizeof(queue));
    queueInit(mutex->wait);
    return 0;
}


// Locks a given mutex, other threads attempting to access this mutex will not run until it is unlocked.
int lpthread_mutex_lock(lpthread_mutex_t *mutex)
{
    while (__sync_lock_test_and_set(&(mutex->flag), 1))
    {
        sched->thr_cur->thr_state = WAITING;
        printf("The thread is waiting for a mutex, put it to the waiting list\n");
        enqueue(mutex->wait, sched->thr_cur);
        scheduler_handler();
    }
}

// Unlocks a given mutex.
int lpthread_mutex_unlock(lpthread_mutex_t *mutex)
{
    lpthread_t * chosen;
    if (mutex->wait->head != NULL)
    {
        chosen = dequeue(mutex->wait);
        printf("Mutex is available, select one thread from the waiting list and put it back to the running queue\n");
        schedAddThread(chosen, chosen->priority);
    }
    mutex->flag = 0;
}

// Destroys a given mutex. Mutex should be unlocked before doing so.
int lpthread_mutex_destroy(lpthread_mutex_t *mutex)
{
    int result = 0;

    if (mutex == NULL)
        return EINVAL;
    if (mutex->flag != 0)
        return EBUSY;
    return result;
}

// Creates a pthread that executes function. Attributes are ignored.
int lpthread_create(lpthread_t *thread, lpthread_attr_t *attr, void *(*function)(void *), void *arg)
{
    if (getcontext(&(thread->ucp)) != 0)
    {
        printf("getcontext error\n");
        return -1;
    }

    thread->ucp.uc_stack.ss_sp = malloc(STACK_SIZE);
    thread->ucp.uc_stack.ss_size = STACK_SIZE;
    thread->thr_id = thr_id++;
    thread->start_tt = timeStamp();
    thread->first_exe_tt = 0;
    makecontext(&(thread->ucp), (void *)runThread, 3, thread, function, arg);
    schedAddThread(thread, 0);
    printf("Thread %ld successfully created\n", thr_id);
    return 0;
}

// Explicit call to the lpthread_t scheduler requesting that the current context be swapped out and another be scheduled.
void lpthread_yield()
{
    lpthread_t * tmp;
    tmp = sched->thr_cur;

    scheduler_handler();

    int new_priority = (tmp->priority + 1)>NUM_LEVELS ? NUM_LEVELS : (tmp->priority + 1);
    schedAddThread(tmp, new_priority);

    sched->thr_cur = schedPickThread();

    sched->thr_cur->thr_state = RUNNING;
    swapcontext(&(tmp->ucp), &(sched->thr_cur->ucp));
}

// Explicit call to the lpthread_t library to end the pthread that called it. If the value_ptr isn't NULL, any return value from the thread will be saved.
void lpthread_exit(void *value_ptr)
{
    if (sched->thr_cur->thr_state == TERMINATED)
    {
        printf("This thread has already exited.\n");
    }
    sched->thr_cur->thr_state = TERMINATED;
    sched->thr_cur->retval = value_ptr;
    sched->thr_cur->end_tt = timeStamp();
    scheduler_handler();
}

// Call to the lpthread_t library ensuring that the calling thread will not execute until the one it references exits. If value_ptr is not null, the return value of the exiting thread will be passed back.
int lpthread_join(lpthread_t *thread, void **value_ptr)
{
    while (thread->thr_state != TERMINATED)
    {
        lpthread_yield();
    }
    thread->retval = value_ptr;
}



