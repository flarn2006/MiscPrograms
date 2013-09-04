#include <curses.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct point {
	double y;
	double x;
};

int ym, xm; //max Y/X terminal coordinates
unsigned int maximum = 0;

char getLevelChar(unsigned int level, unsigned int highest)
{
	const char *levels = ".-*O@";
	level *= 4; //avoid floating point math
	
	int index = level/highest;
	
	if (index > 4 || index < 0) {
		//shouldn't happen, but just in case...
		return '!'; //subtlely indicate there's a problem
	} else {
		if (highest == 0) return levels[4];
		else return levels[index];
	}
}

char plotPoint(unsigned int *plot, int width, const struct point *pt, char ch, int *y_out, int *x_out)
{
	int y = (int)(pt->y * (double)ym);
	int x = (int)(pt->x * (double)xm);
	char lch = 0;

	if (plot) {
		int index = y * width + x;
		if (++plot[index] > maximum) maximum = plot[index];
		lch = getLevelChar(plot[index], maximum);
	}
	
	if (ch > 0) mvaddch(y, x, ch);
	
	if (y_out) *y_out = y;
	if (x_out) *x_out = x;
	
	return lch;
}

void movePointHalfway(struct point *pt, const struct point *dest)
{
	pt->x = (pt->x + dest->x) / 2.0;
	pt->y = (pt->y + dest->y) / 2.0;
}

int main(int argc, char *argv[])
{
	struct point cursor;
	
	int vcount = 3;
	struct point vertices[vcount];
	vertices[0].x = 0.5; vertices[0].y = 0.1;
	vertices[1].x = 0.1; vertices[1].y = 0.9;
	vertices[2].x = 0.9; vertices[2].y = 0.9;

	srand((unsigned)time(NULL));

	initscr();
	cbreak();
	noecho();
	curs_set(0);
	getmaxyx(stdscr, ym, xm);
	
	size_t mem_size = (ym + 1) * (xm + 1) * sizeof(int);
	unsigned int *plot = malloc(mem_size);
	unsigned int highest = 0;
	
	if (!plot) {
		mvprintw(0, 0, "Error allocating memory!\n");
		printw("Press any key to exit.");
		endwin();
		return 1;
	}

	attron(A_BOLD);
	mvprintw(0, 0, "Use the W/A/D keys to select a vertex.\n");
	printw("Or press Space to choose one at random (try holding it down!)");
	attroff(A_BOLD);

	cursor = vertices[0];
	
	int ch = 0; while (ch != 'q') {
		if (ch == 'w' || ch == 'a' || ch == 'd' || ch == ' ') {
			int n; switch (ch) {
				case 'w': n = 0; break;
				case 'a': n = 1; break;
				case 'd': n = 2; break;
				case ' ': n = rand()%vcount; break;
			}
			int last_y, last_x;
			char ch = plotPoint(plot, xm+1, &cursor, 0, &last_y, &last_x);
			movePointHalfway(&cursor, &vertices[n]);
			
			// TODO: Draw all points with levels
			
			attron(A_REVERSE);
			mvaddch(last_y, last_x, ch);
			attroff(A_REVERSE);
		}

		attron(A_REVERSE);
		plotPoint(NULL, 0, &vertices[0], 'W', NULL, NULL);
		plotPoint(NULL, 0, &vertices[1], 'A', NULL, NULL);
		plotPoint(NULL, 0, &vertices[2], 'D', NULL, NULL);
		attroff(A_REVERSE);

		refresh();
		ch = getch();
	}

	free(plot);
	endwin();
	return 0;
}
