#define CELL(s, r, c)	s->cell[9*(r-1) + (c-1)]
#define ELEM(r, c) 9*(r-1) + (c-1)

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "sudoku.h"

struct _sudoku {
	short int cell[0];
};

sudoku newSudoku()
{
	sudoku s;

	s = (sudoku) malloc(sizeof(struct _sudoku) + 81*sizeof(short int));
	if (!s)
	{
		printf("Error allocating memory\n");
		exit(-1);
	}
	
	int i;
	short int all = 0;
	for (i = 0; i < 9; ++i) all += 1 << i;
	for (i = 0; i < 81; ++i) s->cell[i] = all;

	return s;
}

sudoku copySudoku(sudoku s)
{
	assert(s);

	sudoku cpy = newSudoku();
	memcpy(cpy->cell, s->cell, 81*sizeof(short int));

	return cpy;
}

int setCell(sudoku s, int r, int c, int v)
{
	if (v > 9 || v < 1) return -1;
	if (r > 9 || r < 1 ||
	    c > 9 || c < 1) return -2;
	assert(s);

	short int mask = 1 << (v - 1);
	CELL(s, r, c) = CELL(s, r, c) & mask;

  return 0;
}

void deleteSudoku(sudoku s)
{
	free(s);
}

int checkCell(short int cell, int * v)
{
	assert(v);
	int n = 0, i;
	for (i = 0; i < 9; ++i)
	{
		if ((cell >> i) & 1)
		{
			*v = i + 1;
			n += 1;
		}
	}
	
	if (n != 1) *v = 0; 

	return n;
}

int checkSudoku(sudoku s)
{
	int i, c = 0, v = 0, slvd = 1;
	for (i = 0; i < 81; ++i)
	{
		c = checkCell(s->cell[i], &v);
		if (c == 0) return -1;
		if (c != 1) slvd = 0;
	}
	
	return slvd;
}

void printSudoku(sudoku s, int pretty)
{
  if (!s) return;
	
	int n, r, c, v = 0;
	if (pretty) printf("+-----+-----+-----+\n");
	for (r = 1; r <= 9; ++r)
	{
		if (pretty) printf("|");
		for (c = 1; c <= 9; ++c)
		{
			n = checkCell(CELL(s, r, c), &v);
			if (n == 0) 	printf("x");
			else if (n == 1)		printf("%d", v);
      else  {
				if (pretty) printf(" ");
				else printf("0");
			}
			
			if (pretty && (c % 3 == 0)) printf("|");
			else if (pretty) printf(" ");
		}
		if (pretty && (r % 3 == 0)) printf("\n+-----+-----+-----+\n");
    else printf("\n");
	}
}


void reduce(sudoku s)
{
	assert(s);
	
	int found[81];
	int i, j;
	for (i = 0; i < 81; ++i) found[i] = 0;

  short int mask;
	int r, rs, c, cs, n = 0, v = 0, changed = 1;
	while (changed)
	{
		changed = 0;

		for (r = 1; r <= 9; ++r)
		{
			for (c = 1; c <= 9; ++c)
			{
				if (!found[ELEM(r, c)])
				{
					n = checkCell(CELL(s, r, c), &v);

					if (n == 1)
					{
						mask = ~CELL(s, r, c);

						for (i = 1; i <= 9; ++i) {
							if (i != r) CELL(s, i, c) &= mask;
							if (i != c) CELL(s, r, i) &= mask;
						}
            
            rs = r - (r - 1)%3;
            cs = c - (c - 1)%3;
						for (i = rs; i < rs + 3; ++i)
							for (j = cs; j < cs + 3; ++j)
								if (i != r || j != c) CELL(s, i, j) &= mask;

						found[ELEM(r, c)] = 1;
						changed = 1;
					}
				}
			}
		}
	}
}

int guess(queue q, sudoku s)
{
	assert(s);

  int v = 0;
  
  int min = 10;
  int r_min = 0, c_min = 0;

  int r, c, n;
  for (r = 1; r <= 9; ++r)
  {
    for (c = 1; c <= 9; ++c)
    {
      n = checkCell(CELL(s, r, c), &v);
      if (1 < n && n < min)
      {
        min = n;
        r_min = r;
        c_min = c;
        if (min == 2) break;
      }
    }
  }

  if (min == 10) return -1;

  int i;
  short int poss = CELL(s, r_min, c_min);
  sudoku cp;

  for (i = 0; i < 9; ++i)
  {
    if ((poss >> i) & 1)
    {
      cp = copySudoku(s);
      CELL(cp, r_min, c_min) = 1 << i;
      putQueue(q, (void *) cp);
    }
  }
  return 0;
}
