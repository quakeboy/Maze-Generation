// MazeGen.cpp : Defines the entry point for the console application.
//
// need to remove a bug - one closed room generated sometimes ?!

#include "stdafx.h"
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define HCELLS 25
#define VCELLS 25

int _tmain(int argc, _TCHAR* argv[])
{
	srand ( (unsigned int)time(NULL) );

	int cells[HCELLS][VCELLS] = {0};
	int totalcells = HCELLS * VCELLS;
	int coveredcells = 1;

	int curx = 0;
	int cury = 0;

	int tempcellscount = 0;
	int tempcells[4][2]; //max of four only accessible each time

	while (coveredcells < totalcells)
	{
		tempcellscount = 0;

		//find how many cells are available
		if	(curx - 1 >= 0 && 
			((cells[curx - 1][cury] & 0xF0000000) == 0))
		{
			tempcells[tempcellscount][0] = curx - 1;
			tempcells[tempcellscount++][1] = cury;
		}

		if	(curx + 1 < HCELLS && 
			((cells[curx + 1][cury] & 0xF0000000) == 0))
		{
			tempcells[tempcellscount][0] = curx + 1;
			tempcells[tempcellscount++][1] = cury;
		}

		if	(cury - 1 >= 0 && 
			((cells[curx][cury - 1] & 0xF0000000) == 0))
		{
			tempcells[tempcellscount][0] = curx;
			tempcells[tempcellscount++][1] = cury - 1;
		}

		if	(cury + 1 < VCELLS && 
			((cells[curx][cury + 1] & 0xF0000000) == 0))
		{
			tempcells[tempcellscount]	[0] = curx;
			tempcells[tempcellscount++]	[1] = cury + 1;
		}

		int chosenx, choseny;

		//now according to tempcellscount, we know what to do
		if (tempcellscount == 0)
		{
			//no unexplored cells, so go back
			//fill in once backtracking info is done
			if		( (cells[curx][cury] & 0x10000000) != 0) curx = curx - 1;
			else if ( (cells[curx][cury] & 0x20000000) != 0) curx = curx + 1;
			else if ( (cells[curx][cury] & 0x40000000) != 0) cury = cury - 1;
			else if ( (cells[curx][cury] & 0x80000000) != 0) cury = cury + 1;
			continue;
		}
		else if (tempcellscount == 1)
		{
			//just one cell available so do your shit there
			chosenx = tempcells[0][0];
			choseny = tempcells[0][1];
		}
		else
		{
			int t_random_index;
			t_random_index = rand() % tempcellscount;

			//comes here if more than 1 cell is available to choose (max 4)
			chosenx = tempcells[t_random_index][0];
			choseny = tempcells[t_random_index][1];
		}

		//now that a cell is chosen, next what ?
		coveredcells++;

		//track how u came in, so that you can backtrace, also mark that you have visited
		int dirx = chosenx - curx;
		int diry = choseny - cury;

		if		(dirx == 1)		cells[chosenx][choseny] = (cells[chosenx][choseny] | 0x10000000);	//entered from left
		else if (dirx == -1)	cells[chosenx][choseny] = (cells[chosenx][choseny] | 0x20000000);	//right
		else if (diry == 1)		cells[chosenx][choseny] = (cells[chosenx][choseny] | 0x40000000);	//top
		else if (diry == -1)	cells[chosenx][choseny] = (cells[chosenx][choseny] | 0x80000000);	//bottom

		printf ("\n%d,%d", chosenx, choseny);

		curx = chosenx;
		cury = choseny;
	}

	printf ("\n\nmaze done now visualize biatch \n\n  ");
	char block = 219;

	for (int j = 0; j < VCELLS*2+1; j++)
	{
		for (int i = 0; i < HCELLS*2+1; i++)
		{
			//compulsory walls
			if (i%2 ==0 && j%2 ==0)
			{
				//only connectors
				printf("%c", block);
				continue;
			}
			else if (i%2 !=0 && j%2 !=0)
			{
				//only rooms
				printf (" ");
			}
			else
			{			
				//one of the walls, decide whether to print or not
				//check all four adjacent rooms to see if their path was from this point
				if (j % 2 == 0)
				{
					//for vertical walls only
					int ii = (i-1)/2;
					int jj = j/2;
					
					if ( (!(jj - 1 < 0) && (cells[ii][jj-1] & 0x80000000)) || 
						((jj < VCELLS) && (cells[ii][jj] & 0x40000000)))
							printf(" ");
					else
							printf("%c", block);
				}
				else
				{
					//for horizontal walls only

					int ii = i/2;
					int jj = (j-1)/2;
					
					if ( (!(ii - 1 < 0) && (cells[ii-1][jj] & 0x20000000)) || 
						((ii < HCELLS) && (cells[ii][jj] & 0x10000000)))
							printf(" ");
					else
							printf("%c", block);
				}				
			}
		}
		printf("\n  ");
	}

	_getch();
