// consine.c - Displays a 2D sine pattern in the console
// Created by flarn2006
// License: Creative Commons BY-SA 3.0

#include <curses.h>
#include <math.h>
#include <stdlib.h>

char levelchar(float value);

int main(int argc, const char *argv[]) {
	int x, y;
	int maxx, maxy;
	initscr();
	cbreak();
	noecho();
	getmaxyx(stdscr, maxy, maxx);
	
	for (y=0; y<=maxy; y++)
	for (x=0; x<=maxx; x++) {
		float fx = (float)x;
		float fy = (float)y;
		float value = fabsf(sin(fx/5.0)+sin(fy/2.5));
		value /= 2.0;
		char level = levelchar(value);
		mvaddch(y, x, level);
	}
	
	getch();
	endwin();
    return 0;
}

char levelchar(float value)
{
	const char *levels = "'.,-:;!|@#";
	int index = (int)(10*value) % 10;
	return levels[index];
}
