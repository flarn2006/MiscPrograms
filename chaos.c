#include <curses.h>
#include <math.h>
#include <time.h>

struct point {
	double y;
	double x;
};

void plotPoint(WINDOW *win, struct point *pt, char ch)
{
	int ym, xm; getmaxyx(win, ym, xm);
	
	int y = (int)(pt->y * (double)ym);
	int x = (int)(pt->x * (double)xm);

	mvwaddch(win, y, x, ch);
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

	attron(A_BOLD);
	mvprintw(0, 0, "Use the W/A/D keys to select a vertex.\n");
	printw("Or press Space to choose one at random (try holding it down!)  ");
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
			plotPoint(stdscr, &cursor, '*');
			movePointHalfway(&cursor, &vertices[n]);
			attron(A_BOLD | A_BLINK);
			plotPoint(stdscr, &cursor, '*');
			attroff(A_BOLD | A_BLINK);
		}

		attron(A_REVERSE);
		plotPoint(stdscr, &vertices[0], 'W');
		plotPoint(stdscr, &vertices[1], 'A');
		plotPoint(stdscr, &vertices[2], 'D');
		attroff(A_REVERSE);

		refresh();
		ch = getch();
	}

	endwin();
	return 0;
}
