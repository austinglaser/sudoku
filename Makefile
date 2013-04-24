CC	    = gcc
CFLAGS	= -Wall -Wextra -pedantic -std=c99

all: breadth_solve depth_solve

test: CFLAGS += -g
test: sudoku_breadth_test sudoku_depth_test

sudoku_breadth_test: sudoku.o fifo.o sudoku_test.o
	$(CC) -o sudoku_breadth_test sudoku.o fifo.o sudoku_test.o

sudoku_depth_test: sudoku.o lifo.o sudoku_test.o
	$(CC) -o sudoku_depth_test sudoku.o lifo.o sudoku_test.o

breadth_solve: sudoku.o fifo.o main.o
	$(CC) -o breadth_solve sudoku.o fifo.o main.o

depth_solve: sudoku.o lifo.o main.o
	$(CC) -o depth_solve sudoku.o lifo.o main.o

clean:
	rm -f sudoku_breadth_test sudoku_depth_test breadth_solve depth_solve *.o
