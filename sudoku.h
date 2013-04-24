#ifndef _SUDOKU_H_
#define _SUDOKU_H_

/* ADT for a sudoku data structure */

/* The  ADT  */
typedef struct _sudoku * sudoku;

typedef struct _queue * queue;

/* Creates a new empty sudoku and returns a pointer to it.
 * Returns NULL if there is a problem allocating memory.
 */
sudoku newSudoku();

/* Returns a copy of the given sudoku
 */
sudoku copySudoku(sudoku s);

/* Sets the given cell to a determined value between
 * 1 and 9. Returns -1 if a value outside the specified
 * range is given, -2 if the cell is outside the range of
 * the sudoku, and otherwise returns 0.
 */
int setCell(sudoku s, int r, int c, int v);

/* Deletes the sudoku s and frees the memory used. */
void deleteSudoku(sudoku s);

/* Checks cell to see how many possibilities it has.
 * Returns the number of possibilities, and sets v
 * to the possibility if there is only one, and zero
 * otherwise.
 */
int checkCell(short int cell, int * v);

/* Checks to see if the sudoku is solved. Returns 0 
 * if there are still nondetermined cells, 1 if the
 * sudoku is solved, and -1 if there is a contradiction.
 */
int checkSudoku(sudoku s);

/* Applies known cells to reduce the possibilities
 * in the remaining cells.
 */
void reduce(sudoku s);

/* Makes a guess at the cell with the least number
 * of possibilities, and puts the new sudokus
 * into the queue. Returns 0 if successful, or -1
 * if there are no guessable cells (either solved
 * or contradictory
 */
int guess(queue q, sudoku s);

/* Prints the sudoku to stdout */
void printSudoku(sudoku s, int pretty);

/* Returns a new queue with the given maximum capacity. 
 * Takes a function to delete one element of data; if the
 * function is NULL, assumes the data does not need to
 * be explicitly freed.
 */
typedef void (* deleteFn)(void *);
queue newQueue(int capacity, deleteFn df);

/* Deletes a queue */
void deleteQueue(queue q);

/* Returns 1 if q is empty and otherwise 0 */
int isEmptyQueue(queue q);

/* Adds element e to q. Returns 0 if successful and -1 if
 * e could not be added to q because q is full
 */
int putQueue(queue q, void * e);

/* Sets e to point to the last element of q and removes the
 * element from q. Returns 0 if successful and -1 if e is
 * NULL. If q is empty, returns -2 and sets *e to NULL.
 */
int getQueue(queue q, void ** e);

/* Prints the elements of q in order. Requires a pointer
 * to a function that prints an element. Returns 0 if
 * successful and -1 if f is NULL.
 *//*
typedef void (* printFn)(void *);
int printQueue(queue q, printFn f);
*/
#endif
