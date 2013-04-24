#include "sudoku.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void printUsage(char * call)
{
  printf("Usage: %s [-v]\n", call);
  printf("\twhere\t-v\tspecifies verbose mode.\n");
}

static void deleteSu(void * s)
{
  deleteSudoku((sudoku) s);
}

int main(int argc, char ** argv)
{
  int verbose = 0;
  if (argc == 2)
  {
    if (strcmp(argv[1], "-v") == 0) verbose = 1;
    else 
    {
      printUsage(argv[0]);
      return(1);
    }
  }
	else if (argc != 1) {
		printUsage(argv[0]);
		return -1;
	}
	sudoku s = newSudoku();

	printf("Enter the given values for a 9X9 sudoku, or enter 0 if the cell is not given.\n");

	int r, c, v;
	for (r = 1; r <= 9; ++r)
	{
		printf("Enter the elements of row %d, separated by spaces:\n", r);
		for(c = 1; c <= 9; ++c)
		{
			if (scanf("%d", &v) != 1) 
			{
				printf("Invalid input. Exiting.\n");
				return -1;
			}
			if (v > 9 || v < 0)
			{
				printf("Invalid input. Exiting.\n");
				return -1;
			}

			if (v != 0) setCell(s, r, c, v);
		}
	}

	printSudoku(s);

  queue q = newQueue(-1, deleteSu);

  putQueue(q, (void *) s);

  int slvd = 0, guesses = 0;

  while (1)
	{
		if (getQueue(q, (void **) &s)) 
		{
			printf("No possible solutions.\n");
			return 1;
		}

    reduce(s);
    if(verbose) printSudoku(s);

    slvd = checkSudoku(s);
    if (slvd == -1) deleteSudoku(s);
    else if (slvd == 1) 
    {
      printf("Solution:\n");
      printSudoku(s);
      deleteSudoku(s);
      printf("\nNumber of guesses made: %d\n", guesses);
      break;
    }
    else
    {
      guess(q, s);
      deleteSudoku(s);
      ++guesses;
    }
	}

  deleteQueue(q);
  return 0;
}
