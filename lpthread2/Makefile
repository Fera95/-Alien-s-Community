CC=gcc

CFLAGS=-g -ggdb
ifeq (${THREAD_LIB}, PTHREAD)
CFLAGS += -lpthread
endif
ifeq (${THREAD_LIB}, lPTHREAD)
CFLAGS += -D lPTHREAD_LIB
endif

PTHREAD_DEPS=lpthread.c 
PTHREAD_HEADERS=lpthread.h 


THREAD_LIB=PTHREAD

default:  test1 test2 test3 test4


test1: test_pthread1.c $(PTHREAD_DEPS) $(PTHREAD_HEADERS)
		$(CC) test_pthread1.c $(PTHREAD_DEPS) -o test1 $(CFLAGS)

test2: test_pthread2.c $(PTHREAD_DEPS) $(PTHREAD_HEADERS)
		$(CC) test_pthread2.c $(PTHREAD_DEPS) -o test2 $(CFLAGS)

test3: test_pthread3.c $(PTHREAD_DEPS) $(PTHREAD_HEADERS)
		$(CC) test_pthread3.c $(PTHREAD_DEPS) -o test3 $(CFLAGS) -lpthread

test4: test_pthread4.c $(PTHREAD_DEPS) $(PTHREAD_HEADERS)
		$(CC) test_pthread4.c $(PTHREAD_DEPS) -o test4 $(CFLAGS)



.PHONY : clean
clean:
		rm  test1 test2 test3 test4