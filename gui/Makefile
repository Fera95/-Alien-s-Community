CC = gcc
SRC = gui.c main.c bridge.c route.c alien.c ./LPTHREAD/lpthread.c
RUN = alienCommunity
CFLAGS =  -I -Wall `pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags` -lpthread

all: build run

build: $(src)
	$(CC) $(SRC) -o $(RUN)  $(CFLAGS)

clean:
	rm $(RUN)

run:
	./$(RUN)