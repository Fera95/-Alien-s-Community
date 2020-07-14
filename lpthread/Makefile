CC=gcc

CFLAGS=-g -ggdb
ifeq (${THREAD_LIB}, PTHREAD)
CFLAGS += -lpthread
endif
ifeq (${THREAD_LIB}, lPTHREAD)
CFLAGS += -D lPTHREAD_LIB
endif

PTHREAD_DEPS=lpthread.c lpthread_mutex.c lpthread_debug.c list.c lottery.c selfishrr.c edf.c scheduling.c
PTHREAD_HEADERS=lpthread.h tcb.h list.h lottery.h selfishrr.h edf.h


THREAD_LIB=PTHREAD
SCHEDULER=LOTTERY

default:  test1 test2 test3 test4


test1: test_pthread1.c $(PTHREAD_DEPS) $(PTHREAD_HEADERS)
		$(CC) test_pthread1.c $(PTHREAD_DEPS) -o test1 $(CFLAGS)  -lpthread

test2: test_pthread2.c $(PTHREAD_DEPS) $(PTHREAD_HEADERS)
		$(CC) test_pthread2.c $(PTHREAD_DEPS) -o test2 $(CFLAGS) -lpthread

test3: test_pthread3.c $(PTHREAD_DEPS) $(PTHREAD_HEADERS)
		$(CC) test_pthread3.c $(PTHREAD_DEPS) -o test3 $(CFLAGS) -lpthread

test4: test_pthread4.c $(PTHREAD_DEPS) $(PTHREAD_HEADERS)
		$(CC) test_pthread4.c $(PTHREAD_DEPS) -o test4 $(CFLAGS)

.PHONY : clean
clean:
		rm  test1 test2 test3 test4