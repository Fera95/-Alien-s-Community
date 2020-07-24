CC = gcc
SRC = gui.c main.c bridge.c route.c alien.c path.c ./LPTHREAD/lpthread.c 
RUN = alienCommunity
CFLAGS =    -I -Wall `pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags` -lpthread -lm

all: build run

build: $(src)
	$(CC) $(SRC) -g -o $(RUN)  $(CFLAGS)

clean:
	rm $(RUN)

run:
	./$(RUN)