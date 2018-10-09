# Makefile for building and testing this project. This makefile is structured in such a way that it
# would be extensible and maintainable if this project were to grow in the future. Simple test cases
# are included to speed up the time it takes to test the program without needing any non standard 
# dependencies.
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


# Build header file to use Node struct
node.o: node.h
	$(CC) $(CFLAGS) -c node.h


clean:
	rm *.o maze

# Testing make rules
all_test: small_square_test medium_square_test large_square_test doubles_square_test

# All Valid tests
small_square_test: maze
	$(info Running Small Square test)
	./maze 7 
medium_square_test: maze
	$(info )
	$(info Running Medium Square test)
	./maze 21
large_square_test: maze
	$(info )
	$(info Running Large Square test)
	./maze 75
doubles_square_test: maze
	$(info )
	$(info Running Square test with a double)
	./maze 7.2

# All invalid tests (run individually)
even_args_test: maze
	$(info )
	$(info Running even args test)
	./maze 6
negative_args_test: maze
	$(info )
	$(info Running negative args test)
	./maze  -5
arg_not_convertable_test: maze
	$(info )
	$(info Running invalid with second arg not number)
	./maze foo
many_args_test: maze
	$(info )
	$(info Running invalid with too many args)
	./maze  7 7 
too_few_args_test: maze
	$(info )
	$(info Running too few args test)
	./maze 
