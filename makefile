CC=gcc
CFLAGS=-I -Wall -std=c99 -g
OBJS=maze.o

all: maze

maze: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o maze

maze.o: maze.c
	$(CC) $(CFLAGS) -c maze.c

clean:
	rm *.o maze
