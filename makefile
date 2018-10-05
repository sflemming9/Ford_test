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
all_test: small_test_square medium_test_square large_test_square 

# All Valid tests
small_test_square: maze
	$(info Running Small Square test)
	./maze 7 7 
medium_test_square: maze
	$(info )
	$(info Running Medium Square test)
	./maze 21 21
large_test_square: maze
	$(info )
	$(info Running Large Square test)
	./maze 75 75

# All invalid tests (run individually)
even_args_test: maze
	$(info )
	$(info Running invalid args test)
	./maze 6 8
too_few_args_test: maze
	$(info )
	$(info Running too few args test)
	./maze 3
