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
all_test: small_test_square doubles_square medium_test_square large_test_square 

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
doubles_square: maze
	$(info )
	$(info Running Large Square test)
	./maze 7.2 7.2

# All invalid tests (run individually)
even_args_test: maze
	$(info )
	$(info Running invalid args test)
	./maze 6 8
first_even_args_test: maze
	$(info )
	$(info Running invalid with first arg even test)
	./maze 6 7 
second_even_args_test: maze
	$(info )
	$(info Running invalid with second arg even test)
	./maze 7 6
first_arg_not_convertable: maze
	$(info )
	$(info Running invalid with first arg not number)
	./maze foo 6
second_arg_not_convertable: maze
	$(info )
	$(info Running invalid with second arg not number)
	./maze 6 foo
too_many_args: maze
	$(info )
	$(info Running invalid with too many args)
	./maze 7 7 7 
too_few_args_test: maze
	$(info )
	$(info Running too few args test)
	./maze 3
