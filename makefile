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
all_test: small_square_test small_rectangle_test medium_square_test \
	medium_rectangle_test large_square_test large_rectangle_test \
	doubles_arg1_test doubles_arg2_test

# All Valid tests
small_square_test: maze
	$(info Running Small Square test)
	./maze 7 7
small_rectangle_test: maze
	$(info Running Small Square test)
	./maze 7 9
medium_square_test: maze
	$(info )
	$(info Running Medium Square test)
	./maze 21 21
medium_rectangle_test: maze
	$(info )
	$(info Running Medium Square test)
	./maze 21 43
large_square_test: maze
	$(info )
	$(info Running Large Square test)
	./maze 75 75 
large_rectangle_test: maze
	$(info )
	$(info Running Large Square test)
	./maze 75 81
doubles_arg1_test: maze
	$(info )
	$(info Running Square test with a double)
	./maze 7.2 9
doubles_arg2_test: maze
	$(info )
	$(info Running Square test with a double)
	./maze 9 7.2
	
# All invalid tests (run individually)
even_arg1_test: maze
	$(info )
	$(info Running even arg1 test)
	./maze 6 7
even_arg2_test: maze
	$(info )
	$(info Running even arg2 test)
	./maze 7 6
arg1_not_convertable_test: maze
	$(info )
	$(info Running invalid with first arg not number)
	./maze foo 5
arg2_not_convertable_test: maze
	$(info )
	$(info Running invalid with second arg not number)
	./maze 1 foo
negative_arg1_test: maze
	$(info )
	$(info Running negative arg1 test)
	./maze  -5 7
negative_arg2_test: maze
	$(info )
	$(info Running negative arg2 test)
	./maze 7 -5
many_args_test: maze
	$(info )
	$(info Running invalid with too many args)
	./maze  7 7 9
too_few_args_test: maze
	$(info )
	$(info Running too few args test)
	./maze 3
too_small_arg1_test: maze
	$(info )
	$(info Running too small arg1 test)
	./maze 1 5
too_small_arg2_test: maze
	$(info )
	$(info Running too small arg2 test)
	./maze 5 1

