#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include "delayms.h"

struct mover {
	int x, y;
	char ch;
	chtype color;
};

void drawMover(WINDOW *win, const struct mover *mv)
{
	wattron(win, mv->color);
	mvwaddch(win, mv->y, mv->x, mv->ch);
	wattroff(win, mv->color);
}

void moveMover(WINDOW *win, struct mover *mv)
{
	int dir = rand() & 3; // perhaps faster than rand() % 4?
	
	switch (dir) {
		case 0: /* up */    mv->y--; break;
		case 1: /* down */  mv->y++; break;
		case 2: /* left */  mv->x--; break;
		case 3: /* right */ mv->x++; break;
	}
	
	// perform bounds checking and wrap around
	int ym, xm; getmaxyx(win, ym, xm);
	if (mv->y < 0) mv->y = ym;
	if (mv->y > ym) mv->y = 0;
	if (mv->x < 0) mv->x = xm;
	if (mv->x > xm) mv->x = 0;
}

int main(int argc, char *argv[])
{
	struct mover movers[26];

	srand((unsigned)time(NULL));
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(0);
	
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	
	int ym, xm; getmaxyx(stdscr, ym, xm);
	
	int i; for (i=0; i<26; i++) {
		movers[i].x = 2*i + xm/2 - 26;
		movers[i].y = ym / 2;
		movers[i].ch = 'A' + i;
		movers[i].color = COLOR_PAIR(i % 7);
	}
	
	int ch; while ((ch = getch()) != 'q') {
		static int doerase = 1;
		if (doerase) erase();
		
		for (i=0; i<26; i++) {
			drawMover(stdscr, &movers[i]);
			moveMover(stdscr, &movers[i]);
		}
		
		if (ch == 't') doerase = !doerase;
		
		refresh();
		delayms(50);
	}
	
	endwin();
	return 0;
}
