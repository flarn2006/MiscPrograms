// Created by flarn2006 <flarn2006@gmail.com>
// Compile with libmatheval (http://www.gnu.org/software/libmatheval/)
// Also requires ncurses (Google it.)
// To compile without libmatheval, use -DNOLIBMATHEVAL.
// (Edit defaultFunction to change which function to graph then.)

#include <curses.h>
#include <string.h>
#include <math.h>
#ifndef NOLIBMATHEVAL
#include <matheval.h>
#endif

#define XMIN -2*M_PI
#define XMAX 2*M_PI
#define YMIN -M_PI
#define YMAX M_PI
#define XSCL 1
#define YSCL 1

typedef double (*yfunction)(double x);

struct _viewwin {
	double xmin, xmax;
	double ymin, ymax;
	double xscl, yscl;
};

typedef struct _viewwin viewwin;

#ifndef NOLIBMATHEVAL
void *eval = NULL;
#endif

int enableSlopeChars = 1;

double defaultFunction(double x)
{
	return sin(x);
}

double estimateSlope(yfunction func, double x, double accuracy)
{
	double y1 = func(x - accuracy);
	double y2 = func(x + accuracy);
	return (y2 - y1) / (2 * accuracy);
}

double scale(double value, double omin, double omax, double nmin, double nmax)
{
	double x = (value - omin) / (omax - omin);
	return x * (nmax - nmin) + nmin;
}

void plotPoint(WINDOW *win, const viewwin *view, double x, double y, char ch)
{
	int xm, ym; getmaxyx(win, ym, xm);
	int xp = scale(x, view->xmin, view->xmax, 0, xm);
	int yp = scale(y, view->ymin, view->ymax, ym, 0);
	mvwaddch(win, yp, xp, ch);
	//mvprintw(yp+1, xp+1, "[%c](%.2f, %.2f)", ch, x, y);
}

char slopeChar(double slope)
{
	double a = fabs(slope);
	if (a < 0.5)        return '=';
	else if (a < 1.5)   return slope>0 ? '/' : '\\';
	else                return '|';
}

void drawAxes(WINDOW *win, const viewwin *view)
{
	int xm, ym; getmaxyx(win, ym, xm);
	double x0 = scale(0, view->xmin, view->xmax, 0, xm);
	double y0 = scale(0, view->ymin, view->ymax, ym, 0);
	
	int i; for (i=0; i<=xm; i++) {
		mvwaddch(win, y0, i, '-');
	}
	for (i=0; i<=ym; i++) {
		mvwaddch(win, i, x0, '|');
	}
	
	/*double j; for (j = view->xmin; j <= view->xmax; j += view->xscl) {
		plotPoint(win, view, j, 0, '+');
	}
	
	for (j = view->ymin; j <= view->ymax; j += view->yscl) {
		plotPoint(win, view, 0, j, '+');
	}*/
	
	mvwaddch(win, y0, x0, '+');
}

void drawGraph(WINDOW *win, const viewwin *view, yfunction yfunc, int enableSlopeChars)
{
	int xm, ym; getmaxyx(win, ym, xm);
	double step = (view->xmax - view->xmin) / (xm + 1);
	double x; for (x = view->xmin; x <= view->xmax; x += step)
	{
		double y = yfunc(x);
		double d = estimateSlope(yfunc, x, step/2);
		plotPoint(win, view, x, y, enableSlopeChars ? slopeChar(d):'#');
	}
}

#ifndef NOLIBMATHEVAL
double performEval(double x)
{
	return evaluator_evaluate_x(eval, x);
}
#endif

void handleKey(int key, viewwin *view)
{
	double xshift = 0, yshift = 0;
	
	switch (key) {
		case KEY_UP:	yshift = 1; break;
		case KEY_DOWN:	yshift = -1; break;
		case KEY_LEFT:	xshift = -1; break;
		case KEY_RIGHT:	xshift = 1; break;
	}
	
	xshift *= (view->xmax - view->xmin) / 8;
	yshift *= (view->ymax - view->ymin) / 8;
	
	view->xmin += xshift; view->xmax += xshift;
	view->ymin += yshift; view->ymax += yshift;

	if (key == '-') {
		view->xmin *= 1.5; view->xmax *= 1.5;
		view->ymin *= 1.5; view->ymax *= 1.5;
	}

	if (key == '=') {
		view->xmin /= 1.5; view->xmax /= 1.5;
		view->ymin /= 1.5; view->ymax /= 1.5;
	}

	if (key == 's') enableSlopeChars = !enableSlopeChars;
}

int main(int argc, char *argv[])
{
	viewwin view;
	int key = 0;
	yfunction yfunc = defaultFunction;

	view.xmin = XMIN;
	view.xmax = XMAX;
	view.ymin = YMIN;
	view.ymax = YMAX;
	view.xscl = XSCL;
	view.yscl = YSCL;

#ifndef NOLIBMATHEVAL
	if (argc > 1) {
		if (strncmp(argv[1], "y=", 2) == 0) argv[1] += 2;
		eval = evaluator_create(argv[1]);
		
		if (!eval) {
			fprintf(stderr, "Error in expression!\n");
			return 1;
		}
		
		yfunc = performEval;
	}
#endif

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);

	while (key != (int)'q') {	
		erase();
	
		attron(COLOR_PAIR(1));
		drawAxes(stdscr, &view);
		attroff(COLOR_PAIR(1));
	
		drawGraph(stdscr, &view, yfunc, enableSlopeChars);
		refresh();
		key = getch(); handleKey(key, &view);
	}

	endwin();

#ifndef NOLIBMATHEVAL
	if (eval) evaluator_destroy(eval);
#endif

	return 0;
}
