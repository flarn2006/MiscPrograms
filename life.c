#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define BV(x,y) ((field[x][y] == '#' || field[x][y] == 'X') ? 1:0);

char field[30][20];

int neighbors(int x, int y)
{
	int count = 0;
	if (x > 0 && y > 0)		count += BV(x-1,y-1);
	if (y > 0)				count += BV(x,y-1);
	if (x < 29 && y > 0)	count += BV(x+1,y-1);
	if (x > 0)				count += BV(x-1,y);
	if (x < 29)				count += BV(x+1,y);
	if (x > 0 && y < 19)	count += BV(x-1,y+1);
	if (y < 19)				count += BV(x,y+1);
	if (x < 29 && y < 19)	count += BV(x+1,y+1);
	return count;
}

int main (int argc, const char * argv[])
{
	int x, y;
	int gen = 0;
	initscr();
	cbreak();
	noecho();

	srand((unsigned)time(NULL));
	
	for (y=0; y<20; y++)
		for (x=0; x<30; x++)
		{
			field[x][y] = rand() % 2 ? '#':' ';
		}
	
	for (;;)
	{
		clear();
		mvprintw(22, 0, "Generation: %d", gen);
		for (y=0; y<20; y++)
			for (x=0; x<30; x++)
			{
				if (field[x][y] == '#') mvprintw(y, x*2, "[]");
			}
		refresh();
		if (getch() == 'q') break;
		
		gen ++;
		
		for (y=0; y<20; y++)
			for (x=0; x<30; x++)
			{
				if (neighbors(x,y) == 3 && field[x][y] == ' ')
					field[x][y] = '+';
				if (!(neighbors(x,y) == 2 || neighbors(x,y) == 3) && field[x][y] == '#')
					field[x][y] = 'X';
			}
		
		for (y=0; y<20; y++)
			for (x=0; x<30; x++)
			{
				if (field[x][y] == 'X') field[x][y] = ' ';
				if (field[x][y] == '+') field[x][y] = '#';
			}
	}
	
	endwin();
	return 0;
}

