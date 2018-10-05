# Makefile for building and testing this project 
#
# Author: Sabrina Flemming
CC=gcc
CFLAGS=-I -Wall -std=c99 -g
OBJS=maze.o

# Build complete executable
all: maze

# Build maze executable
maze: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o maze

# Build maze object file
maze.o: maze.c 
	$(CC) $(CFLAGS) -c maze.c

node.o: node.h
	$(CC) $(CFLAGS) -c node.h


clean:
	rm *.o maze

# Testing make rules
run: maze
	./maze 5 7 
