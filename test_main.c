#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>

int verbose = 0, pretty = 0, manual = 0; //global variables

void printUsage(char * call) //usage statement
{
  printf("Usage: %s [-vpm] <sudoku_file>\n", call);
  printf("\twhere\t-v\t\tspecifies 'verbose'      mode.\n");
  printf("\t\t-p\t\tspecifies 'pretty'       mode.\n");
	printf("\t\t-m\t\tspecifies 'manual entry' mode.\n");
	printf("\t\t<sudoku_file>\tis a text file with 9 lines, each specifying one line\n\t\t\t\tof an unsolved puzzle. zeros represent unfilled squares.\n");
}

static void dS(void * s) { deleteSudoku((sudoku) s); } //delete function for the queue (part of the price for using a general purpose queue)

/* Input:		Command line arguments argc and argv, and a file pointer f.
 * Returns: -1 if an error occurs, otherwise 0.
 *		-----------------------------------------------------------
 * Parses the command line arguments. This program takes between
 * one and 4 arguments: 3 possible flags, representing verbose print,
 * pretty print, and manual entry modes. If manual entry is not set,
 * one of the arguments must be a sudoku text file (nominally 9 lines
 * of integers between 1 and 9; in reality, any file with 81 integers
 * between 1 and 9 will work correctly). Returns an error value of -1
 * if the input is malformed; or if the filename is incorrect, and zero
 * otherwise. Sets *f to be a pointer to the sudoku file.
 */
int parseInput(int argc, char ** argv, FILE ** f)
{	
	verbose = 0;
	pretty = 0;
	manual = 0;
	
	*f = NULL;
	int i = 0;
	
	if (2 <= argc && argc <= 5) {
		for (i = 1; i < argc; i++) {
			if (argv[i][0] == '-') { //if the first character is a -, we have a flag
				switch (argv[i][1]) {
					case 'v':
						verbose = 1;
						break;
					case 'p':
						pretty = 1;
						break;
					case 'm':
						manual = 1;
						break;
					default:
						return -1;
				}
			} else { //otherwise, we assume it is a filename
				*f = fopen(argv[i], "r"); 
				if (!(*f)) {
					printf("%s is not a file.\n", argv[i]);
					free(*f);
					return -1;
				}
			}
		}
		return 0;
	}
	return 1;
}

/* Input:		The initialized pointer to the file.
 * Returns: NULL if an error occurs, otherwise the initialized board.
 *		-----------------------------------------------------------
 * Initializes the board. If manual entry is set, the function prompts
 * the user for input; otherwise, it reads from the file. The function
 * reads values in row over column, and sets the value of each cell if
 * it encounters an integer other than zero. Returns NULL if manual is
 * not set and f is NULL, or if it encounters malform input, and
 * otherwise returns an initialized sudoku.
 */
sudoku initBoard(FILE * f)
{
	sudoku s = newSudoku();
	
	int r, c, v = 0;
	if (manual) //manual entry mode, from the command line. Can also be used for redirecting input from a file
	{
		printf("Enter the given values for a 9X9 sudoku, or enter 0 if the cell is not given.\n");
		for (r = 1; r <= 9; ++r)
		{
			printf("Enter the elements of row %d:\n", r);
			for(c = 1; c <= 9; ++c)
			{
				if (scanf("%1d", &v) != 1) //scan character by character
				{
					printf("Malformed input. Exiting.\n");
					return NULL;
				}
				if (v > 9 || v < 0)
				{
					printf("Value out of range. Exiting.\n");
					return NULL;
				}
				
				if (v != 0) setCell(s, r, c, v);
			}
		}
	}
	else //read info from the file
	{ 
		if (!f) return NULL;
		for (r = 1; r <= 9; ++r)
		{
			for(c = 1; c <= 9; ++c)
			{
				if (fscanf(f, "%1d", &v) != 1) //scan character by character
				{
					printf("Malformed file. Exiting.\n");
					return NULL;
				}
				if (v > 9 || v < 0)
				{
					printf("Value out of range. Exiting.\n");
					return NULL;
				}
				
				if (v != 0) setCell(s, r, c, v);
			}
		}
	}
	return s;
}

/* Input:		An empty queue, and an initialized sudoku s, an empty queue
 *					for the solutions, and a pointer to a variable to store the
 *					number of guesses
 * Returns: NULL if no possible solutions exist or the queue is not
 *					empty, and otherwise the solution to the input puzzle.
 *		-----------------------------------------------------------
 * Solves the puzzle using backtracking. The solver is initialized by
 * putting the puzzle into the empty queue. Then, each iteration will
 * pull a board out of the queue, perform a simple reduction on that
 * board, and then make a guess on the cell which has the least number
 * of possibilities. If verbose is set, it prints each board before
 * making a guess, giving a sense of the whole solution process.
 * 
 * If there is nothing to pull out of the queue, there are no possible
 * solutions to the puzzle, and the function returns an error value of
 * NULL. If the queue is not empty at initialization, the function
 * prints an error message regardless of the state of the flags, and
 * returns an error.
 */
sudoku solve(queue q, sudoku s, int * guesses)
{
	if (!isEmptyQueue(q)) {
		printf("Error: call to solve with a non-empty queue");
		return NULL;
	}
	
  putQueue(q, (void *) s);
	
	*guesses = 0;
	
  int slvd = 0;
  while (1)
	{
		if (getQueue(q, (void **) &s)){
			return NULL;
		}
		
    reduce(s);
		
    if(verbose) {
      system("clear");
			printSudoku(s, pretty);
			printf("\n");
		}
		
    slvd = checkSudoku(s);
    if (slvd == -1) deleteSudoku(s);
		else if (slvd == 1) return s;
    else
    {
      if(guess(q, s)) {
				printf("Error: Full queue");
				return NULL;
			}
      deleteSudoku(s);
      (*guesses)++;
    }
	}
}

int main(int argc, char ** argv)
{
	FILE * f = NULL;
	
	if (parseInput(argc, argv, &f)) { //parse our input; exit if there's an error
		printUsage(argv[0]);
		return -1;
	}
	
	sudoku s = initBoard(f); //initialize the board; exit if there's an error
	if (!s) {
		printUsage(argv[0]);
		return -1;
	}
	
	fclose(f);
	
  printSudoku(s, true);


}
