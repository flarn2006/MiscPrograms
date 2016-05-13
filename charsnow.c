#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include "delayms.h"

#define DELAY 30

void dropchar(char ch, int column, int color, int *heights)
{
	int lowest = heights[column];
	int y; for (y=0; y<lowest; y++) {
		if (y > 0) mvaddch(y-1, column, ' ');
		attron(COLOR_PAIR(color));
		mvaddch(y, column, ch);
		attroff(COLOR_PAIR(color));
		delayms(DELAY);
		refresh();
	}

	heights[column] --;
}

int main(int argc, char *argv[])
{
	int ymax, xmax;
	int *heights;

	initscr();
	cbreak();
	noecho();
	curs_set(0);
	srand((unsigned)time(NULL));

	getmaxyx(stdscr, ymax, xmax);
	heights = malloc(sizeof(int) * (xmax + 1));
	int i; for (i=0; i<=xmax; i++) {
		heights[i] = ymax;
	}

	clear();
	printw("Press Ctrl+C to stop...");
	refresh();
	delayms(1000);
	clear();

	start_color();
	for (i=1; i<=7; i++) {
		init_pair(i, i, 0);
	}

	while (1) {
		char ch = rand() % 94 + 33;
		int x = rand() % (xmax + 1);
		int color = rand() % 7 + 1;
		
		if (rand() & 1) attron(A_BOLD);
		dropchar(ch, x, color, heights);
		attroff(A_BOLD); /*if it wasn't set to begin with nothing will happen*/
	}

	return 0;
}
