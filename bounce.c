#include <curses.h>
#include "delayms.h"

struct ball {
	int y, x;
	int vy, vx;
};

void oneFrame(struct ball *b, int ymin, int ymax, int xmin, int xmax)
{
	if (b->y <= ymin || b->y >= ymax) b->vy *= -1;
	if (b->x <= xmin || b->x >= xmax) b->vx *= -1;

	b->y += b->vy;
	b->x += b->vx;
}

int main(int argc, char *argv[])
{
	int ymax, xmax;
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	getmaxyx(stdscr, ymax, xmax);

	mvprintw(0, 0, "Press Ctrl+C to stop...");
	refresh();
	delayms(1000);
	clear();

	struct ball b = {1, 1, 1, 1};

	for (;;) {
		mvaddch(b.y, b.x, 'O');
		refresh();
		delayms(50);
		mvaddch(b.y, b.x, ' ');
		oneFrame(&b, 0, ymax, 0, xmax);
	}

	endwin();
	return 0;
}
